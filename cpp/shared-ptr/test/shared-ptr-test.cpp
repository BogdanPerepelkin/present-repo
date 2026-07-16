#include "shared-ptr.h"

#include "checked-object.h"
#include "extra-test-classes.h"

#include <gtest/gtest.h>

#include <string>

namespace ct_test {

class SharedPtrTest : public ::testing::Test {
protected:
  CheckedObject::NoNewInstancesGuard instances_guard;
};

TEST_F(SharedPtrTest, default_ctor) {
  ct::SharedPtr<CheckedObject> p;
  EXPECT_EQ(p.get(), nullptr);
  EXPECT_FALSE(static_cast<bool>(p));
}

TEST_F(SharedPtrTest, ptr_ctor) {
  CheckedObject* p = new CheckedObject(42);
  ct::SharedPtr<CheckedObject> q(p);
  EXPECT_TRUE(static_cast<bool>(q));
  EXPECT_EQ(q.get(), p);
  EXPECT_EQ(*q, 42);
}

TEST_F(SharedPtrTest, ptr_ctor_nullptr) {
  ct::SharedPtr<CheckedObject> p(nullptr);
  EXPECT_FALSE(static_cast<bool>(p));
  EXPECT_EQ(p.use_count(), 0);
}

TEST_F(SharedPtrTest, ptr_ctor_non_empty_nullptr) {
  ct::SharedPtr<CheckedObject> p(static_cast<CheckedObject*>(nullptr));
  EXPECT_FALSE(static_cast<bool>(p));
  EXPECT_EQ(p.use_count(), 1);
}

TEST_F(SharedPtrTest, const_dereferencing) {
  const ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  EXPECT_EQ(*p, 42);
  EXPECT_EQ(p->operator int(), 42);
}

TEST_F(SharedPtrTest, reset) {
  ct::SharedPtr<CheckedObject> q(new CheckedObject(42));
  EXPECT_TRUE(static_cast<bool>(q));
  q.reset();
  EXPECT_FALSE(static_cast<bool>(q));
}

TEST_F(SharedPtrTest, reset_nullptr) {
  ct::SharedPtr<CheckedObject> q;
  EXPECT_FALSE(static_cast<bool>(q));
  q.reset();
  EXPECT_FALSE(static_cast<bool>(q));
}

TEST_F(SharedPtrTest, reset_ptr) {
  ct::SharedPtr<CheckedObject> q(new CheckedObject(42));
  EXPECT_TRUE(static_cast<bool>(q));
  q.reset(new CheckedObject(43));
  EXPECT_EQ(*q, 43);
}

TEST_F(SharedPtrTest, copy_ctor) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  EXPECT_EQ(p.use_count(), 1);
  ct::SharedPtr<CheckedObject> q = p;
  EXPECT_TRUE(static_cast<bool>(p));
  EXPECT_TRUE(static_cast<bool>(q));
  EXPECT_TRUE(p == q);
  EXPECT_EQ(*p, 42);
  EXPECT_EQ(*q, 42);
  EXPECT_EQ(q.use_count(), 2);
}

TEST_F(SharedPtrTest, copy_ctor_nullptr) {
  ct::SharedPtr<CheckedObject> p;
  ct::SharedPtr<CheckedObject> q = p;
  EXPECT_FALSE(static_cast<bool>(p));
  EXPECT_FALSE(static_cast<bool>(q));
}

TEST_F(SharedPtrTest, move_ctor) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::SharedPtr<CheckedObject> q = std::move(p);
  EXPECT_FALSE(static_cast<bool>(p));
  EXPECT_TRUE(static_cast<bool>(q));
  EXPECT_EQ(*q, 42);
}

TEST_F(SharedPtrTest, move_ctor_nullptr) {
  ct::SharedPtr<CheckedObject> p;
  ct::SharedPtr<CheckedObject> q = std::move(p);
  EXPECT_FALSE(static_cast<bool>(p));
  EXPECT_FALSE(static_cast<bool>(q));
}

TEST_F(SharedPtrTest, copy_assignment_operator) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::SharedPtr<CheckedObject> q(new CheckedObject(43));
  p = q;
  EXPECT_EQ(*p, 43);
  EXPECT_TRUE(p == q);
}

