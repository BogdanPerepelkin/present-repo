#include "optional.h"

#include <utility>

namespace ct_test {

namespace {

struct ConstexprValue {
  constexpr ConstexprValue()
      : value(0) {}

  constexpr ConstexprValue(int value)
      : value(value) {}

  constexpr ConstexprValue(const ConstexprValue& other)
      : value(other.value) {}

  constexpr ConstexprValue& operator=(const ConstexprValue& other) {
    value = other.value + 1;
    return *this;
  }

  constexpr int get() const {
    return value;
  }

private:
  int value;
};

} // namespace

static_assert(
    [] {
      ct::Optional<ConstexprValue> a;
      return !a.has_value();
    }(),
    "default constructor"
);

static_assert(
    [] {
      struct ConstexprValueWithNonTrivialDestructor : ConstexprValue {
        using ConstexprValue::ConstexprValue;

        constexpr ~ConstexprValueWithNonTrivialDestructor() {}
      };

      ct::Optional<ConstexprValueWithNonTrivialDestructor> a;
      return !a.has_value();
    }(),
    "non-trivial destructor"
);

static_assert(
    [] {
      ct::Optional<ConstexprValue> a(ct::nullopt);
      return !a.has_value();
    }(),
    "nullopt constructor"
);

static_assert(
    [] {
      ct::Optional<ConstexprValue> a(42);
      return (*a).get() == 42;
    }(),
    "value constructor"
);

static_assert(
    [] {
      ct::Optional<ConstexprValue> a(ct::in_place, 42);
      return (*a).get() == 42;
    }(),
    "in_place constructor"
);

static_assert(
    [] {
      ct::Optional<ConstexprValue> a(42);
      return (*std::as_const(a)).get() == 42;
    }(),
    "const indirection"
);

static_assert(
    [] {
      ct::Optional<ConstexprValue> a(42);
      return a->get() == 42;
    }(),
    "member access"
);

static_assert(
    [] {
      ct::Optional<ConstexprValue> a(42);
      return std::as_const(a)->get() == 42;
    }(),
    "const member access"
);

static_assert(
    [] {
      ct::Optional<int> a(42);
      return a == a;
    }(),
    "equals"
);

static_assert(
    [] {
      ct::Optional<int> a(42);
      ct::Optional<int> b(43);
      return a != b;
    }(),
    "not equals"
);

static_assert(
    [] {
      ct::Optional<int> a(42);
      ct::Optional<int> b(43);
      return a < b;
    }(),
    "less"
);

static_assert(
    [] {
      ct::Optional<int> a(42);
      ct::Optional<int> b(43);
      return a <= b;
    }(),
    "less or equals"
);

static_assert(
    [] {
      ct::Optional<int> a(43);
      ct::Optional<int> b(42);
      return a > b;
    }(),
    "greater"
);

static_assert(
    [] {
      ct::Optional<int> a(43);
      ct::Optional<int> b(42);
      return a >= b;
    }(),
    "greater or equals"
);

static_assert(
    [] {
      ct::Optional<int> a(43);
      ct::Optional<int> b(a);
      return a == b;
    }(),
    "copy constructor"
);

static_assert(
    [] {
      ct::Optional<int> a(43);
      ct::Optional<int> b(std::move(a));
      return b && *b == 43;
    }(),
    "move constructro"
);

static_assert(
    [] {
      ct::Optional<int> a(43);
      ct::Optional<int> b(42);
      a = b;
      return a == b;
    }(),
    "copy assignment"
);

static_assert(
    [] {
      ct::Optional<int> a(43);
      ct::Optional<int> b(42);
      a = std::move(b);
      return *a == 42;
    }(),
    "move assignment"
);

static_assert(
    [] {
      ct::Optional<int> a(43);
      ct::Optional<int> b(42);
      a = std::move(b);
      return *a == 42;
    }(),
    "move assignment"
);

static_assert(
    [] {
      ct::Optional<int> a(43);
      ct::Optional<int> b(42);
      swap(a, b);
      return *a == 42 && *b == 43;
    }(),
    "swap"
);

static_assert(
    [] {
      ct::Optional<int> a(43);
      a.reset();
      return !a.has_value();
    }(),
    "reset"
);

static_assert(
    [] {
      ct::Optional<int> a(43);
      a.emplace(42);
      return *a == 42;
    }(),
    "emplace"
);

} // namespace ct_test
