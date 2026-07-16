#include "optional.h"

#include "checked-object.h"

#include <gtest/gtest.h>

namespace ct_test {

namespace {

struct OnlyCopyConstructible : CheckedObject {
  using CheckedObject::CheckedObject;

  OnlyCopyConstructible(const OnlyCopyConstructible&) = default;
  OnlyCopyConstructible& operator=(const OnlyCopyConstructible&) = delete;

  OnlyCopyConstructible(OnlyCopyConstructible&& other) = delete;
  OnlyCopyConstructible& operator=(OnlyCopyConstructible&&) = delete;
};

struct OnlyMoveConstructible : CheckedObject {
  using CheckedObject::CheckedObject;

  OnlyMoveConstructible(const OnlyMoveConstructible&) = delete;
  OnlyMoveConstructible& operator=(const OnlyMoveConstructible&) = delete;

  OnlyMoveConstructible(OnlyMoveConstructible&& other) = default;
  OnlyMoveConstructible& operator=(OnlyMoveConstructible&&) = delete;
};

struct OnlyCopyable : CheckedObject {
  using CheckedObject::CheckedObject;

  OnlyCopyable(const OnlyCopyable& other)
      : CheckedObject(other) {
    ++copy_ctor_calls;
  }

  OnlyCopyable& operator=(const OnlyCopyable& other) {
    CheckedObject::operator=(other);
    ++copy_assign_calls;
    return *this;
  }

  OnlyCopyable(OnlyCopyable&& other) = delete;
  OnlyCopyable& operator=(OnlyCopyable&& other) = delete;

  inline static size_t copy_ctor_calls = 0;
  inline static size_t copy_assign_calls = 0;
};

struct OnlyMovable : CheckedObject {
  using CheckedObject::CheckedObject;

  OnlyMovable(const OnlyMovable&) = delete;
  OnlyMovable& operator=(const OnlyMovable&) = delete;

  OnlyMovable(OnlyMovable&& other)
      : CheckedObject(std::move(other)) {
    ++move_ctor_calls;
  }

  OnlyMovable& operator=(OnlyMovable&& other) {
    CheckedObject::operator=(std::move(other));
    ++move_assign_calls;
    return *this;
  }

  inline static size_t move_ctor_calls = 0;
  inline static size_t move_assign_calls = 0;
};

class OptionalTest : public ::testing::Test {
protected:
  void SetUp() noexcept override {
    OnlyCopyable::copy_ctor_calls = 0;
    OnlyCopyable::copy_assign_calls = 0;
    OnlyMovable::move_ctor_calls = 0;
    OnlyMovable::move_assign_calls = 0;
  }