TEST_F(SharedPtrTest, copy_assignment_operator_from_nullptr) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::SharedPtr<CheckedObject> q;
  p = q;
  EXPECT_FALSE(static_cast<bool>(p));
}

TEST_F(SharedPtrTest, copy_assignment_operator_to_nullptr) {
  ct::SharedPtr<CheckedObject> p;
  ct::SharedPtr<CheckedObject> q(new CheckedObject(43));
  p = q;
  EXPECT_EQ(*p, 43);
  EXPECT_TRUE(p == q);
}

TEST_F(SharedPtrTest, copy_assignment_operator_nullptr) {
  ct::SharedPtr<CheckedObject> p;
  ct::SharedPtr<CheckedObject> q;
  p = q;
  EXPECT_FALSE(static_cast<bool>(p));
}

TEST_F(SharedPtrTest, copy_assignment_operator_self) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  p = p;
  EXPECT_EQ(*p, 42);
}

TEST_F(SharedPtrTest, copy_assignment_operator_self_nullptr) {
  ct::SharedPtr<CheckedObject> p;
  p = p;
  EXPECT_FALSE(static_cast<bool>(p));
}

TEST_F(SharedPtrTest, move_assignment_operator) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::SharedPtr<CheckedObject> q(new CheckedObject(43));
  p = std::move(q);
  EXPECT_EQ(*p, 43);
  EXPECT_FALSE(static_cast<bool>(q));
}

TEST_F(SharedPtrTest, move_assignment_operator_from_nullptr) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::SharedPtr<CheckedObject> q;
  p = std::move(q);
  EXPECT_FALSE(static_cast<bool>(p));
  EXPECT_FALSE(static_cast<bool>(q));
}

TEST_F(SharedPtrTest, move_assignment_operator_to_nullptr) {
  ct::SharedPtr<CheckedObject> p;
  ct::SharedPtr<CheckedObject> q(new CheckedObject(43));
  p = std::move(q);
  EXPECT_EQ(*p, 43);
  EXPECT_FALSE(static_cast<bool>(q));
}

TEST_F(SharedPtrTest, move_assignment_operator_nullptr) {
  ct::SharedPtr<CheckedObject> p;
  ct::SharedPtr<CheckedObject> q;
  p = std::move(q);
  EXPECT_FALSE(static_cast<bool>(p));
  EXPECT_FALSE(static_cast<bool>(q));
}

TEST_F(SharedPtrTest, move_assignment_operator_self) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  p = std::move(p);
  EXPECT_EQ(*p, 42);
}

TEST_F(SharedPtrTest, move_assignment_operator_self_nullptr) {
  ct::SharedPtr<CheckedObject> p;
  p = std::move(p);
  EXPECT_FALSE(static_cast<bool>(p));
}

TEST_F(SharedPtrTest, make_shared) {
  ct::SharedPtr<CheckedObject> p = ct::make_shared<CheckedObject>(42);
  EXPECT_EQ(*p, 42);
}

TEST_F(SharedPtrTest, make_shared_forwarding) {
  struct Pair {
    Pair(int& x, double&& y)
        : x(x)
        , y(y) {}

    int& x;
    double y;
  };

  int x = 42;
  ct::SharedPtr<Pair> p = ct::make_shared<Pair>(x, 3.14);
  EXPECT_EQ(p->x, 42);
  EXPECT_EQ(&p->x, &x);
  EXPECT_EQ(p->y, 3.14);
}

TEST_F(SharedPtrTest, ptr_ctor_inheritance) {
  bool deleted = false;
  { ct::SharedPtr<DestructionTrackerBase> p(new DestructionTracker(&deleted)); }
  EXPECT_TRUE(deleted);
}

TEST_F(SharedPtrTest, reset_ptr_inheritance) {
  bool deleted = false;
  {
    ct::SharedPtr<DestructionTrackerBase> p;
    p.reset(new DestructionTracker(&deleted));
  }
  EXPECT_TRUE(deleted);
}

