#include "extra-test-classes.h"
#include "fault-injection.h"
#include "shared-ptr.h"

#include <gtest/gtest.h>

namespace ct_test {

TEST(AllocationCallsTest, weak_ptr_allocations) {
  AllocationCounter ac;
  int* i_p = new int(1337);
  ct::WeakPtr<int> w_p;
  {
    ct::SharedPtr<int> s_p(i_p);
    w_p = s_p;
  }
  ac.assert_allocated(2, 1);
  EXPECT_FALSE(w_p.lock());
}

TEST(AllocationCallsTest, make_shared_weak_ptr_allocations) {
  AllocationCounter ac;
  ct::WeakPtr<int> w_p;
  {
    ct::SharedPtr<int> s_p = ct::make_shared<int>(42);
    w_p = s_p;
  }
  ac.assert_allocated(1, 1);
  EXPECT_FALSE(w_p.lock());
}

TEST(AllocationCallsTest, allocations) {
  AllocationCounter ac;
  int* i_p = new int(1337);
  {
    ct::SharedPtr<int> p(i_p);
    EXPECT_EQ(*i_p, *p);
  }
  ac.assert_allocated(2, 0);
}

TEST(AllocationCallsTest, make_shared_allocations) {
  AllocationCounter ac;
  {
    ct::SharedPtr<int> p = ct::make_shared<int>(42);
    EXPECT_EQ(*p, 42);
  }
  ac.assert_allocated(1, 0);
}

TEST(FaultInjectionTest, pointer_ctor) {
  faulty_run([] {
    bool deleted = false;
    DestructionTracker* ptr = new DestructionTracker(&deleted);
    try {
      ct::SharedPtr<DestructionTracker> sp(ptr);
    } catch (...) {
      FaultInjectionDisable dg;
      EXPECT_TRUE(deleted);
      throw;
    }
  });
}

TEST(FaultInjectionTest, pointer_ctor_with_custom_deleter) {
  faulty_run([] {
    bool deleted = false;
    int* ptr = new int(42);
    try {
      ct::SharedPtr<int> sp(ptr, TrackingDeleter<int>(&deleted));
    } catch (...) {
      FaultInjectionDisable dg;
      EXPECT_TRUE(deleted);
      throw;
    }
  });
}

TEST(FaultInjectionTest, reset_ptr) {
  faulty_run([] {
    bool deleted1 = false;
    bool deleted2 = false;

    FaultInjectionDisable dg1;
    DestructionTracker* ptr1 = new DestructionTracker(&deleted1);
    DestructionTracker* ptr2 = new DestructionTracker(&deleted2);
    ct::SharedPtr<DestructionTracker> sp(ptr1);
    dg1.reset();

    try {
      sp.reset(ptr2);
    } catch (...) {
      FaultInjectionDisable dg2;
      EXPECT_TRUE(deleted2);
      EXPECT_FALSE(deleted1);
      EXPECT_TRUE(sp.get() == ptr1);
      throw;
    }
  });
}

TEST(FaultInjectionTest, reset_ptr_with_custom_deleter) {
  faulty_run([] {
    bool deleted1 = false;
    bool deleted2 = false;

    FaultInjectionDisable dg1;
    int* ptr1 = new int(42);
    int* ptr2 = new int(43);
    ct::SharedPtr<int> sp(ptr1, TrackingDeleter<int>(&deleted1));
    dg1.reset();

    try {
      sp.reset(ptr2, TrackingDeleter<int>(&deleted2));
    } catch (...) {
      FaultInjectionDisable dg2;
      EXPECT_TRUE(deleted2);
      EXPECT_FALSE(deleted1);
      EXPECT_TRUE(sp.get() == ptr1);
      throw;
    }
  });
}

TEST(FaultInjectionTest, make_shared) {
  struct FaultyObject {
    explicit FaultyObject(int value)
        : value(value) {
      fault_injection_point();
    }

    int value;
  };

  faulty_run([] { ct::SharedPtr<FaultyObject> p = ct::make_shared<FaultyObject>(42); });
}

} // namespace ct_test