  CheckedObject::NoNewInstancesGuard instances_guard;
};

} // namespace

const ct::NullOpt* get_nullopt_ptr() noexcept {
  return &ct::nullopt;
}

const ct::InPlace* get_in_place_ptr() noexcept {
  return &ct::in_place;
}

TEST_F(OptionalTest, DefaultConstructor) {
  ct::Optional<CheckedObject> a;
  EXPECT_FALSE(a.has_value());
  EXPECT_FALSE(static_cast<bool>(a));
}

TEST_F(OptionalTest, DefaultConstructorNoInstances) {
  ct::Optional<CheckedObject> a;
  EXPECT_FALSE(a.has_value());
  EXPECT_FALSE(static_cast<bool>(a));
  instances_guard.expect_no_instances();
}

TEST_F(OptionalTest, ValueConstructor) {
  ct::Optional<CheckedObject> a(42);
  EXPECT_TRUE(a.has_value());
  EXPECT_TRUE(static_cast<bool>(a));
  EXPECT_EQ(*a, 42);
}

TEST_F(OptionalTest, ValueConstructorInitList) {
  ct::Optional<std::pair<int, int>> a({42, 55});
  EXPECT_TRUE(a.has_value());
  EXPECT_TRUE(static_cast<bool>(a));
  EXPECT_EQ(a->first, 42);
  EXPECT_EQ(a->second, 55);
}

TEST_F(OptionalTest, ValueConstructorExplicitConversion) {
  ct::Optional<std::string> a(std::string_view("hello"));
  EXPECT_TRUE(a.has_value());
  EXPECT_EQ(*a, "hello");
}

TEST_F(OptionalTest, ValueConstructorImplicitConversion) {
  ct::Optional<std::string> a = "hello";
  EXPECT_TRUE(a.has_value());
  EXPECT_EQ(*a, "hello");
}

TEST_F(OptionalTest, ValueConstructorLvalueReference) {
  struct RefWrapper {
    RefWrapper(int& x)
        : value(x) {}

    int& value;
  };

  int x = 42;
  ct::Optional<RefWrapper> a = x;
  EXPECT_TRUE(a.has_value());
  EXPECT_EQ(a->value, 42);

  a->value = 55;
  EXPECT_EQ(x, 55);
}

TEST_F(OptionalTest, Dereference) {
  ct::Optional<CheckedObject> a(42);
  EXPECT_EQ(*a, 42);
  EXPECT_EQ(*std::as_const(a), 42);
  EXPECT_EQ(*std::move(a), 42);
  EXPECT_EQ(*std::move(std::as_const(a)), 42);
}

TEST_F(OptionalTest, MemberAccess) {
  ct::Optional<CheckedObject> a(42);
  EXPECT_EQ(a->operator int(), 42);
  EXPECT_EQ(std::as_const(a)->operator int(), 42);
}

TEST_F(OptionalTest, HijackedAddressof) {
  struct ValueType {
    int x;

    void* operator&() = delete;
  };

  ct::Optional<ValueType> a(ValueType{42});
  EXPECT_TRUE(a->x == 42);
  EXPECT_TRUE(a->x == (*a).x);
  EXPECT_TRUE(&a->x == &(*a).x);
  EXPECT_TRUE(std::as_const(a)->x == 42);
  EXPECT_TRUE(&std::as_const(a)->x == &(*a).x);
}

TEST_F(OptionalTest, Reset) {
  ct::Optional<CheckedObject> a(42);
  EXPECT_TRUE(a.has_value());
  a.reset();
  EXPECT_FALSE(a.has_value());
  instances_guard.expect_no_instances();
}

TEST_F(OptionalTest, Dtor) {
  ct::Optional<CheckedObject> a(42);
  EXPECT_TRUE(a.has_value());
  EXPECT_EQ(*a, 42);
}

TEST_F(OptionalTest, CopyConstructor) {
  ct::Optional<OnlyCopyConstructible> a(ct::in_place, 42);
  ct::Optional<OnlyCopyConstructible> b = a;
  EXPECT_TRUE(b.has_value());
  EXPECT_EQ(*b, 42);
}

TEST_F(OptionalTest, CopyConstructorEmpty) {
  ct::Optional<OnlyCopyConstructible> a;
  ct::Optional<OnlyCopyConstructible> b = a;
  EXPECT_FALSE(b.has_value());
}

TEST_F(OptionalTest, MoveConstructor) {
  ct::Optional<OnlyMoveConstructible> a(ct::in_place, 42);
  ct::Optional<OnlyMoveConstructible> b = std::move(a);
  EXPECT_TRUE(b.has_value());
  EXPECT_EQ(*b, 42);
  EXPECT_TRUE(a.has_value());
  EXPECT_EQ(*a, 42);
}

TEST_F(OptionalTest, MoveConstructorEmpty) {
  ct::Optional<OnlyMoveConstructible> a;
  ct::Optional<OnlyMoveConstructible> b = std::move(a);
  EXPECT_FALSE(b.has_value());
  EXPECT_FALSE(a.has_value());
}

TEST_F(OptionalTest, CopyAssignmentEmptyEmpty) {
  ct::Optional<OnlyCopyable> a;
  ct::Optional<OnlyCopyable> b;
  b = a;
  EXPECT_FALSE(b.has_value());
  EXPECT_EQ(OnlyCopyable::copy_ctor_calls, 0);
  EXPECT_EQ(OnlyCopyable::copy_assign_calls, 0);
}

TEST_F(OptionalTest, CopyAssignmentToEmpty) {
  ct::Optional<OnlyCopyable> a(ct::in_place, 42);
  ct::Optional<OnlyCopyable> b;
  b = a;
  EXPECT_TRUE(b.has_value());
  EXPECT_EQ(*b, 42);
  EXPECT_EQ(OnlyCopyable::copy_ctor_calls, 1);
  EXPECT_EQ(OnlyCopyable::copy_assign_calls, 0);
}

TEST_F(OptionalTest, CopyAssignmentFromEmpty) {
  ct::Optional<OnlyCopyable> a;
  ct::Optional<OnlyCopyable> b(ct::in_place, 42);
  b = a;
  EXPECT_FALSE(b.has_value());
  EXPECT_EQ(OnlyCopyable::copy_ctor_calls, 0);
  EXPECT_EQ(OnlyCopyable::copy_assign_calls, 0);
}

TEST_F(OptionalTest, CopyAssignment) {
  ct::Optional<OnlyCopyable> a(ct::in_place, 42);
  ct::Optional<OnlyCopyable> b(ct::in_place, 41);
  b = a;
  EXPECT_TRUE(b.has_value());
  EXPECT_EQ(*b, 42);
  EXPECT_EQ(OnlyCopyable::copy_ctor_calls, 0);
  EXPECT_EQ(OnlyCopyable::copy_assign_calls, 1);
}

TEST_F(OptionalTest, MoveAssignmentEmptyEmpty) {
  ct::Optional<OnlyMovable> a;
  ct::Optional<OnlyMovable> b;
  b = std::move(a);
  EXPECT_FALSE(b.has_value());
  EXPECT_FALSE(a.has_value());
  EXPECT_EQ(OnlyMovable::move_ctor_calls, 0);
  EXPECT_EQ(OnlyMovable::move_assign_calls, 0);
}

TEST_F(OptionalTest, MoveAssignmentToEmpty) {
  ct::Optional<OnlyMovable> a(ct::in_place, 42);
  ct::Optional<OnlyMovable> b;
  b = std::move(a);
  EXPECT_TRUE(b.has_value());
  EXPECT_EQ(*b, 42);
  EXPECT_TRUE(a.has_value());
  EXPECT_EQ(*a, 42);
  EXPECT_EQ(OnlyMovable::move_ctor_calls, 1);
  EXPECT_EQ(OnlyMovable::move_assign_calls, 0);
}

TEST_F(OptionalTest, MoveAssignmentFromEmpty) {
  ct::Optional<OnlyMovable> a;
  ct::Optional<OnlyMovable> b(ct::in_place, 42);
  b = std::move(a);
  EXPECT_FALSE(b.has_value());
  EXPECT_FALSE(a.has_value());
  EXPECT_EQ(OnlyMovable::move_ctor_calls, 0);
  EXPECT_EQ(OnlyMovable::move_assign_calls, 0);
}

TEST_F(OptionalTest, MoveAssignment) {
  ct::Optional<OnlyMovable> a(ct::in_place, 42);
  ct::Optional<OnlyMovable> b(ct::in_place, 41);
  b = std::move(a);
  EXPECT_TRUE(b.has_value());
  EXPECT_EQ(*b, 42);
  EXPECT_TRUE(a.has_value());
  EXPECT_EQ(*a, 42);
  EXPECT_EQ(OnlyMovable::move_ctor_calls, 0);
  EXPECT_EQ(OnlyMovable::move_assign_calls, 1);
}

TEST_F(OptionalTest, NulloptConstructor) {
  {
    ct::Optional<CheckedObject> a = ct::nullopt;
    EXPECT_FALSE(a.has_value());
    EXPECT_FALSE(static_cast<bool>(a));
    instances_guard.expect_no_instances();
  }
  {
    ct::Optional<CheckedObject> a = {ct::nullopt};
    EXPECT_FALSE(a.has_value());
    EXPECT_FALSE(static_cast<bool>(a));
    instances_guard.expect_no_instances();
  }
  {
    ct::Optional<CheckedObject> a{ct::nullopt};
    EXPECT_FALSE(a.has_value());
    EXPECT_FALSE(static_cast<bool>(a));
    instances_guard.expect_no_instances();
  }
}

TEST_F(OptionalTest, NulloptAssignment) {
  ct::Optional<CheckedObject> a(42);
  a = ct::nullopt;
  EXPECT_FALSE(a.has_value());
  EXPECT_FALSE(static_cast<bool>(a));
  EXPECT_TRUE(noexcept(a = ct::nullopt));
  instances_guard.expect_no_instances();
}

TEST_F(OptionalTest, EmptyConstructorInt) {
  ct::Optional<int> a = {};
  EXPECT_FALSE(a.has_value());
  EXPECT_FALSE(static_cast<bool>(a));
}

TEST_F(OptionalTest, EmptyConstructorForStructWithDefaultConstructor) {
  ct::Optional<CheckedObject> a = {};
  EXPECT_FALSE(a.has_value());
  EXPECT_FALSE(static_cast<bool>(a));
}

TEST_F(OptionalTest, EmptyAssignmentInt) {
  ct::Optional<int> a(42);
  a = {};
  EXPECT_FALSE(a.has_value());
  EXPECT_FALSE(static_cast<bool>(a));
}

TEST_F(OptionalTest, EmptyAssignmentForStructWithDefaultConstructor) {
  struct A {};

  ct::Optional<A> a(A{});
  a = {};
  EXPECT_FALSE(a.has_value());
  EXPECT_FALSE(static_cast<bool>(a));
}

TEST_F(OptionalTest, EmptyAssignmentForStructWithoutDefaultConstructor) {
  ct::Optional<CheckedObject> a(42);
  a = {};
  EXPECT_FALSE(a.has_value());
  EXPECT_FALSE(static_cast<bool>(a));
  instances_guard.expect_no_instances();
}

TEST_F(OptionalTest, ValueAssignment) {
  ct::Optional<OnlyMovable> a(42);

  a = OnlyMovable(55);
  EXPECT_TRUE(a.has_value());
  EXPECT_EQ(*a, 55);
  EXPECT_EQ(a->move_ctor_calls, 0);
  EXPECT_EQ(a->move_assign_calls, 1);
}

TEST_F(OptionalTest, ValueAssignmentToEmpty) {
  ct::Optional<OnlyMovable> a;

  a = OnlyMovable(55);
  EXPECT_TRUE(a.has_value());
  EXPECT_EQ(*a, 55);
  EXPECT_EQ(a->move_ctor_calls, 1);
  EXPECT_EQ(a->move_assign_calls, 0);
}

TEST_F(OptionalTest, ValueAssignmentInitList) {
  ct::Optional<std::pair<int, int>> a(ct::in_place, 42, 55);

  a = {13, 37};
  EXPECT_TRUE(a.has_value());
  EXPECT_EQ(a->first, 13);
  EXPECT_EQ(a->second, 37);
}

TEST_F(OptionalTest, ValueAssignmentScalar) {
  ct::Optional<int> a(42);

  a = 55;
  EXPECT_TRUE(a.has_value());
  EXPECT_EQ(*a, 55);
}

TEST_F(OptionalTest, ValueAssignmentScalarConversion) {
  ct::Optional<int> a(42);

  short s = 55;
  a = s;
  EXPECT_TRUE(a.has_value());
  EXPECT_EQ(*a, 55);
}

TEST_F(OptionalTest, SwapNonEmpty) {
  ct::Optional<CheckedObject> a(42);
  ct::Optional<CheckedObject> b(55);

  swap(a, b);

  EXPECT_EQ(*a, 55);
  EXPECT_EQ(*b, 42);
}

TEST_F(OptionalTest, SwapEmptyRight) {
  ct::Optional<CheckedObject> a(42);
  ct::Optional<CheckedObject> b;

  swap(a, b);

  EXPECT_FALSE(a);
  EXPECT_EQ(*b, 42);
}

TEST_F(OptionalTest, SwapEmptyLeft) {
  ct::Optional<CheckedObject> a;
  ct::Optional<CheckedObject> b(55);

  swap(a, b);

  EXPECT_EQ(*a, 55);
  EXPECT_FALSE(b);
}

TEST_F(OptionalTest, SwapEmptyBoth) {
  ct::Optional<CheckedObject> a;
  ct::Optional<CheckedObject> b;

  swap(a, b);

  EXPECT_FALSE(a);
  EXPECT_FALSE(b);
}

namespace {

struct CustomSwap {
  CustomSwap(int value) noexcept
      : value(value) {}