TEST_F(SharedPtrTest, custom_deleter) {
  bool deleted = false;
  { ct::SharedPtr<CheckedObject> p(new CheckedObject(42), TrackingDeleter<CheckedObject>(&deleted)); }
  EXPECT_TRUE(deleted);
}

TEST_F(SharedPtrTest, custom_deleter_reset) {
  bool deleted = false;
  {
    ct::SharedPtr<CheckedObject> p;
    p.reset(new CheckedObject(42), TrackingDeleter<CheckedObject>(&deleted));
  }
  EXPECT_TRUE(deleted);
}

TEST_F(SharedPtrTest, custom_deleter_final) {
  struct Deleter final : TrackingDeleter<CheckedObject> {
    using TrackingDeleter::TrackingDeleter;
  };

  bool deleted = false;
  { ct::SharedPtr<CheckedObject> p(new CheckedObject(42), Deleter(&deleted)); }
  EXPECT_TRUE(deleted);
}

TEST_F(SharedPtrTest, custom_deleter_function_pointer) {
  thread_local bool deleted;
  deleted = false;
  {
    auto deleter = [](CheckedObject* ptr) {
      deleted = true;
      delete ptr;
    };
    ct::SharedPtr<CheckedObject> p(new CheckedObject(42), static_cast<void (*)(CheckedObject*)>(deleter));
  }
  EXPECT_TRUE(deleted);
}

TEST_F(SharedPtrTest, custom_deleter_lvalue) {
  thread_local bool deleted;
  deleted = false;
  {
    auto deleter = [](CheckedObject* ptr) {
      deleted = true;
      delete ptr;
    };
    ct::SharedPtr<CheckedObject> p(new CheckedObject(42), deleter);
  }
  EXPECT_TRUE(deleted);
}

TEST_F(SharedPtrTest, inheritance_convertible) {
  using Base = ct::SharedPtr<DestructionTrackerBase>;
  using ConstBase = ct::SharedPtr<const DestructionTrackerBase>;
  using Derived = ct::SharedPtr<DestructionTracker>;
  using ConstDerived = ct::SharedPtr<const DestructionTracker>;

  EXPECT_TRUE((std::is_convertible_v<const Derived&, Base>) );
  EXPECT_TRUE(!(std::is_convertible_v<const Base&, Derived>) );

  EXPECT_TRUE((std::is_convertible_v<const ConstDerived&, ConstBase>) );
  EXPECT_TRUE(!(std::is_convertible_v<const ConstBase&, ConstDerived>) );

  EXPECT_TRUE((std::is_convertible_v<const Derived&, ConstBase>) );
  EXPECT_TRUE(!(std::is_convertible_v<const Base&, ConstDerived>) );

  EXPECT_TRUE(!(std::is_convertible_v<const ConstDerived&, Base>) );
  EXPECT_TRUE(!(std::is_convertible_v<const ConstBase&, Derived>) );
}

TEST_F(SharedPtrTest, aliasing_ctor) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  std::string x;
  ct::SharedPtr<std::string> q(p, &x);
  EXPECT_EQ(p.use_count(), 2);
  EXPECT_EQ(q.use_count(), 2);
  EXPECT_EQ(q.get(), &x);
}

TEST_F(SharedPtrTest, aliasing_ctor_nullptr_non_empty) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::SharedPtr<std::string> q(p, nullptr);
  EXPECT_EQ(p.use_count(), 2);
  EXPECT_EQ(q.use_count(), 2);
  EXPECT_EQ(q.get(), nullptr);
  EXPECT_TRUE(static_cast<bool>(p));
  EXPECT_FALSE(static_cast<bool>(q));
}

TEST_F(SharedPtrTest, aliasing_ctor_empty_non_nullptr) {
  ct::SharedPtr<CheckedObject> p;
  std::string x;
  ct::SharedPtr<std::string> q(p, &x);
  EXPECT_EQ(p.use_count(), 0);
  EXPECT_EQ(q.use_count(), 0);
  EXPECT_EQ(p.get(), nullptr);
  EXPECT_EQ(q.get(), &x);
  EXPECT_FALSE(static_cast<bool>(p));
  EXPECT_TRUE(static_cast<bool>(q));
}

