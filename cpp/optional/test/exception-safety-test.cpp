#include "checked-object.h"
#include "fault-injection.h"
#include "optional.h"

#include <gtest/gtest.h>

#include <exception>
#include <utility>

namespace ct_test {

namespace {

class FaultyObject : private CheckedObject {
public:
  explicit FaultyObject(int data)
      : CheckedObject(data) {
    fault_injection_point();
  }

  FaultyObject(const FaultyObject& other)
      : CheckedObject(other) {
    fault_injection_point();
  }

  FaultyObject& operator=(const FaultyObject& c) {
    fault_injection_point();
    CheckedObject::operator=(c);
    return *this;
  }

  operator int() const {
    fault_injection_point();
    return CheckedObject::operator int();
  }

  friend void swap(FaultyObject& lhs, FaultyObject& rhs) {
    fault_injection_point();
    std::swap(static_cast<CheckedObject&>(lhs), static_cast<CheckedObject&>(rhs));
  }
};

template <typename... Ts>
class StrongExceptionSafetyGuard {
public:
  explicit StrongExceptionSafetyGuard(const Ts&... values) noexcept
      : _guards(values...) {}

private:
  std::tuple<StrongExceptionSafetyGuard<Ts>...> _guards;
};

template <typename T>
struct IsOptional : std::false_type {};

template <typename T>
struct IsOptional<ct::Optional<T>> : std::true_type {};

template <typename T>
concept is_optional = IsOptional<T>::value;

template <typename T>
class StrongExceptionSafetyGuard<T> {
public:
  explicit StrongExceptionSafetyGuard(const T& value) noexcept
      : _ref(value)
      , _expected((FaultInjectionDisable{}, value)) {}

  StrongExceptionSafetyGuard(const StrongExceptionSafetyGuard&) = delete;

  ~StrongExceptionSafetyGuard() {
    if (std::uncaught_exceptions() > 0) {
      FaultInjectionDisable dg;
      if constexpr (is_optional<T>) {
        EXPECT_EQ(_ref.has_value(), _expected.has_value());
        if (_ref.has_value() && _expected.has_value()) {
          EXPECT_EQ(*_ref, *_expected);
        }
      } else {
        EXPECT_EQ(_ref, _expected);
      }
    }
  }

private:
  const T& _ref;
  T _expected;
};

} // namespace

TEST(ExceptionSafetyTest, ValueConstructor) {
  FaultyObject value(42);

  faulty_run([&] {
    StrongExceptionSafetyGuard guard(value);
    ct::Optional<FaultyObject> opt = std::move(value);
    std::ignore = opt;
  });
}

TEST(ExceptionSafetyTest, CopyConstructor) {
  ct::Optional<FaultyObject> original(42);

  faulty_run([&] {
    StrongExceptionSafetyGuard guard(original);
    ct::Optional<FaultyObject> copy = original;
    std::ignore = copy;
  });
}

TEST(ExceptionSafetyTest, MoveConstructor) {
  ct::Optional<FaultyObject> original(42);

  faulty_run([&] {
    StrongExceptionSafetyGuard guard(original);
    ct::Optional<FaultyObject> moved(std::move(original));
    std::ignore = moved;
  });
}

TEST(ExceptionSafetyTest, ValueAssignment) {
  ct::Optional<FaultyObject> opt(42);
  FaultyObject value(55);

  faulty_run([&] {
    StrongExceptionSafetyGuard guard(opt, value);
    opt = std::move(value);
  });
}

TEST(ExceptionSafetyTest, CopyAssignment) {
  ct::Optional<FaultyObject> original(42);
  ct::Optional<FaultyObject> target(55);

  faulty_run([&] {
    StrongExceptionSafetyGuard guard(original, target);
    target = original;
  });
}

TEST(ExceptionSafetyTest, MoveAssignment) {
  ct::Optional<FaultyObject> original(42);
  ct::Optional<FaultyObject> target(55);

  faulty_run([&] {
    StrongExceptionSafetyGuard guard(original, target);
    target = std::move(original);
  });
}

TEST(ExceptionSafetyTest, SwapTwoNonEmpty) {
  ct::Optional<FaultyObject> lhs(42);
  ct::Optional<FaultyObject> rhs(55);

  faulty_run([&] {
    StrongExceptionSafetyGuard guard(lhs, rhs);
    swap(lhs, rhs);
  });
}

TEST(ExceptionSafetyTest, SwapNonEmptyAndEmpty) {
  ct::Optional<FaultyObject> lhs(42);
  ct::Optional<FaultyObject> rhs;

  faulty_run([&] {
    StrongExceptionSafetyGuard guard(lhs, rhs);
    swap(lhs, rhs);
  });
}

TEST(ExceptionSafetyTest, SwapEmptyAndNonEmpty) {
  ct::Optional<FaultyObject> lhs;
  ct::Optional<FaultyObject> rhs(55);

  faulty_run([&] {
    StrongExceptionSafetyGuard guard(lhs, rhs);
    swap(lhs, rhs);
  });
}

TEST(ExceptionSafetyTest, EmplaceToEmpty) {
  ct::Optional<FaultyObject> opt;
  FaultyObject value(55);

  faulty_run([&] {
    StrongExceptionSafetyGuard guard(opt, value);
    opt.emplace(std::move(value));
  });
}

TEST(ExceptionSafetyTest, EmplaceToNonEmpty) {
  ct::Optional<FaultyObject> opt(42);
  FaultyObject value(55);

  faulty_run([&] {
    try {
      opt.emplace(std::move(value));
    } catch (...) {
      EXPECT_FALSE(opt.has_value());
      throw;
    }
  });
}

TEST(ExceptionSafetyTest, Reset) {
  ct::Optional<FaultyObject> opt(42);

  assert_nothrow([&] { opt.reset(); });
}

} // namespace ct_test
