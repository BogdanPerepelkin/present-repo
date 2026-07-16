#include "checked-object.h"
#include "shared-ptr.h"

#include <gtest/gtest.h>

#include <string>

namespace ct_test {

class WeakPtrTest : public ::testing::Test {
protected:
  CheckedObject::NoNewInstancesGuard instances_guard;
};

TEST_F(WeakPtrTest, lock) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::WeakPtr<CheckedObject> q = p;
  ct::SharedPtr<CheckedObject> r = q.lock();
  EXPECT_TRUE(r == p);
  EXPECT_EQ(*r, 42);
}

TEST_F(WeakPtrTest, lock_default_constructed) {
  ct::WeakPtr<CheckedObject> q;
  EXPECT_FALSE(static_cast<bool>(q.lock()));
}

TEST_F(WeakPtrTest, lock_expired) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));

  ct::WeakPtr<CheckedObject> q = p;
  p.reset();
  instances_guard.expect_no_instances();

  ct::SharedPtr<CheckedObject> r = q.lock();
  EXPECT_FALSE(static_cast<bool>(r));
}

TEST_F(WeakPtrTest, lock_empty_nonnull) {
  ct::SharedPtr<CheckedObject> p;

  std::string x;
  ct::SharedPtr<std::string> q(p, &x);

  ct::WeakPtr<std::string> r = q;
  EXPECT_FALSE(static_cast<bool>(r.lock()));
}

TEST_F(WeakPtrTest, use_count) {
  ct::SharedPtr<CheckedObject> p1(new CheckedObject(42));
  ct::SharedPtr<CheckedObject> p2 = p1;
  ct::WeakPtr<CheckedObject> q = p2;

  EXPECT_EQ(p1.use_count(), 2);
  EXPECT_EQ(3, q.lock().use_count());
}

TEST_F(WeakPtrTest, reset) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::WeakPtr<CheckedObject> q1 = p;
  ct::WeakPtr<CheckedObject> q2 = p;
  q1.reset();
  EXPECT_FALSE(static_cast<bool>(q1.lock()));
  EXPECT_TRUE(static_cast<bool>(p));
  EXPECT_TRUE(q2.lock() == p);
}

TEST_F(WeakPtrTest, reset_nullptr) {
  ct::WeakPtr<CheckedObject> q;
  EXPECT_FALSE(static_cast<bool>(q.lock()));
  q.reset();
  EXPECT_FALSE(static_cast<bool>(q.lock()));
}

TEST_F(WeakPtrTest, make_shared) {
  ct::SharedPtr<CheckedObject> p = ct::make_shared<CheckedObject>(42);
  EXPECT_EQ(*p, 42);
}

TEST_F(WeakPtrTest, make_shared_weak_ptr) {
  ct::WeakPtr<CheckedObject> p;
  {
    ct::SharedPtr<CheckedObject> q = ct::make_shared<CheckedObject>(42);
    p = q;
  }
  instances_guard.expect_no_instances();
}

TEST_F(WeakPtrTest, copy_ctor) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::WeakPtr<CheckedObject> q = p;
  ct::WeakPtr<CheckedObject> r = q;
  EXPECT_TRUE(r.lock() == p);
  EXPECT_TRUE(q.lock() == p);
}

TEST_F(WeakPtrTest, copy_ctor_nullptr) {
  ct::WeakPtr<CheckedObject> p;
  ct::WeakPtr<CheckedObject> q = p;
  EXPECT_FALSE(static_cast<bool>(p.lock()));
  EXPECT_FALSE(static_cast<bool>(q.lock()));
}

TEST_F(WeakPtrTest, move_ctor) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::WeakPtr<CheckedObject> q = p;
  ct::WeakPtr<CheckedObject> r = std::move(q);
  EXPECT_TRUE(r.lock() == p);
  EXPECT_FALSE(static_cast<bool>(q.lock()));
}

TEST_F(WeakPtrTest, move_ctor_nullptr) {
  ct::WeakPtr<CheckedObject> p;
  ct::WeakPtr<CheckedObject> q = p;
  EXPECT_FALSE(static_cast<bool>(p.lock()));
  EXPECT_FALSE(static_cast<bool>(q.lock()));
}

TEST_F(WeakPtrTest, copy_assignment_operator) {
  ct::SharedPtr<CheckedObject> p1(new CheckedObject(42));
  ct::WeakPtr<CheckedObject> q1 = p1;
  ct::SharedPtr<CheckedObject> p2(new CheckedObject(43));
  ct::WeakPtr<CheckedObject> q2 = p2;

  q1 = q2;

  EXPECT_TRUE(q1.lock() == p2);
  EXPECT_TRUE(q2.lock() == p2);
}

TEST_F(WeakPtrTest, copy_assignment_operator_from_nullptr) {
  ct::SharedPtr<CheckedObject> p1(new CheckedObject(42));
  ct::WeakPtr<CheckedObject> q1 = p1;
  ct::WeakPtr<CheckedObject> q2;

  q1 = q2;

  EXPECT_FALSE(static_cast<bool>(q1.lock()));
  EXPECT_FALSE(static_cast<bool>(q2.lock()));
}

TEST_F(WeakPtrTest, copy_assignment_operator_to_nullptr) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::WeakPtr<CheckedObject> q1;
  ct::WeakPtr<CheckedObject> q2 = p;

  q1 = q2;

  EXPECT_TRUE(q1.lock() == p);
  EXPECT_TRUE(q2.lock() == p);
}

TEST_F(WeakPtrTest, copy_assignment_operator_nullptr) {
  ct::WeakPtr<CheckedObject> q1;
  ct::WeakPtr<CheckedObject> q2;

  q1 = q2;

  EXPECT_FALSE(static_cast<bool>(q1.lock()));
  EXPECT_FALSE(static_cast<bool>(q2.lock()));
}