TEST_F(SharedPtrTest, aliasing_move_ctor) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  std::string x;
  ct::SharedPtr<std::string> q(std::move(p), &x);
  EXPECT_EQ(p.use_count(), 0);
  EXPECT_EQ(q.use_count(), 1);
  EXPECT_EQ(p.get(), nullptr);
  EXPECT_EQ(q.get(), &x);
}

TEST_F(SharedPtrTest, aliasing_move_ctor_nullptr_non_empty) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::SharedPtr<std::string> q(std::move(p), nullptr);
  EXPECT_EQ(p.use_count(), 0);
  EXPECT_EQ(q.use_count(), 1);
  EXPECT_EQ(p.get(), nullptr);
  EXPECT_EQ(q.get(), nullptr);
  EXPECT_FALSE(static_cast<bool>(p));
  EXPECT_FALSE(static_cast<bool>(q));
}

TEST_F(SharedPtrTest, aliasing_move_ctor_empty_non_nullptr) {
  ct::SharedPtr<CheckedObject> p;
  std::string x;
  ct::SharedPtr<std::string> q(std::move(p), &x);
  EXPECT_EQ(p.use_count(), 0);
  EXPECT_EQ(q.use_count(), 0);
  EXPECT_EQ(p.get(), nullptr);
  EXPECT_EQ(q.get(), &x);
  EXPECT_FALSE(static_cast<bool>(p));
  EXPECT_TRUE(static_cast<bool>(q));
}

TEST_F(SharedPtrTest, copy_ctor_const) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::SharedPtr<const CheckedObject> q = p;
  EXPECT_EQ(*q, 42);
}

TEST_F(SharedPtrTest, move_ctor_const) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::SharedPtr<const CheckedObject> q = std::move(p);
  EXPECT_EQ(*q, 42);
  EXPECT_FALSE(static_cast<bool>(p));
}

TEST_F(SharedPtrTest, copy_assignment_operator_const) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::SharedPtr<const CheckedObject> q(new CheckedObject(43));
  q = p;
  EXPECT_EQ(*q, 42);
  EXPECT_EQ(*p, 42);
}

TEST_F(SharedPtrTest, copy_assignment_operator_const_to_nullptr) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::SharedPtr<const CheckedObject> q;
  q = p;
  EXPECT_EQ(*q, 42);
  EXPECT_EQ(*p, 42);
}

TEST_F(SharedPtrTest, copy_assignment_operator_const_from_nullptr) {
  ct::SharedPtr<CheckedObject> p;
  ct::SharedPtr<const CheckedObject> q(new CheckedObject(43));
  q = p;
  EXPECT_FALSE(static_cast<bool>(q));
  EXPECT_FALSE(static_cast<bool>(p));
}

TEST_F(SharedPtrTest, move_assignment_operator_const) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::SharedPtr<const CheckedObject> q(new CheckedObject(43));
  q = std::move(p);
  EXPECT_EQ(*q, 42);
  EXPECT_FALSE(static_cast<bool>(p));
}

TEST_F(SharedPtrTest, move_assignment_operator_const_to_nullptr) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::SharedPtr<const CheckedObject> q;
  q = std::move(p);
  EXPECT_EQ(*q, 42);
  EXPECT_FALSE(static_cast<bool>(p));
}

TEST_F(SharedPtrTest, move_assignment_operator_const_from_nullptr) {
  ct::SharedPtr<CheckedObject> p;
  ct::SharedPtr<const CheckedObject> q(new CheckedObject(43));
  q = std::move(p);
  EXPECT_FALSE(static_cast<bool>(q));
  EXPECT_FALSE(static_cast<bool>(p));
}

TEST_F(SharedPtrTest, copy_ctor_inheritance) {
  bool deleted = false;
  {
    DestructionTracker* ptr = new DestructionTracker(&deleted);
    ct::SharedPtr<DestructionTracker> d(ptr);
    {
      ct::SharedPtr<DestructionTrackerBase> b = d;
      EXPECT_EQ(b.get(), ptr);
      EXPECT_EQ(d.get(), ptr);
    }
    EXPECT_FALSE(deleted);
  }
  EXPECT_TRUE(deleted);
}