  CustomSwap(const CustomSwap&) = delete;
  CustomSwap(CustomSwap&&) = default;

  CustomSwap& operator=(const CustomSwap&) = delete;
  CustomSwap& operator=(CustomSwap&&) = delete;

  friend void swap(CustomSwap& lhs, CustomSwap& rhs) noexcept {
    std::swap(lhs.value, rhs.value);
    ++lhs.value;
    ++rhs.value;
  }

  int value;
};

} // namespace

TEST_F(OptionalTest, SwapCustom) {
  ct::Optional<CustomSwap> a(42);
  ct::Optional<CustomSwap> b(55);

  swap(a, b);

  EXPECT_EQ(a->value, 56);
  EXPECT_EQ(b->value, 43);
}

TEST_F(OptionalTest, SwapEmptyCustom) {
  ct::Optional<CustomSwap> a(42);
  ct::Optional<CustomSwap> b;

  swap(a, b);

  EXPECT_FALSE(a);
  EXPECT_EQ(b->value, 42);
}

namespace {

struct NonDefaultConstructor {
  NonDefaultConstructor(int, int, int, std::unique_ptr<int>) {}
};

} // namespace

TEST_F(OptionalTest, InPlaceConstructor) {
  ct::Optional<NonDefaultConstructor> a(ct::in_place, 1, 2, 3, std::unique_ptr<int>());
  EXPECT_TRUE(a.has_value());
}

TEST_F(OptionalTest, Emplace) {
  ct::Optional<NonDefaultConstructor> a;
  NonDefaultConstructor& result = a.emplace(1, 2, 3, std::unique_ptr<int>());
  EXPECT_TRUE(a.has_value());
  EXPECT_EQ(&result, &*a);
}

TEST_F(OptionalTest, EmplaceThrow) {
  struct ThrowingConstructor {
    struct Exception : std::exception {
      using std::exception::exception;
    };

    ThrowingConstructor(bool should_throw, int) {
      if (should_throw) {
        throw Exception();
      }
    }
  };

  ct::Optional<ThrowingConstructor> a(ct::in_place, false, 42);
  EXPECT_THROW(a.emplace(true, 55), ThrowingConstructor::Exception);
  EXPECT_FALSE(a.has_value());
}

TEST_F(OptionalTest, CopyAssignmentThrow) {
  struct ThrowingCopy {
    struct Exception : std::exception {
      using std::exception::exception;
    };

    ThrowingCopy(bool should_throw_when_copied, int value)
        : should_throw_when_copied(should_throw_when_copied)
        , value(value) {}

    ThrowingCopy(const ThrowingCopy& other) {
      if (other.should_throw_when_copied) {
        throw Exception();
      }
    }

    ThrowingCopy& operator=(const ThrowingCopy&) = default;

    bool should_throw_when_copied;
    int value;
  };

  {
    ct::Optional<ThrowingCopy> a(ct::in_place, false, 42);
    EXPECT_NO_THROW(a = ThrowingCopy(true, 55));
    EXPECT_TRUE(a.has_value());
    EXPECT_EQ(a->value, 55);
  }
  {
    ct::Optional<ThrowingCopy> b;
    EXPECT_THROW(b = ThrowingCopy(true, 55), ThrowingCopy::Exception);
    EXPECT_FALSE(b.has_value());
  }
}

namespace {

struct ComparisonCounters {
  size_t equal = 0;
  size_t not_equal = 0;
  size_t less = 0;
  size_t less_equal = 0;
  size_t greater = 0;
  size_t greater_equal = 0;
};

struct CustomComparison {
  CustomComparison(int value, ComparisonCounters* counters)
      : value(value)
      , counters(counters) {}