TEST_F(WeakPtrTest, copy_assignment_operator_self) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::WeakPtr<CheckedObject> q = p;

  q = q;

  EXPECT_TRUE(q.lock() == p);
}

TEST_F(WeakPtrTest, copy_assignment_operator_self_nullptr) {
  ct::WeakPtr<CheckedObject> q;

  q = q;

  EXPECT_FALSE(static_cast<bool>(q.lock()));
}

TEST_F(WeakPtrTest, shared_assignment_operator) {
  ct::SharedPtr<CheckedObject> p1(new CheckedObject(42));
  ct::SharedPtr<CheckedObject> p2(new CheckedObject(43));
  ct::WeakPtr<CheckedObject> q = p1;

  q = p2;

  EXPECT_TRUE(q.lock() == p2);
}

TEST_F(WeakPtrTest, shared_assignment_operator_aliased) {
  CheckedObject x(43);
  ct::SharedPtr<CheckedObject> p1(new CheckedObject(42));
  ct::SharedPtr<CheckedObject> p2(p1, &x);
  ct::WeakPtr<CheckedObject> q = p1;

  q = p2;

  EXPECT_TRUE(q.lock() == p2);
}

TEST_F(WeakPtrTest, move_assignment_operator) {
  ct::SharedPtr<CheckedObject> p1(new CheckedObject(42));
  ct::WeakPtr<CheckedObject> q1 = p1;
  ct::SharedPtr<CheckedObject> p2(new CheckedObject(43));
  ct::WeakPtr<CheckedObject> q2 = p2;

  q1 = std::move(q2);

  EXPECT_TRUE(q1.lock() == p2);
  EXPECT_FALSE(static_cast<bool>(q2.lock()));
}

TEST_F(WeakPtrTest, move_assignment_operator_from_nullptr) {
  ct::SharedPtr<CheckedObject> p1(new CheckedObject(42));
  ct::WeakPtr<CheckedObject> q1 = p1;
  ct::WeakPtr<CheckedObject> q2;

  q1 = std::move(q2);

  EXPECT_FALSE(static_cast<bool>(q1.lock()));
  EXPECT_FALSE(static_cast<bool>(q2.lock()));
}

TEST_F(WeakPtrTest, move_assignment_operator_to_nullptr) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::WeakPtr<CheckedObject> q1;
  ct::WeakPtr<CheckedObject> q2 = p;

  q1 = std::move(q2);

  EXPECT_TRUE(q1.lock() == p);
  EXPECT_FALSE(static_cast<bool>(q2.lock()));
}

TEST_F(WeakPtrTest, move_assignment_operator_nullptr) {
  ct::WeakPtr<CheckedObject> q1;
  ct::WeakPtr<CheckedObject> q2;

  q1 = std::move(q2);

  EXPECT_FALSE(static_cast<bool>(q1.lock()));
  EXPECT_FALSE(static_cast<bool>(q2.lock()));
}

TEST_F(WeakPtrTest, move_assignment_operator_self) {
  ct::SharedPtr<CheckedObject> p(new CheckedObject(42));
  ct::WeakPtr<CheckedObject> q = p;

  q = std::move(q);

  EXPECT_TRUE(q.lock() == p);
}

TEST_F(WeakPtrTest, move_assignment_operator_self_nullptr) {
  ct::WeakPtr<CheckedObject> q;

  q = std::move(q);

  EXPECT_FALSE(static_cast<bool>(q.lock()));
}

namespace {

struct Base {
  Base() = default;

  Base(const Base&) = delete;
  Base& operator=(const Base&) = delete;
  Base(Base&&) = delete;
  Base& operator=(Base&&) = delete;
};

struct Derived : Base {};

} // namespace

TEST_F(WeakPtrTest, shared_ctor_inheritance) {
  ct::SharedPtr<Derived> p(new Derived());
  ct::WeakPtr<Base> q = p;

  EXPECT_TRUE(q.lock() == p);
}

TEST_F(WeakPtrTest, copy_ctor_inheritance) {
  ct::SharedPtr<Derived> p(new Derived());
  ct::WeakPtr<Derived> q1 = p;
  ct::WeakPtr<Base> q2 = q1;

  EXPECT_TRUE(q1.lock() == p);
  EXPECT_TRUE(q2.lock() == p);
}

TEST_F(WeakPtrTest, move_ctor_inheritance) {
  ct::SharedPtr<Derived> p(new Derived());
  ct::WeakPtr<Derived> q1 = p;
  ct::WeakPtr<Base> q2 = std::move(q1);

  EXPECT_FALSE(static_cast<bool>(q1.lock()));
  EXPECT_TRUE(q2.lock() == p);
}

TEST_F(WeakPtrTest, copy_assignment_operator_inheritance) {
  ct::SharedPtr<Derived> p1(new Derived());
  ct::WeakPtr<Derived> q1 = p1;
  ct::SharedPtr<Base> p2(new Base());
  ct::WeakPtr<Base> q2 = p2;

  q2 = q1;

  EXPECT_TRUE(q1.lock() == p1);
  EXPECT_TRUE(q2.lock() == p1);
}

TEST_F(WeakPtrTest, move_assignment_operator_inheritance) {
  ct::SharedPtr<Derived> p1(new Derived());
  ct::WeakPtr<Derived> q1 = p1;
  ct::SharedPtr<Base> p2(new Base());
  ct::WeakPtr<Base> q2 = p2;

  q2 = std::move(q1);

  EXPECT_FALSE(static_cast<bool>(q1.lock()));
  EXPECT_TRUE(q2.lock() == p1);
}

} // namespace ct_test