TEST_F(SharedPtrTest, move_ctor_inheritance) {
  bool deleted = false;
  {
    DestructionTracker* ptr = new DestructionTracker(&deleted);
    ct::SharedPtr<DestructionTracker> d(ptr);
    {
      ct::SharedPtr<DestructionTrackerBase> b = std::move(d);
      EXPECT_EQ(b.get(), ptr);
      EXPECT_FALSE(static_cast<bool>(d));
    }
    EXPECT_TRUE(deleted);
    deleted = false;
  }
  EXPECT_FALSE(deleted);
}

TEST_F(SharedPtrTest, copy_assignment_operator_inheritance) {
  bool deleted = false;
  {
    DestructionTracker* ptr = new DestructionTracker(&deleted);
    ct::SharedPtr<DestructionTracker> d(ptr);
    {
      ct::SharedPtr<DestructionTrackerBase> b(new DestructionTrackerBase());
      b = d;
      EXPECT_EQ(b.get(), ptr);
      EXPECT_EQ(d.get(), ptr);
    }
    EXPECT_FALSE(deleted);
  }
  EXPECT_TRUE(deleted);
}

TEST_F(SharedPtrTest, move_assignment_operator_inheritance) {
  bool deleted = false;
  {
    DestructionTracker* ptr = new DestructionTracker(&deleted);
    ct::SharedPtr<DestructionTracker> d(ptr);
    {
      ct::SharedPtr<DestructionTrackerBase> b(new DestructionTrackerBase());
      b = std::move(d);
      EXPECT_EQ(b.get(), ptr);
      EXPECT_FALSE(static_cast<bool>(d));
    }
    EXPECT_TRUE(deleted);
    deleted = false;
  }
  EXPECT_FALSE(deleted);
}

TEST_F(SharedPtrTest, equivalence) {
  ct::SharedPtr<CheckedObject> p1(new CheckedObject(42));
  ct::SharedPtr<CheckedObject> p2(new CheckedObject(43));

  EXPECT_FALSE(p1 == p2);
  EXPECT_TRUE(p1 != p2);
}

TEST_F(SharedPtrTest, equivalence_aliasing) {
  ct::SharedPtr<CheckedObject> p1(new CheckedObject(42));
  ct::SharedPtr<CheckedObject> p2(new CheckedObject(43));
  ct::SharedPtr<CheckedObject> q1(p2, p1.get());
  ct::SharedPtr<CheckedObject> q2(p1, p2.get());

  EXPECT_TRUE(p1 == q1);
  EXPECT_FALSE(p1 != q1);

  EXPECT_TRUE(p2 == q2);
  EXPECT_FALSE(p2 != q2);

  EXPECT_FALSE(p1 == q2);
  EXPECT_TRUE(p1 != q2);

  EXPECT_FALSE(p2 == q1);
  EXPECT_TRUE(p2 != q1);
}

TEST_F(SharedPtrTest, equivalence_self) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));

  EXPECT_TRUE(p == p);
  EXPECT_FALSE(p != p);
}

TEST_F(SharedPtrTest, equivalence_nullptr) {
  ct::SharedPtr<CheckedObject> p;

  EXPECT_TRUE(p == nullptr);
  EXPECT_FALSE(p != nullptr);
  EXPECT_TRUE(nullptr == p);
  EXPECT_FALSE(nullptr != p);
}

TEST_F(SharedPtrTest, make_shared_non_movable) {
  struct NonMovableNonCopyable {
  public:
    NonMovableNonCopyable(std::string str, double d)
        : str(std::move(str))
        , d(d) {}

    [[maybe_unused]] NonMovableNonCopyable(const NonMovableNonCopyable& other) = delete;
    [[maybe_unused]] NonMovableNonCopyable(NonMovableNonCopyable&& other) = delete;
    NonMovableNonCopyable& operator=(const NonMovableNonCopyable& other) = delete;
    NonMovableNonCopyable& operator=(NonMovableNonCopyable&& other) = delete;

  public:
    std::string str;
    double d;
  };

  auto p = ct::make_shared<NonMovableNonCopyable>("aboba", 0.0);
  EXPECT_EQ(p->str, "aboba");
  EXPECT_EQ(p->d, 0.0);
}

} // namespace ct_test