  bool operator==(const CustomComparison& other) const {
    ++counters->equal;
    return this->value == other.value;
  }

  bool operator!=(const CustomComparison& other) const {
    ++counters->not_equal;
    return this->value != other.value;
  }

  bool operator<(const CustomComparison& other) const {
    ++counters->less;
    return this->value < other.value;
  }

  bool operator<=(const CustomComparison& other) const {
    ++counters->less_equal;
    return this->value <= other.value;
  }

  bool operator>(const CustomComparison& other) const {
    ++counters->greater;
    return this->value > other.value;
  }

  bool operator>=(const CustomComparison& other) const {
    ++counters->greater_equal;
    return this->value >= other.value;
  }

private:
  int value;
  ComparisonCounters* counters;
};

} // namespace

TEST_F(OptionalTest, ComparisonNonEmptyAndNonEmpty) {
  ComparisonCounters ca, cb;
  ct::Optional<CustomComparison> a(ct::in_place, 41, &ca);
  ct::Optional<CustomComparison> b(ct::in_place, 42, &cb);

  EXPECT_FALSE(a == b);
  EXPECT_TRUE(a != b);
  EXPECT_TRUE(a < b);
  EXPECT_TRUE(a <= b);
  EXPECT_FALSE(a > b);
  EXPECT_FALSE(a >= b);

  EXPECT_TRUE(a == a);
  EXPECT_FALSE(a != a);
  EXPECT_FALSE(a < a);
  EXPECT_TRUE(a <= a);
  EXPECT_FALSE(a > a);
  EXPECT_TRUE(a >= a);

  EXPECT_FALSE(b == a);
  EXPECT_TRUE(b != a);
  EXPECT_FALSE(b < a);
  EXPECT_FALSE(b <= a);
  EXPECT_TRUE(b > a);
  EXPECT_TRUE(b >= a);

  EXPECT_EQ(ca.equal, 2);
  EXPECT_EQ(ca.not_equal, 2);
  EXPECT_EQ(ca.less, 2);
  EXPECT_EQ(ca.less_equal, 2);
  EXPECT_EQ(ca.greater, 2);
  EXPECT_EQ(ca.greater_equal, 2);

  EXPECT_EQ(cb.equal, 1);
  EXPECT_EQ(cb.not_equal, 1);
  EXPECT_EQ(cb.less, 1);
  EXPECT_EQ(cb.less_equal, 1);
  EXPECT_EQ(cb.greater, 1);
  EXPECT_EQ(cb.greater_equal, 1);
}

TEST_F(OptionalTest, ComparisonNonEmptyAndEmpty) {
  ComparisonCounters ca;
  ct::Optional<CustomComparison> a(ct::in_place, 41, &ca);
  ct::Optional<CustomComparison> b;

  EXPECT_FALSE(a == b);
  EXPECT_TRUE(a != b);
  EXPECT_FALSE(a < b);
  EXPECT_FALSE(a <= b);
  EXPECT_TRUE(a > b);
  EXPECT_TRUE(a >= b);

  EXPECT_FALSE(b == a);
  EXPECT_TRUE(b != a);
  EXPECT_TRUE(b < a);
  EXPECT_TRUE(b <= a);
  EXPECT_FALSE(b > a);
  EXPECT_FALSE(b >= a);

  EXPECT_EQ(ca.equal, 0);
  EXPECT_EQ(ca.not_equal, 0);
  EXPECT_EQ(ca.less, 0);
  EXPECT_EQ(ca.less_equal, 0);
  EXPECT_EQ(ca.greater, 0);
  EXPECT_EQ(ca.greater_equal, 0);
}

TEST_F(OptionalTest, ComparisonEmptyAndEmpty) {
  ct::Optional<CustomComparison> a;
  ct::Optional<CustomComparison> b;

  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);
  EXPECT_FALSE(a < b);
  EXPECT_TRUE(a <= b);
  EXPECT_FALSE(a > b);
  EXPECT_TRUE(a >= b);

  EXPECT_TRUE(a == a);
  EXPECT_FALSE(a != a);
  EXPECT_FALSE(a < a);
  EXPECT_TRUE(a <= a);
  EXPECT_FALSE(a > a);
  EXPECT_TRUE(a >= a);

  EXPECT_TRUE(b == a);
  EXPECT_FALSE(b != a);
  EXPECT_FALSE(b < a);
  EXPECT_TRUE(b <= a);
  EXPECT_FALSE(b > a);
  EXPECT_TRUE(b >= a);
}

TEST_F(OptionalTest, ComparisonThreeWay) {
  static_assert(std::is_eq(ct::Optional<int>{} <=> ct::Optional<int>{}));
  static_assert(std::is_lt(ct::Optional<int>{} <=> ct::Optional<int>{1}));
  static_assert(std::is_gt(ct::Optional<int>{1} <=> ct::Optional<int>{}));
  static_assert(std::is_eq(ct::Optional<int>{1} <=> ct::Optional<int>{1}));
  static_assert(std::is_lt(ct::Optional<int>{1} <=> ct::Optional<int>{2}));
}

TEST_F(OptionalTest, TypeDeduction) {
  {
    ct::Optional opt = 42;
    static_assert(std::is_same_v<decltype(opt), ct::Optional<int>>);
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(*opt, 42);
  }
  {
    int x = 42;
    ct::Optional opt = x;
    static_assert(std::is_same_v<decltype(opt), ct::Optional<int>>);
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(*opt, 42);
  }
  {
    const int x = 42;
    ct::Optional opt = x;
    static_assert(std::is_same_v<decltype(opt), ct::Optional<int>>);
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(*opt, 42);
  }
  {
    ct::Optional x = 42;
    ct::Optional opt = x;
    static_assert(std::is_same_v<decltype(opt), ct::Optional<int>>);
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(*opt, 42);
  }
  {
    struct Dummy {};

    ct::Optional opt = Dummy{};
    static_assert(std::is_same_v<decltype(opt), ct::Optional<Dummy>>);
    EXPECT_TRUE(opt.has_value());
  }
  {
    const int& x = 42;
    ct::Optional opt = x;
    static_assert(std::is_same_v<decltype(opt), ct::Optional<int>>);
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(*opt, 42);
  }
  {
    const int* x = nullptr;
    ct::Optional opt = x;
    static_assert(std::is_same_v<decltype(opt), ct::Optional<const int*>>);
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(*opt, nullptr);
  }
  {
    int x[] = {1, 2, 3};
    ct::Optional opt = x;
    static_assert(std::is_same_v<decltype(opt), ct::Optional<int*>>);
    EXPECT_TRUE(opt.has_value());
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ((*opt)[0], 1);
  }
  {
    ct::Optional opt1 = OnlyMovable(42);
    static_assert(std::is_same_v<decltype(opt1), ct::Optional<OnlyMovable>>);

    ct::Optional opt2 = std::move(opt1);
    static_assert(std::is_same_v<decltype(opt2), ct::Optional<OnlyMovable>>);
    EXPECT_TRUE(opt2.has_value());
    EXPECT_EQ(*opt2, 42);
  }
}

} // namespace ct_test
