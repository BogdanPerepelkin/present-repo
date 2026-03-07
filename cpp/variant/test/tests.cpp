#include "test-classes.h"
#include "variant.h"

#include <gtest/gtest.h>

#include <compare>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#undef EXPECT_TRUE
#undef EXPECT_FALSE

#define EXPECT_TRUE(...) GTEST_EXPECT_TRUE((__VA_ARGS__))
#define EXPECT_FALSE(...) GTEST_EXPECT_FALSE((__VA_ARGS__))

namespace ct_test {

TEST(traits, destructor) {
  using V1 = ct::Variant<int, double, Trivial>;
  using V2 = ct::Variant<int, std::string>;
  using V3 = ct::Variant<char, long, V1>;
  using V4 = ct::Variant<char, V2, int>;
  EXPECT_TRUE(std::is_trivially_destructible_v<V1>);
  EXPECT_FALSE(std::is_trivially_destructible_v<V2>);
  EXPECT_TRUE(std::is_trivially_destructible_v<V3>);
  EXPECT_FALSE(std::is_trivially_destructible_v<V4>);
}

TEST(traits, default_constructor) {
  using V1 = ct::Variant<std::string, int, std::vector<int>>;
  using V2 = ct::Variant<NoDefaultConstructor, int>;
  using V3 = ct::Variant<int, NoDefaultConstructor>;
  using V4 = ct::Variant<ThrowingDefaultConstructor, int, double>;
  using V5 = ct::Variant<int, double, ThrowingDefaultConstructor>;
  EXPECT_TRUE(std::is_default_constructible_v<V1>);
  EXPECT_FALSE(std::is_default_constructible_v<V2>);
  EXPECT_TRUE(std::is_default_constructible_v<V3>);
  EXPECT_TRUE(std::is_default_constructible_v<V4>);
  EXPECT_TRUE(std::is_nothrow_default_constructible_v<V1>);
  EXPECT_FALSE(std::is_nothrow_default_constructible_v<V4>);
  EXPECT_TRUE(std::is_nothrow_default_constructible_v<V5>);
}

TEST(traits, copy_constructor) {
  using V1 = ct::Variant<int, NoCopy, std::vector<std::string>>;
  using V2 = ct::Variant<std::string, std::vector<std::string>, int>;
  using V3 = ct::Variant<int, double, Trivial>;
  using V4 = ct::Variant<double, int, NonTrivialCopy>;
  EXPECT_FALSE(std::is_copy_constructible_v<V1>);
  EXPECT_TRUE(std::is_copy_constructible_v<V2>);
  EXPECT_FALSE(std::is_trivially_copy_constructible_v<V2>);
  static_assert(all_trivially_copy_constructible<int, double, Trivial>);
  EXPECT_TRUE(std::is_trivially_copy_constructible_v<V3>);
  EXPECT_FALSE(std::is_trivially_copy_constructible_v<V4>);
}

TEST(traits, move_constructor) {
  using V1 = ct::Variant<int, std::string, NoMove, double>;
  using V2 = ct::Variant<double, std::string, int>;
  using V3 = ct::Variant<int, Trivial, char>;
  using V4 = ct::Variant<int, double, ThrowingMoveAssignmentWithoutMoveConstructor>;
  EXPECT_FALSE(std::is_move_constructible_v<V1>);
  EXPECT_TRUE(std::is_move_constructible_v<V2>);
  EXPECT_TRUE(std::is_move_constructible_v<V3>);
  EXPECT_TRUE(std::is_nothrow_move_constructible_v<V2>);
  EXPECT_FALSE(std::is_trivially_move_constructible_v<V2>);
  EXPECT_TRUE(std::is_trivially_move_constructible_v<V3>);
  EXPECT_TRUE(std::is_move_constructible_v<V4>);
  EXPECT_FALSE(std::is_nothrow_move_constructible_v<V4>);
}

TEST(traits, converting_constructor) {
  using V1 = ct::Variant<std::string, std::vector<double>>;
  EXPECT_FALSE(std::is_constructible_v<V1, std::size_t>);
  EXPECT_TRUE(std::is_constructible_v<V1, const char*>);
  EXPECT_TRUE(std::is_nothrow_constructible_v<V1, std::string&&>);
  EXPECT_FALSE(std::is_nothrow_constructible_v<V1, const char*>);
}

TEST(traits, in_place_type) {
  using V1 = ct::Variant<int, float, std::string, Trivial, std::vector<int>, NoDefaultConstructor>;
  EXPECT_FALSE(std::is_constructible_v<V1, ct::InPlaceType<ThrowingMoveAssignmentWithoutMoveConstructor>>);
  EXPECT_TRUE(std::is_constructible_v<V1, ct::InPlaceType<Trivial>>);
  EXPECT_FALSE(std::is_constructible_v<V1, ct::InPlaceType<NoDefaultConstructor>>);
  EXPECT_TRUE(std::is_constructible_v<V1, ct::InPlaceType<std::vector<int>>, size_t, int>);
  EXPECT_TRUE(std::is_constructible_v<V1, ct::InPlaceType<std::vector<int>>, size_t>);
  EXPECT_TRUE(std::is_constructible_v<V1, ct::InPlaceType<std::string>>);
}

TEST(traits, in_place_index) {
  using V1 = ct::Variant<int, float, std::string, Trivial, std::vector<int>, NoDefaultConstructor>;
  EXPECT_FALSE(std::is_constructible_v<V1, ct::InPlaceIndex<1337>>);
  EXPECT_TRUE(std::is_constructible_v<V1, ct::InPlaceIndex<3>>);
  EXPECT_FALSE(std::is_constructible_v<V1, ct::InPlaceIndex<5>>);
  EXPECT_TRUE(std::is_constructible_v<V1, ct::InPlaceIndex<4>, size_t, int>);
  EXPECT_TRUE(std::is_constructible_v<V1, ct::InPlaceIndex<4>, size_t>);
  EXPECT_TRUE(std::is_constructible_v<V1, ct::InPlaceIndex<3>>);
}

TEST(traits, copy_assignment) {
  using V1 = ct::Variant<std::string, double, NoCopy>;
  using V2 = ct::Variant<std::vector<short>, int, NoCopyAssignment>;
  using V3 = ct::Variant<Trivial, int, NonTrivialCopyAssignment>;
  using V4 = ct::Variant<double, NonTrivialCopy, bool>;
  using V5 = ct::Variant<int, short, char, Trivial, bool>;
  using V6 = ct::Variant<int, double, NoCopy>;
  EXPECT_FALSE(std::is_copy_assignable_v<V1>);
  EXPECT_FALSE(std::is_copy_assignable_v<V2>);
  EXPECT_TRUE(std::is_copy_assignable_v<V3>);
  EXPECT_TRUE(std::is_copy_assignable_v<V4>);
  EXPECT_TRUE(std::is_copy_assignable_v<V5>);
  EXPECT_FALSE(std::is_trivially_copy_assignable_v<V3>);
  EXPECT_FALSE(std::is_trivially_copy_assignable_v<V4>);
  EXPECT_TRUE(std::is_trivially_copy_assignable_v<V5>);
  EXPECT_FALSE(std::is_copy_assignable_v<V6>);
}

TEST(traits, move_assignment) {
  using V1 = ct::Variant<std::string, double, NoMove>;
  using V2 = ct::Variant<int, std::vector<std::string>, NoMoveAssignment, bool>;
  using V3 = ct::Variant<Trivial, int, std::vector<double>>;
  using V4 = ct::Variant<double, std::string, bool>;
  using V5 = ct::Variant<int, short, char, Trivial, bool>;
  using V6 = ct::Variant<int, std::string, ThrowingMoveAssignmentWithoutMoveConstructor, double>;
  using V7 = ct::Variant<int, ThrowingMoveAssignment, double>;
  using V8 = ct::Variant<int, double, NoMove>;
  using V9 = ct::Variant<NonTrivialCopyWithTrivialMove>;
  EXPECT_FALSE(std::is_move_assignable_v<V1>);
  EXPECT_FALSE(std::is_move_assignable_v<V2>);
  EXPECT_TRUE(std::is_move_assignable_v<V3>);
  EXPECT_TRUE(std::is_move_assignable_v<V4>);
  EXPECT_TRUE(std::is_move_assignable_v<V5>);
  EXPECT_FALSE(std::is_trivially_move_assignable_v<V3>);
  EXPECT_FALSE(std::is_trivially_move_assignable_v<V4>);
  EXPECT_TRUE(std::is_trivially_move_assignable_v<V5>);
  EXPECT_TRUE(std::is_move_assignable_v<V6>);
  EXPECT_TRUE(std::is_move_assignable_v<V7>);
  EXPECT_FALSE(std::is_nothrow_move_assignable_v<V6>);
  EXPECT_FALSE(std::is_nothrow_move_assignable_v<V7>);
  EXPECT_TRUE(std::is_nothrow_move_assignable_v<V3>);
  EXPECT_TRUE(std::is_nothrow_move_assignable_v<V4>);
  EXPECT_TRUE(std::is_nothrow_move_assignable_v<V5>);
  EXPECT_FALSE(std::is_move_assignable_v<V8>);
  static_assert(std::is_move_assignable_v<V9>);

  // static_assert(
  //     std::is_nothrow_move_constructible_v<NonTrivialCopyWithTrivialMove> &&
  //     std::is_nothrow_move_assignable_v<NonTrivialCopyWithTrivialMove>
  // );
  //
  // static_assert(
  //     std::is_nothrow_move_constructible_v<ct::UniStorageT<NonTrivialCopyWithTrivialMove>> &&
  //     std::is_nothrow_move_assignable_v<NonTrivialCopyWithTrivialMove>
  // );

  static_assert(std::is_trivially_move_assignable_v<V9>);
  static_assert(std::is_trivially_move_assignable_v<NonTrivialCopyWithTrivialMove>);
  EXPECT_TRUE(std::is_trivially_move_assignable_v<V9>);
}

void func(const char*&& t) {
  new std::string[1]{std::forward<const char*>(t)};
  // new bool[1]{std::forward<const char*>(t)};
}

TEST(traits, converting_assignment) {
  using V1 = ct::Variant<std::string, std::vector<char>, bool>;
  EXPECT_TRUE(std::is_assignable_v<V1&, std::string&&>);

  EXPECT_TRUE(std::is_assignable_v<V1&, const char*>);
  EXPECT_FALSE(std::is_assignable_v<V1&, size_t>);
  EXPECT_TRUE(std::is_nothrow_assignable_v<V1&, std::string&&>);
  EXPECT_FALSE(std::is_nothrow_assignable_v<V1&, const std::string&>);
  EXPECT_FALSE(std::is_assignable_v<V1&, double*>);
}

TEST(traits, swap) {
  using VariantWithNothrowSwap = ct::Variant<int, bool>;
  using VariantWithThrowingSwap = ct::Variant<ThrowingSwap, long>;
  using VariantNonMoveAssignable = ct::Variant<MoveConstructorWithoutMoveAssignment, int>;

  EXPECT_TRUE(std::is_nothrow_swappable_v<VariantWithNothrowSwap>);
  EXPECT_TRUE(std::is_nothrow_swappable_v<VariantNonMoveAssignable>);
  EXPECT_FALSE(std::is_nothrow_swappable_v<VariantWithThrowingSwap>);
}

TEST(traits, variant_size) {
  using V1 = ct::Variant<int, std::string, ct::Variant<int, std::vector<int>, size_t>, bool>;
  EXPECT_EQ(ct::variant_size<V1>, 4);
  EXPECT_EQ(ct::variant_size<V1>, ct::variant_size<const V1>);
}

TEST(traits, variant_alternative) {
  using V1 = ct::Variant<int, std::string, ct::Variant<int, std::vector<int>, size_t>, bool>;
  using T1 = ct::variant_alternative<1, V1>;
  using T2 = ct::variant_alternative<1, const V1>;
  EXPECT_TRUE(std::is_same_v<T1, std::string>);
  EXPECT_TRUE(std::is_same_v<const T1, T2>);
}

static_assert(ct::Variant<int>().index() == 0, "Constexpr empty ctor failed");
static_assert(ct::holds_alternative<int>(ct::Variant<int, double>()), "Constexpr empty ctor holds_alternative failed");
static_assert(ct::holds_alternative<int>(ct::Variant<int>()), "Constexpr empty ctor holds_alternative failed");
static_assert(holds_alternative<int>(ct::Variant<int>()), "Constexpr empty ctor holds_alternative ADL failed");
static_assert(ct::Variant<int, double>().index() == 0, "Constexpr empty ctor failed");

TEST(correctness, empty_ctor) {
  ct::Variant<int, double> v;
  ASSERT_TRUE(v.index() == 0);
  ASSERT_TRUE(holds_alternative<int>(v));
}

TEST(correctness, converting_ctor) {
  ct::Variant<std::string, long, std::string, char, std::string, int, std::string> v(123);
  ASSERT_TRUE(v.index() == 5);
  ASSERT_TRUE(holds_alternative<int>(v));
  ASSERT_TRUE(get<int>(v) == 123);
}

TEST(correctness, const_types_copy) {
  ct::Variant<int, const NonTrivialCopy> v1(ct::in_place_index<1>, 0);
  // std::cout << get<1>(v1).x << std::endl;
  ct::Variant<int, const NonTrivialCopy> v2(v1);
  ASSERT_TRUE(v2.index() == 1);
  ASSERT_TRUE(holds_alternative<const NonTrivialCopy>(v2));
  // std::cout << get<1>(v2).x << std::endl;
  ASSERT_TRUE(get<1>(v2).x == 1);
}

TEST(correctness, const_types_move) {
  struct NonTrivialMoveConstructor {
    explicit NonTrivialMoveConstructor(int x)
        : x(x) {}

    NonTrivialMoveConstructor(const NonTrivialMoveConstructor&& other)
        : x(other.x + 1) {}

    int x;
  };

  ct::Variant<int, const NonTrivialMoveConstructor> v1(ct::in_place_index<1>, 0);
  ct::Variant<int, const NonTrivialMoveConstructor> v2(std::move(v1));
  ASSERT_TRUE(v2.index() == 1);
  ASSERT_TRUE(holds_alternative<const NonTrivialMoveConstructor>(v2));
  ASSERT_TRUE(get<1>(v2).x == 1);
}

static constexpr bool simple_copy_ctor_test() {
  ct::Variant<int, double> x{42.0};
  ct::Variant<int, double> other{x};
  if (x.index() != other.index()) {
    return false;
  }
  if (get<1>(x) != get<1>(other)) {
    return false;
  }
  if (!holds_alternative<double>(x) || !holds_alternative<double>(other)) {
    return false;
  }
  return true;
}

static_assert(simple_copy_ctor_test(), "Basic constexpr copy-constructor failed");

TEST(correctness, copy_ctor1) {
  ASSERT_TRUE(simple_copy_ctor_test());
}

TEST(correctness, copy_constructor_without_default) {
  ct::Variant<NoDefaultConstructor, NonTrivialCopy> orig(ct::in_place_index<1>, 123);
  ct::Variant<NoDefaultConstructor, NonTrivialCopy> copy(orig);
  ASSERT_EQ(orig.index(), copy.index());
  ASSERT_EQ(get<1>(orig).x + 1, get<NonTrivialCopy>(copy).x);
}

static constexpr bool direct_init_copy_ctor() {
  ct::Variant<NoCopyAssignment> x;
  ct::Variant<NoCopyAssignment> other{x};
  if (!holds_alternative<NoCopyAssignment>(x) || !holds_alternative<NoCopyAssignment>(other)) {
    return false;
  }
  return true;
}

TEST(correctness, copy_ctor2) {
  ASSERT_TRUE(direct_init_copy_ctor());
}

TEST(correctness, destructor) {
  { ct::Variant<NonTrivialDestructor> v; }
  ASSERT_EQ(NonTrivialDestructor::destructor_count, 1);
}

static constexpr bool simple_move_ctor_test() {
  {
    ct::Variant<NoCopyAssignment> x;
    ct::Variant<NoCopyAssignment> other{std::move(x)};
    if (!holds_alternative<NoCopyAssignment>(x) || !holds_alternative<NoCopyAssignment>(other)) {
      return false;
    }
  }
  {
    ct::Variant<int, double> x{42};
    ct::Variant<int, double> y = std::move(x);
    if (x.index() != y.index() || x.index() != 0 || get<0>(x) != get<0>(y)) {
      return false;
    }
  }
  return true;
}

static_assert(simple_move_ctor_test(), "Simple constexpr move test failed");

TEST(correctness, move_ctor) {
  simple_move_ctor_test();

  ct::Variant<CoinWrapper> x;
  ct::Variant<CoinWrapper> y = std::move(x);
  ASSERT_TRUE(!get<0>(x).has_coins());
  ASSERT_TRUE(get<0>(y).has_coins() == 1);
}

static constexpr bool simple_value_move_ctor() {
  {
    OnlyMovable x;
    ct::Variant<OnlyMovable> y(std::move(x));
    if (x.has_coin() || !get<0>(y).has_coin()) {
      return false;
    }
  }
  {
    CoinWrapper x;
    ct::Variant<CoinWrapper> y(std::move(x));
    if (x.has_coins() || !get<0>(y).has_coins()) {
      return false;
    }
  }
  return true;
}

static_assert(simple_value_move_ctor(), "Simple value-forwarding ctor failed");

TEST(correctness, value_move_ctor) {
  simple_value_move_ctor();
  ct::Variant<int, CoinWrapper> x(Coin{});
  ASSERT_TRUE(x.index() == 0);
}

void func1(decltype(u'\u2043')&& t) {
  std::optional<char16_t> x[] = {std::forward<decltype(u'\u2043')>(t)};
}

// void func2(decltype(u'\u2043')&& t) {
//   char x[] = {std::forward<decltype(u'\u2043')>(t)};
// }

TEST(correctness, alternative_selection) {
  {
    // std::cout << ct::utils::ChooseOverload<decltype(u'\u2043'), char, std::optional<char16_t>>::index;
    // static_assert(std::is_constructible_v<ct::Type<char, std::optional<char16_t>>, decltype(u'\u2043')>);
    ct::Variant<char, std::optional<char16_t>> v = u'\u2043';
    ASSERT_EQ(v.index(), 1);
  }
  {
    double d = 3.14;
    ct::Variant<int, std::reference_wrapper<double>> v = d;
    ASSERT_EQ(v.index(), 1);
  }
  // For the brave and true
  {
    // See NB in #4 https://en.cppreference.com/w/cpp/utility/variant/variant
    ct::Variant<bool, std::string> v("asdasd");
    ASSERT_EQ(v.index(), 1); // Overload resolution is not your friend anymore
  }
  {
    ct::Variant<long, double, float> v = 0;
    ASSERT_EQ(v.index(), 0);
  }
  {
    ct::Variant<std::vector<int>, bool, std::string> v(true);
    ASSERT_EQ(v.index(), 1);
  }
  {
    struct ToStruct {};

    struct FromStruct {
      constexpr operator ToStruct() const noexcept {
        return {};
      }

      explicit constexpr operator double() const noexcept {
        return 3.14;
      }
    };

    ct::Variant<double, ToStruct> v(FromStruct{});
    ASSERT_EQ(v.index(), 1);
  }
}

// TODO: refactor this test
TEST(correctness, valueless_by_exception) {
  using V = ct::Variant<std::vector<int>, ThrowingMoveAssignmentWithoutMoveConstructor>;
  auto v1 = std::vector{1, 2, 3};
  V v = v1;
  ASSERT_ANY_THROW({
    V tmp(ct::in_place_index<1>);
    v = std::move(tmp);
  });
  ASSERT_TRUE(v.valueless_by_exception());
  auto v2 = std::vector{4, 5, 6};
  V w = v2;
  ASSERT_FALSE(w.valueless_by_exception());
  ASSERT_EQ(get<std::vector<int>>(w), v2);
  // std::cout << w.index() << std::endl;
  // std::cout << v.index() << std::endl;
  w.swap(v);
  ASSERT_TRUE(w.valueless_by_exception());
  ASSERT_FALSE(v.valueless_by_exception());
  ASSERT_EQ(get<0>(v), v2);
  w.swap(v);
  ASSERT_TRUE(v.valueless_by_exception());
  ASSERT_FALSE(w.valueless_by_exception());
  ASSERT_EQ(get<0>(w), v2);
}

TEST(correctness, assign) {
  struct BruhConversion {
    BruhConversion(int) {
      throw std::exception();
    }
  };

  std::string s = "here comes some std::string";
  ct::Variant<std::string, BruhConversion> v = s;
  ASSERT_ANY_THROW(v = 42);
  ASSERT_EQ(get<0>(v), s);
}

TEST(correctness, visit) {
  using V = ct::Variant<int, long, double>;
  V v1 = 42;
  V v2 = 1337L;
  V v3 = 0.5;
  bool was_called = false;
  ct::visit(
      [&](int i, long l, double d) {
        ASSERT_EQ(i, 42);
        ASSERT_EQ(l, 1337L);
        ASSERT_EQ(d, 0.5);
        was_called = true;
      },
      v1,
      v2,
      v3
  );
  ASSERT_TRUE(was_called);
}

TEST(correctness, visit_adl) {
  using V = ct::Variant<int, long, double>;
  V v1 = 42;
  V v2 = 1337L;
  V v3 = 0.5;
  bool was_called = false;
  visit(
      [&](int i, long l, double d) {
        ASSERT_EQ(i, 42);
        ASSERT_EQ(l, 1337L);
        ASSERT_EQ(d, 0.5);
        was_called = true;
      },
      v1,
      v2,
      v3
  );
  ASSERT_TRUE(was_called);
}

TEST(correctness, get) {
  ct::Variant<std::string, int, float> v(ct::in_place_index<1>, 42);

  ASSERT_EQ(ct::get<1>(v), 42);
  ASSERT_EQ(ct::get<1>(std::as_const(v)), 42);
  ASSERT_EQ(ct::get<1>(std::move(v)), 42);
  ASSERT_EQ(ct::get<1>(std::move(std::as_const(v))), 42);

  ASSERT_EQ(ct::get<int>(v), 42);
  ASSERT_EQ(ct::get<int>(std::as_const(v)), 42);
  ASSERT_EQ(ct::get<int>(std::move(v)), 42);
  ASSERT_EQ(ct::get<int>(std::move(std::as_const(v))), 42);
}

TEST(correctness, get_adl) {
  ct::Variant<std::string, int, float> v(ct::in_place_index<1>, 42);

  ASSERT_EQ(get<1>(v), 42);
  ASSERT_EQ(get<1>(std::as_const(v)), 42);
  ASSERT_EQ(get<1>(std::move(v)), 42);
  ASSERT_EQ(get<1>(std::move(std::as_const(v))), 42);

  ASSERT_EQ(get<int>(v), 42);
  ASSERT_EQ(get<int>(std::as_const(v)), 42);
  ASSERT_EQ(get<int>(std::move(v)), 42);
  ASSERT_EQ(get<int>(std::move(std::as_const(v))), 42);
}

TEST(correctness, emplace) {
  using V = ct::Variant<std::vector<int>, std::string>;
  std::string s = "A fairly long string that will cause an allocation";
  std::vector<int> t = {1, 2, 3};
  V v = s;
  ASSERT_EQ(v.index(), 1);
  v.emplace<0>(t);
  ASSERT_EQ(v.index(), 0);
  ASSERT_EQ(get<0>(v), t);
  v.emplace<std::string>(s);
  ASSERT_EQ(v.index(), 1);
  ASSERT_EQ(get<1>(v), s);
  v.emplace<0>(t);
  ASSERT_EQ(v.index(), 0);
  ASSERT_EQ(get<0>(v), t);
}

TEST(correctness, emplace_conversions) {
  using V = ct::Variant<int, std::string>;
  V v;
  ASSERT_EQ(get<0>(v), 0);
  v.emplace<1>(3.14, static_cast<int>('a'));
  ASSERT_EQ(get<1>(v), "aaa");
}

static constexpr bool in_place_ctor() {
  ct::Variant<bool, double> x1(ct::in_place_type<double>, 42);
  ct::Variant<bool, double> x2(ct::in_place_index<1>, 42);
  return (x1.index() == 1 && get<1>(x1) == 42.0) && (x2.index() == 1 && get<1>(x2) == 42.0);
}

static_assert(in_place_ctor(), "Simple in-place ctor failed");

TEST(correctness, inplace_ctors) {
  in_place_ctor();

  ct::Variant<bool, std::string> why_not(ct::in_place_type<bool>, "asdasd");
  ASSERT_TRUE(why_not.index() == 0);
  ASSERT_TRUE(get<0>(why_not));

  ct::Variant<bool, std::string> x2(ct::in_place_index<0>, "asdasd");
  ASSERT_TRUE(x2.index() == 0);
  ASSERT_TRUE(get<0>(x2));

  ct::Variant<std::string, std::vector<int>, char> var{ct::in_place_index<1>, std::vector<int>{1, 2, 3, 4, 5}};
  auto other = std::vector<int>{1, 2, 3, 4, 5};
  ASSERT_EQ(get<1>(var), other);
  auto other2 = std::vector<int>(4, 42);
  ct::Variant<std::string, std::vector<int>, char> var2{ct::in_place_index<1>, 4, 42};
  ASSERT_EQ(get<1>(var2), other2);
}

TEST(correctness, variant_exceptions1) {
  using T = ThrowingMoveAssignmentWithoutMoveConstructor;
  ct::Variant<T> x;
  try {
    x.emplace<T>(T{});
  } catch (const std::exception&) {
    ASSERT_TRUE(x.valueless_by_exception());
    ASSERT_EQ(x.index(), ct::variant_npos);
    ASSERT_THROW(get<0>(x), ct::BadVariantAccess);
    ASSERT_THROW(get<0>(x), std::exception);
    return;
  }
  FAIL();
}

static constexpr bool get_if_test_basic() {
  ct::Variant<float, double, long double> v = 4.5;
  // std::cout << v.index() << std::endl;
  if (double* ptr = ct::get_if<double>(&v); ptr == nullptr) {
    return false;
  }
  if (const double* ptr = ct::get_if<double>(&std::as_const(v)); ptr == nullptr) {
    return false;
  }
  if (double* ptr = ct::get_if<1>(&v); ptr == nullptr) {
    return false;
  }
  if (const double* ptr = ct::get_if<1>(&std::as_const(v)); ptr == nullptr) {
    return false;
  }
  return true;
}

static_assert(get_if_test_basic(), "Bad get_if behavior");

// [my]
// static_assert(get<0>(ct::Type<int>(0)) == 0);

// TODO ================================================================================

TEST(correctness, multiple_same_types) {
  // [my]
  // std::cout << get_if_test_basic() << std::endl;

  ct::Variant<int, const int, const int, const volatile int> v;
  v.emplace<int>(4);
  ASSERT_TRUE(holds_alternative<int>(v));
  ASSERT_TRUE(v.index() == 0);
  ASSERT_TRUE(get_if<int>(&v));
  ASSERT_TRUE(get_if<0>(&v));
  ASSERT_TRUE(get<int>(v) == 4);
  ASSERT_TRUE(get<0>(v) == 4);

  v.emplace<1>(4);
  ASSERT_TRUE(v.index() == 1);
  ASSERT_TRUE(get_if<1>(&v));
  ASSERT_TRUE(get<1>(v) == 4);

  v.emplace<2>(4);
  ASSERT_TRUE(v.index() == 2);
  ASSERT_TRUE(get_if<2>(&v));
  ASSERT_TRUE(get<2>(v) == 4);

  ASSERT_THROW(get<1>(v), ct::BadVariantAccess);
}

TEST(correctness, overloaded_address) {
  using V = ct::Variant<int, OverloadedAddressOf, std::string>;
  {
    V v(ct::in_place_index<1>);
    OverloadedAddressOf* ptr = get_if<1>(&v);
    ASSERT_TRUE(ptr != nullptr);
    ASSERT_TRUE(&*ptr == nullptr);

    v = 42;
    ptr = get_if<1>(&v);
    ASSERT_TRUE(ptr == nullptr);

    v = OverloadedAddressOf();
    ptr = get_if<1>(&v);
    ASSERT_TRUE(ptr != nullptr);
    ASSERT_TRUE(&*ptr == nullptr);
  }
  {
    V v = OverloadedAddressOf();
    V w(v);
    OverloadedAddressOf* ptr = get_if<1>(&w);
    ASSERT_TRUE(ptr != nullptr);
    ASSERT_TRUE(&*ptr == nullptr);

    w = 42;
    w = v;
    ptr = get_if<1>(&w);
    ASSERT_TRUE(ptr != nullptr);
    ASSERT_TRUE(&*ptr == nullptr);
  }
  {
    V v(ct::in_place_type<OverloadedAddressOf>);
    auto w = V(ct::in_place_index<1>);
    OverloadedAddressOf* ptr = get_if<1>(&w);
    ASSERT_TRUE(ptr != nullptr);
    ASSERT_TRUE(&*ptr == nullptr);

    w = 42;
    w = std::move(v);
    ptr = get_if<1>(&w);
    ASSERT_TRUE(ptr != nullptr);
    ASSERT_TRUE(&*ptr == nullptr);
  }
  {
    V v = 42;
    v.emplace<OverloadedAddressOf>(OverloadedAddressOf());
    OverloadedAddressOf* ptr = get_if<1>(&v);
    ASSERT_TRUE(ptr != nullptr);
    ASSERT_TRUE(&*ptr == nullptr);
  }
}

TEST(visits, visit_valueless) {
  using T = ThrowingMoveAssignmentWithoutMoveConstructor;
  ct::Variant<T> x;
  try {
    x.emplace<T>(T{});
  } catch (const std::exception&) {
    ASSERT_TRUE(x.valueless_by_exception());
    auto visitor = [](auto&&) {
    };
    ASSERT_THROW(ct::visit(visitor, x), ct::BadVariantAccess);
    ASSERT_THROW(ct::visit(visitor, x), std::exception);
    return;
  }

  assert(false && "Exception expected");
}

TEST(visits, visit_on_multiple) {
  ct::Variant<int, const int, const int, double> v;
  v.emplace<2>(42);
  auto visitor = [](auto x) -> int {
    return x;
  };
  auto result = ct::visit(visitor, v);
  ASSERT_EQ(result, 42);

  auto visitor2 = [](int x) -> int {
    return x;
  };
  result = ct::visit(visitor2, v);
  ASSERT_EQ(result, 42);

  auto visitor3 = [](const double x) -> int {
    return static_cast<int>(x);
  };
  result = ct::visit(visitor3, v);
  ASSERT_EQ(result, 42);
}

TEST(visits, visit_overload) {
  ct::Variant<const char*> v = "abce";
  auto visitor = Overload{
      [](const std::string&) -> bool { return false; },
      [](bool) -> bool { return true; },
  };
  ASSERT_TRUE(ct::visit(visitor, v));
}

TEST(visits, visit_overload_different_types) {
  ct::Variant<int, double, bool> v = 3.14;
  auto visitor = [](auto x) {
    return x;
  };
  ASSERT_FLOAT_EQ(ct::visit<float>(visitor, v), 3.14F);
}

TEST(visits, visit_derived) {
  using V = ct::Variant<int, double, long>;

  struct DerivedFromVariant : V {
    using V::V;
  };

  DerivedFromVariant v;
  v.emplace<2>(42);

  auto visitor = [](auto x) -> int {
    return x;
  };

  auto result = ct::visit(visitor, v);
  ASSERT_EQ(result, 42);
}

static constexpr bool test_visit() {
  using V = ct::Variant<int, short, long>;
  V a1(1);
  V b1(2);
  V c1(3);
  auto res1 = ct::visit(SumOfSquaresVisitor{}, a1, b1, c1);

  V a2(ct::in_place_index<0>, 2);
  V b2(ct::in_place_index<1>, 2);
  V c2(ct::in_place_index<2>, 2);
  auto res2 = ct::visit(SumOfSquaresVisitor{}, a2, b2, c2);

  return (res1 == 14) && (res2 == 12);
}

static_assert(test_visit(), "Visit is not constexpr");

TEST(visits, visit_visitor_forwarding) {
  ct::Variant<int> var = 322;
  StrangeVisitor vis;
  int val1 = ct::visit(vis, var);
  ASSERT_EQ(val1, 322);
  int val2 = ct::visit(StrangeVisitor(), var);
  ASSERT_EQ(val2, 323);
  int val3 = ct::visit(std::as_const(vis), var);
  ASSERT_EQ(val3, 324);
  int val4 = ct::visit(std::move(std::as_const(vis)), var);
  ASSERT_EQ(val4, 325);
}

TEST(visits, visit_args_forwarding) {
  ct::Variant<OnlyMovable> var;
  int val1 = ct::visit([](const OnlyMovable&) { return 322; }, var);
  ASSERT_EQ(val1, 322);
  int val2 = ct::visit([](OnlyMovable&) { return 322; }, var);
  ASSERT_EQ(val2, 322);
  int val3 = ct::visit([](const OnlyMovable&&) { return 322; }, std::move(std::as_const(var)));
  ASSERT_EQ(val3, 322);
  int val4 = ct::visit([](OnlyMovable&&) { return 322; }, std::move(var));
  ASSERT_EQ(val4, 322);
}

TEST(visits, visit_result_forwarding) {
  ct::Variant<int> var;
  int x = 42;
  EXPECT_TRUE(std::is_same_v<decltype(ct::visit([&](auto) -> int { return x; }, var)), int>);
  EXPECT_TRUE(std::is_same_v<decltype(ct::visit([&](auto) -> int& { return x; }, var)), int&>);
  EXPECT_TRUE(std::is_same_v<decltype(ct::visit([&](auto) -> const int& { return x; }, var)), const int&>);
  EXPECT_TRUE(std::is_same_v<decltype(ct::visit([&](auto) -> int&& { return std::move(x); }, var)), int&&>);
  EXPECT_TRUE(std::is_same_v<decltype(ct::visit([&](auto) -> const int&& { return std::move(x); }, var)), const int&&>);
}

TEST(swap, both_valueless) {
  using T = ThrowingMoveAssignmentWithoutMoveConstructor;
  T::swap_called = 0;
  using V = ct::Variant<int, T>;
  V a = 14;
  V b = 88;
  ASSERT_ANY_THROW({
    V tmp(ct::in_place_index<1>);
    a = std::move(tmp);
  });
  ASSERT_ANY_THROW({
    V tmp(ct::in_place_index<1>);
    b = std::move(tmp);
  });
  ASSERT_TRUE(a.valueless_by_exception());
  ASSERT_TRUE(b.valueless_by_exception());
  a.swap(b);
  ASSERT_TRUE(a.valueless_by_exception());
  ASSERT_TRUE(b.valueless_by_exception());
  ASSERT_EQ(T::swap_called, 0);
}

TEST(swap, one_valueless) {
  using V = ct::Variant<NonTrivialDestructor, ThrowingMoveAssignmentWithoutMoveConstructor>;
  V a;
  V b;
  ASSERT_ANY_THROW({
    V tmp(ct::in_place_index<1>);
    a = std::move(tmp);
  });
  ASSERT_TRUE(a.valueless_by_exception());

  NonTrivialDestructor::destructor_count = 0;
  ThrowingMoveAssignmentWithoutMoveConstructor::swap_called = 0;
  a.swap(b);
  ASSERT_FALSE(a.valueless_by_exception());
  ASSERT_TRUE(b.valueless_by_exception());
  ASSERT_NE(NonTrivialDestructor::destructor_count, 0);
  ASSERT_EQ(ThrowingMoveAssignmentWithoutMoveConstructor::swap_called, 0);

  NonTrivialDestructor::destructor_count = 0;
  ThrowingMoveAssignmentWithoutMoveConstructor::swap_called = 0;
  a.swap(b);
  ASSERT_TRUE(a.valueless_by_exception());
  ASSERT_FALSE(b.valueless_by_exception());
  ASSERT_NE(NonTrivialDestructor::destructor_count, 0);
  ASSERT_EQ(ThrowingMoveAssignmentWithoutMoveConstructor::swap_called, 0);
}

TEST(swap, same_alternative) {
  using T = ThrowingMoveAssignmentWithoutMoveConstructor;
  T::swap_called = 0;
  using V = ct::Variant<int, T>;
  V a(ct::in_place_index<1>);
  V b(ct::in_place_index<1>);
  a.swap(b);
  ASSERT_EQ(T::swap_called, 1);
}

TEST(swap, different_alternatives) {
  using V = ct::Variant<int, std::string, Trivial>;
  V a(42);
  V b("kek");
  V c(ct::in_place_index<2>);
  a.swap(b);
  b.swap(c);
  ASSERT_TRUE(holds_alternative<std::string>(a));
  ASSERT_TRUE(holds_alternative<Trivial>(b));
  ASSERT_TRUE(holds_alternative<int>(c));
  ASSERT_EQ(get<std::string>(a), "kek");
  ASSERT_EQ(get<int>(c), 42);
}

TEST(swap, swap_no_move_assigment) {
  using T = MoveConstructorWithoutMoveAssignment;
  using V = ct::Variant<T, long>;

  V v1 = 10L;
  V v2 = T(5);
  V v3 = T(6);
  // std::cout << "[look]" << std::endl;
  v1.swap(v2);
  ASSERT_TRUE(holds_alternative<T>(v1));
  ASSERT_TRUE(holds_alternative<long>(v2));
  ASSERT_EQ(get<T>(v1).x, 5);
  ASSERT_EQ(get<long>(v2), 10);
  v1.swap(v3);
  ASSERT_EQ(get<T>(v1).x, 6);
  ASSERT_EQ(get<T>(v3).x, 5);
}

TEST(assignment, same_alternative) {
  using V = ct::Variant<NonTrivialIntWrapper, NonTrivialCopyAssignment>;
  V a(ct::in_place_type<NonTrivialCopyAssignment>, 42);
  V b(ct::in_place_type<NonTrivialCopyAssignment>, 14882);
  a = b;
  ASSERT_EQ(get<1>(a).x, 14882 + NonTrivialCopyAssignment::ASSIGN_DELTA);
}

TEST(assignment, back_and_forth) {
  using V = ct::Variant<NonTrivialIntWrapper, NonTrivialCopyAssignment>;
  constexpr auto CTOR_DELTA = NonTrivialCopyAssignment::CTOR_DELTA;
  constexpr auto ASSIGN_DELTA = NonTrivialCopyAssignment::ASSIGN_DELTA;

  V a = NonTrivialIntWrapper(42);
  V b = NonTrivialCopyAssignment(14882);
  ASSERT_EQ(get<0>(a).x, 42);
  ASSERT_EQ(get<1>(b).x, 14882 + CTOR_DELTA);
  // std::cout << "[look]" << std::endl;
  // std::cout << a.index() << " = a.index" << std::endl;
  a = 42;
  ASSERT_EQ(get<0>(a).x, 43);
  a = NonTrivialCopyAssignment(42);
  ASSERT_EQ(get<1>(a).x, 42 + CTOR_DELTA);
  b = a;
  ASSERT_EQ(get<1>(b).x, 42 + CTOR_DELTA + ASSIGN_DELTA);
  a = b;
  ASSERT_EQ(get<1>(a).x, 42 + CTOR_DELTA + ASSIGN_DELTA * 2);
  ASSERT_EQ(get<1>(b).x, 42 + CTOR_DELTA + ASSIGN_DELTA);
}

TEST(assignment, move_only) {
  OnlyMovable::move_assignment_called = 0;
  using V = ct::Variant<OnlyMovable>;
  V a(ct::in_place_type<OnlyMovable>);
  V b(ct::in_place_type<OnlyMovable>);
  a = std::move(b);
  ASSERT_TRUE(get<0>(a).has_coin());
  ASSERT_FALSE(get<0>(b).has_coin());
  ASSERT_EQ(OnlyMovable::move_assignment_called, 1);
}

TEST(assignment, different_alternatives) {
  using V = ct::Variant<std::vector<int>, std::vector<double>>;
  V a = std::vector{13.37, 2020.02};
  V b = std::vector{1337, 14882};
  a = b;
  ASSERT_TRUE(holds_alternative<std::vector<int>>(a));
}

TEST(assignment, converting_assignment2) {
  using V = ct::Variant<int, long>;
  int i2 = 7;
  const int ci2 = 6;

  V v1 = 11;
  V v2 = v1;
  V v3 = v1;
  V v4 = v1;
  V v5 = v1;

  v1 = 8;
  v2 = i2;
  v3 = ci2;
  v4 = std::move(i2);
  v5 = std::move(ci2);

  EXPECT_EQ(get<0>(v1), 8);
  EXPECT_EQ(get<0>(v2), 7);
  EXPECT_EQ(get<0>(v3), 6);
  EXPECT_EQ(get<0>(v4), 7);
  EXPECT_EQ(get<0>(v5), 6);
}

TEST(assignment, converting_assignment2_const) {
  using V = ct::Variant<const int, long>;
  long l2 = 7;
  const long cl2 = 6;

  V v1 = 11;
  V v2 = v1;
  V v3 = v1;
  V v4 = v1;
  V v5 = v1;

  v1 = 8L;
  v2 = l2;
  v3 = cl2;
  v4 = std::move(l2);
  v5 = std::move(cl2);

  EXPECT_EQ(get<1>(v1), 8);
  EXPECT_EQ(get<1>(v2), 7);
  EXPECT_EQ(get<1>(v3), 6);
  EXPECT_EQ(get<1>(v4), 7);
  EXPECT_EQ(get<1>(v5), 6);
}

TEST(assignment, duplicated_type_with_throwing_copy_ctor) {
  struct ThrowingCopy {
    ThrowingCopy() = default;

    ThrowingCopy(const ThrowingCopy&) {}

    ThrowingCopy(ThrowingCopy&&) noexcept {}

    ThrowingCopy& operator=(const ThrowingCopy&) = default;
  };

  ct::Variant<ThrowingCopy, ThrowingCopy> a, b;
  a = b;
}

TEST(valueless_by_exception, copy_assign_nothrow) {
  static constexpr ThrowingMemberParams params = {};
  using CountedCalls = ThrowingMembers<params>;
  CountedCalls::reset_counters();

  using V = ct::Variant<std::vector<int>, CountedCalls>;
  // std::cout << CountedCalls::move_calls() << std::endl;
  V v1 = std::vector{1, 2, 3};
  // std::cout << CountedCalls::move_calls() << std::endl;
  V v2 = ThrowingMembersConstructorTag{};

  // std::cout << "[look]" << std::endl;
  ASSERT_NO_THROW(v1 = v2);
  // std::cout << CountedCalls::move_calls() << std::endl;
  ASSERT_FALSE(v1.valueless_by_exception());
  ASSERT_EQ(CountedCalls::copy_calls(), 1);
  ASSERT_EQ(CountedCalls::move_calls(), 0);
}

TEST(valueless_by_exception, copy_assign_throwing_copy) {
  static constexpr ThrowingMemberParams params = {.throwing_copy = true};
  using ThrowingCopy = ThrowingMembers<params>;
  ThrowingCopy::reset_counters();

  using V = ct::Variant<std::vector<int>, ThrowingCopy>;
  V v1 = std::vector{1, 2, 3};
  V v2 = ThrowingMembersConstructorTag{};

  ASSERT_ANY_THROW(v1 = v2);
  ASSERT_FALSE(v1.valueless_by_exception());
  ASSERT_EQ(ThrowingCopy::copy_calls(), 1);
  ASSERT_EQ(ThrowingCopy::move_calls(), 0);
}

TEST(valueless_by_exception, copy_assign_throwing_move) {
  static constexpr ThrowingMemberParams params = {.throwing_move = true};
  using ThrowingMove = ThrowingMembers<params>;
  ThrowingMove::reset_counters();

  using V = ct::Variant<std::vector<int>, ThrowingMove>;
  V v1 = std::vector{1, 2, 3};
  V v2 = ThrowingMembersConstructorTag{};

  ASSERT_NO_THROW(v1 = v2);
  ASSERT_FALSE(v1.valueless_by_exception());
  ASSERT_EQ(ThrowingMove::copy_calls(), 1);
  ASSERT_EQ(ThrowingMove::move_calls(), 0);
}

TEST(valueless_by_exception, copy_assign_throwing_copy_and_move) {
  static constexpr ThrowingMemberParams params = {.throwing_copy = true, .throwing_move = true};
  using ThrowingCopyAndMove = ThrowingMembers<params>;
  ThrowingCopyAndMove::reset_counters();

  using V = ct::Variant<std::vector<int>, ThrowingCopyAndMove>;
  V v1 = std::vector{1, 2, 3};
  V v2 = ThrowingMembersConstructorTag{};

  ASSERT_ANY_THROW(v1 = v2);
  ASSERT_TRUE(v1.valueless_by_exception());
  ASSERT_EQ(ThrowingCopyAndMove::copy_calls(), 1);
  ASSERT_EQ(ThrowingCopyAndMove::move_calls(), 0);
}

TEST(valueless_by_exception, move_assign_nothrow) {
  static constexpr ThrowingMemberParams params = {};
  using CountedCalls = ThrowingMembers<params>;
  CountedCalls::reset_counters();

  using V = ct::Variant<std::vector<int>, CountedCalls>;
  V v1 = std::vector{1, 2, 3};
  V v2 = ThrowingMembersConstructorTag{};

  ASSERT_NO_THROW(v1 = std::move(v2));
  ASSERT_FALSE(v1.valueless_by_exception());
  ASSERT_EQ(CountedCalls::copy_calls(), 0);
  ASSERT_EQ(CountedCalls::move_calls(), 1);
}

TEST(valueless_by_exception, move_assign_throwing_copy) {
  static constexpr ThrowingMemberParams params = {.throwing_copy = true};
  using ThrowingCopy = ThrowingMembers<params>;
  ThrowingCopy::reset_counters();

  using V = ct::Variant<std::vector<int>, ThrowingCopy>;
  V v1 = std::vector{1, 2, 3};
  V v2 = ThrowingMembersConstructorTag{};

  ASSERT_NO_THROW(v1 = std::move(v2));
  ASSERT_FALSE(v1.valueless_by_exception());
  ASSERT_EQ(ThrowingCopy::copy_calls(), 0);
  ASSERT_EQ(ThrowingCopy::move_calls(), 1);
}

TEST(valueless_by_exception, move_assign_throwing_move) {
  static constexpr ThrowingMemberParams params = {.throwing_move = true};
  using ThrowingMove = ThrowingMembers<params>;
  ThrowingMove::reset_counters();

  using V = ct::Variant<std::vector<int>, ThrowingMove>;
  V v1 = std::vector{1, 2, 3};
  V v2 = ThrowingMembersConstructorTag{};

  ASSERT_ANY_THROW(v1 = std::move(v2));
  ASSERT_TRUE(v1.valueless_by_exception());
  ASSERT_EQ(ThrowingMove::copy_calls(), 0);
  ASSERT_EQ(ThrowingMove::move_calls(), 1);
}

TEST(valueless_by_exception, move_assign_throwing_copy_and_move) {
  static constexpr ThrowingMemberParams params = {.throwing_copy = true, .throwing_move = true};
  using ThrowingCopyAndMove = ThrowingMembers<params>;
  ThrowingCopyAndMove::reset_counters();

  using V = ct::Variant<std::vector<int>, ThrowingCopyAndMove>;
  V v1 = std::vector{1, 2, 3};
  V v2 = ThrowingMembersConstructorTag{};

  ASSERT_ANY_THROW(v1 = std::move(v2));
  ASSERT_TRUE(v1.valueless_by_exception());
  ASSERT_EQ(ThrowingCopyAndMove::copy_calls(), 0);
  ASSERT_EQ(ThrowingCopyAndMove::move_calls(), 1);
}

TEST(valueless_by_exception, converting_assign_nothrow) {
  static constexpr ThrowingMemberParams params = {};
  using CountedCalls = ThrowingMembers<params>;
  CountedCalls::reset_counters();

  using V = ct::Variant<std::vector<int>, CountedCalls>;
  V v1 = std::vector{1, 2, 3};

  ASSERT_NO_THROW(v1 = ThrowingMembersConstructorTag{});
  ASSERT_FALSE(v1.valueless_by_exception());
  ASSERT_EQ(CountedCalls::copy_calls(), 0);
  ASSERT_EQ(CountedCalls::move_calls(), 0);
}

TEST(valueless_by_exception, converting_assign_throwing_conv) {
  static constexpr ThrowingMemberParams params = {};

  struct CountedCalls : ThrowingMembers<params> {
    CountedCalls(ThrowingMembersConstructorTag t)
        : ThrowingMembers(t) {
      throw std::exception();
    }
  };

  CountedCalls::reset_counters();

  using V = ct::Variant<std::vector<int>, CountedCalls>;
  V v1 = std::vector{1, 2, 3};

  ASSERT_ANY_THROW(v1 = ThrowingMembersConstructorTag{});
  ASSERT_FALSE(v1.valueless_by_exception());
  ASSERT_EQ(CountedCalls::copy_calls(), 0);
  ASSERT_EQ(CountedCalls::move_calls(), 0);
}

TEST(valueless_by_exception, converting_assign_throwing_move) {
  static constexpr ThrowingMemberParams params = {.throwing_move = true};
  using CountedCalls = ThrowingMembers<params>;
  CountedCalls::reset_counters();

  using V = ct::Variant<std::vector<int>, CountedCalls>;
  V v1 = std::vector{1, 2, 3};

  ASSERT_NO_THROW(v1 = ThrowingMembersConstructorTag{});
  ASSERT_FALSE(v1.valueless_by_exception());
  ASSERT_EQ(CountedCalls::copy_calls(), 0);
  ASSERT_EQ(CountedCalls::move_calls(), 0);
}

TEST(valueless_by_exception, converting_assign_throwing_conv_and_move) {
  static constexpr ThrowingMemberParams params = {.throwing_move = true};

  struct CountedCalls : ThrowingMembers<params> {
    CountedCalls(ThrowingMembersConstructorTag t)
        : ThrowingMembers(t) {
      throw std::exception();
    }
  };

  CountedCalls::reset_counters();

  using V = ct::Variant<std::vector<int>, CountedCalls>;
  V v1 = std::vector{1, 2, 3};

  ASSERT_ANY_THROW(v1 = ThrowingMembersConstructorTag{});
  ASSERT_TRUE(v1.valueless_by_exception());
  ASSERT_EQ(CountedCalls::copy_calls(), 0);
  ASSERT_EQ(CountedCalls::move_calls(), 0);
}

TEST(constructor, move_only) {
  using V = ct::Variant<OnlyMovable>;
  V a(ct::in_place_type<OnlyMovable>);
  V b(std::move(a));
  ASSERT_TRUE(get<0>(b).has_coin());
  ASSERT_FALSE(get<0>(a).has_coin());
}

TEST(constructor, ctad) {
  using V = ct::Variant<int, double>;
  V a(3);
  ct::Variant b = a;
  ct::Variant c = ct::Variant(a);
  EXPECT_TRUE(std::is_same_v<decltype(b), V>);
  EXPECT_TRUE(std::is_same_v<decltype(c), V>);
}

TEST(constructor, converting_ctor) {
  using V = ct::Variant<int, long>;
  int i1 = 10;
  const int ci1 = 9;

  V v1 = 11;
  V v2 = i1;
  V v3 = ci1;
  V v4 = std::move(i1);
  V v5 = std::move(ci1);

  EXPECT_EQ(get<0>(v1), 11);
  EXPECT_EQ(get<0>(v2), 10);
  EXPECT_EQ(get<0>(v3), 9);
  EXPECT_EQ(get<0>(v4), 10);
  EXPECT_EQ(get<0>(v5), 9);
}

TEST(constructor, converting_ctor_const) {
  using V = ct::Variant<const int, long>;
  int i1 = 10;
  const int ci1 = 9;

  V v1 = 11;
  V v2 = i1;
  V v3 = ci1;
  V v4 = std::move(i1);
  V v5 = std::move(ci1);

  EXPECT_EQ(get<0>(v1), 11);
  EXPECT_EQ(get<0>(v2), 10);
  EXPECT_EQ(get<0>(v3), 9);
  EXPECT_EQ(get<0>(v4), 10);
  EXPECT_EQ(get<0>(v5), 9);
}

TEST(destructor, emplace) {
  NonTrivialDestructor::reset_counters();
  {
    ct::Variant<NonTrivialDestructor, int> v;
    int x = 14882;
    v.emplace<1>(x);
    ASSERT_EQ(NonTrivialDestructor::destructor_count, 1);
  }
  {
    ct::Variant<NonTrivialDestructor, int> v;
    int x = 14882;
    v.emplace<int>(x);
    ASSERT_EQ(NonTrivialDestructor::destructor_count, 2);
  }
}

TEST(destructor, copy_assignment) {
  NonTrivialDestructor::reset_counters();
  {
    ct::Variant<NonTrivialDestructor, int> v;
    ct::Variant<NonTrivialDestructor, int> u = 14882;
    v = u;
  }
  ASSERT_EQ(NonTrivialDestructor::destructor_count, 1);
}

TEST(destructor, move_assignment) {
  NonTrivialDestructor::reset_counters();
  {
    ct::Variant<NonTrivialDestructor, int> v;
    v = ct::Variant<NonTrivialDestructor, int>(14882);
  }
  ASSERT_EQ(NonTrivialDestructor::destructor_count, 1);
}

TEST(destructor, converting_assignment) {
  NonTrivialDestructor::reset_counters();
  {
    ct::Variant<NonTrivialDestructor, int> v;
    v = 14882;
  }
  ASSERT_EQ(NonTrivialDestructor::destructor_count, 1);
}

template <typename Var>
static constexpr bool test_equal(const Var& l, const Var& r, bool expect_equal) {
  return ((l == r) == expect_equal) && (!(l != r) == expect_equal) && ((r == l) == expect_equal) &&
         (!(r != l) == expect_equal);
}

TEST(relops, equality) {
  using V = ct::Variant<NonTrivialIntWrapper, int, std::string>;
  {
    V v1(ct::in_place_index<0>, 42);
    V v2(ct::in_place_index<0>, 42);
    ASSERT_TRUE(test_equal(v1, v2, true));
  }
  {
    V v1(ct::in_place_index<0>, 42);
    V v2(ct::in_place_index<0>, 43);
    ASSERT_TRUE(test_equal(v1, v2, false));
  }
  {
    V v1(ct::in_place_index<0>, 42);
    V v2(ct::in_place_index<1>, 42);
    ASSERT_TRUE(test_equal(v1, v2, false));
  }
}

template <typename Var>
static constexpr bool test_less(const Var& l, const Var& r, bool expect_less, bool expect_greater) {
  return ((l < r) == expect_less) && (!(l >= r) == expect_less) && ((l > r) == expect_greater) &&
         (!(l <= r) == expect_greater);
}

TEST(relops, relational_basic) {
  using V = ct::Variant<NonTrivialIntWrapper, int, std::string>;
  {
    V v1(ct::in_place_index<0>, 42);
    V v2(ct::in_place_index<0>, 42);
    ASSERT_TRUE(test_less(v1, v2, false, false));
    ASSERT_TRUE(test_less(v2, v1, false, false));
  }
  {
    V v1(ct::in_place_index<0>, 42);
    V v2(ct::in_place_index<0>, 43);
    ASSERT_TRUE(test_less(v1, v2, true, false));
    ASSERT_TRUE(test_less(v2, v1, false, true));
  }
  {
    V v1(ct::in_place_index<0>, 43);
    V v2(ct::in_place_index<1>, 42);
    ASSERT_TRUE(test_less(v1, v2, true, false));
    ASSERT_TRUE(test_less(v2, v1, false, true));
  }
}

TEST(relops, relational_empty) {
  using V = ct::Variant<int, EmptyComparable, std::string>;
  {
    V v1, v2;
    ASSERT_ANY_THROW(v2 = V(ct::in_place_type<EmptyComparable>));
    ASSERT_TRUE(v2.valueless_by_exception());
    ASSERT_TRUE(test_less(v1, v2, false, true));
    ASSERT_TRUE(test_less(v2, v1, true, false));
  }
  {
    V v1, v2;
    ASSERT_ANY_THROW(v1 = V(ct::in_place_type<EmptyComparable>));
    ASSERT_TRUE(v1.valueless_by_exception());
    ASSERT_ANY_THROW(v2 = V(ct::in_place_type<EmptyComparable>));
    ASSERT_TRUE(v2.valueless_by_exception());
    ASSERT_TRUE(test_less(v1, v2, false, false));
    ASSERT_TRUE(test_less(v2, v1, false, false));
  }
}

TEST(relops, relational_custom) {
  ComparisonCounters ca, cb;
  ct::Variant<CustomComparable> a(ct::in_place_index<0>, 42, &ca);
  ct::Variant<CustomComparable> b(ct::in_place_index<0>, 43, &cb);

  EXPECT_FALSE(operator==(a, b));
  EXPECT_TRUE(operator!=(a, b));
  EXPECT_TRUE(operator<(a, b));
  EXPECT_TRUE(operator<=(a, b));
  EXPECT_FALSE(operator>(a, b));
  EXPECT_FALSE(operator>=(a, b));
  EXPECT_EQ(operator<=>(a, b), std::strong_ordering::less);

  EXPECT_TRUE(operator==(a, a));
  EXPECT_FALSE(operator!=(a, a));
  EXPECT_FALSE(operator<(a, a));
  EXPECT_TRUE(operator<=(a, a));
  EXPECT_FALSE(operator>(a, a));
  EXPECT_TRUE(operator>=(a, a));
  EXPECT_EQ(operator<=>(a, a), std::strong_ordering::equal);

  EXPECT_FALSE(operator==(b, a));
  EXPECT_TRUE(operator!=(b, a));
  EXPECT_FALSE(operator<(b, a));
  EXPECT_FALSE(operator<=(b, a));
  EXPECT_TRUE(operator>(b, a));
  EXPECT_TRUE(operator>=(b, a));
  EXPECT_EQ(operator<=>(b, a), std::strong_ordering::greater);

  EXPECT_EQ(ca.equal, 2);
  EXPECT_EQ(ca.not_equal, 2);
  EXPECT_EQ(ca.less, 2);
  EXPECT_EQ(ca.less_equal, 2);
  EXPECT_EQ(ca.greater, 2);
  EXPECT_EQ(ca.greater_equal, 2);
  EXPECT_EQ(ca.spaceship, 2);

  EXPECT_EQ(cb.equal, 1);
  EXPECT_EQ(cb.not_equal, 1);
  EXPECT_EQ(cb.less, 1);
  EXPECT_EQ(cb.less_equal, 1);
  EXPECT_EQ(cb.greater, 1);
  EXPECT_EQ(cb.greater_equal, 1);
  EXPECT_EQ(cb.spaceship, 1);
}

TEST(relops, three_way_category) {
  using V1 = ct::Variant<PartiallyOrdered>;
  using V2 = ct::Variant<WeakOrdered>;
  using V3 = ct::Variant<StrongOrdered>;
  using V4 = ct::Variant<StrongOrdered, WeakOrdered>;
  using V5 = ct::Variant<PartiallyOrdered, WeakOrdered, StrongOrdered>;
  using V6 = ct::Variant<StrongOrdered, WeakOrdered, PartiallyOrdered>;

  EXPECT_TRUE(std::is_same_v<std::partial_ordering, std::compare_three_way_result_t<V1>>);
  EXPECT_TRUE(std::is_same_v<std::weak_ordering, std::compare_three_way_result_t<V2>>);
  EXPECT_TRUE(std::is_same_v<std::strong_ordering, std::compare_three_way_result_t<V3>>);
  EXPECT_TRUE(std::is_same_v<std::weak_ordering, std::compare_three_way_result_t<V4>>);
  EXPECT_TRUE(std::is_same_v<std::partial_ordering, std::compare_three_way_result_t<V5>>);
  EXPECT_TRUE(std::is_same_v<std::partial_ordering, std::compare_three_way_result_t<V6>>);
}

TEST(relops, three_way_propagate) {
  using V = ct::Variant<int, double>;
  constexpr double nan = std::numeric_limits<double>::quiet_NaN();

  {
    V v1(ct::in_place_type<int>, 1);
    V v2(ct::in_place_type<double>, nan);
    EXPECT_EQ(v1 <=> v2, std::partial_ordering::less);
  }
  {
    V v1(ct::in_place_type<double>, nan);
    V v2(ct::in_place_type<int>, 2);
    EXPECT_EQ(v1 <=> v2, std::partial_ordering::greater);
  }
  {
    V v1(ct::in_place_type<double>, nan);
    V v2(ct::in_place_type<double>, nan);
    EXPECT_EQ(v1 <=> v2, std::partial_ordering::unordered);
  }
}

// TODO [my]
// struct structure {
//
//  constexpr structure() {};
//
//  struct subStruct {
//    union {
//      const int a;
//      double s;
//    };
//
//    constexpr subStruct() {}
//  };
//
//  subStruct storage;
//
//  constexpr void func() {
//    storage;
////    std::construct_at(&storage.a, 1);
// }
// };
//
// static_assert([]() {
// structure a;
// a.func();
// return true;
// }());

// static_assert(std::is_default_constructible_v<const int>);

// TODO  [my]

static_assert([] {
  ct::Variant<ConstexprNonTrivialDestructor, int, int> a;
  a.emplace<2>(42);

  return true;
}());

static_assert([] {
  ct::Variant<const int, const ConstexprNonTrivialDestructor> a;
  auto b = a;

  return true;
}());

static_assert([] {
  using A = ConstexprNonTrivialDestructor;
  using V = ct::Variant<int, const int, const A, A>;

  {
    V a1(ct::in_place_index<1>, 42);
    V b1 = a1;
    if (b1.index() != a1.index()) {
      return false;
    }
  }

  {
    V a2(ct::in_place_index<2>);
    V b2 = a2;
    if (b2.index() != a2.index()) {
      return false;
    }
  }

  return true;
}());

static_assert([] {
  using A = ConstexprNonTrivialDestructor;
  using V = ct::Variant<int, int, A, A>;

  {
    V a1(ct::in_place_index<1>, 42);
    V b1 = a1;
    if (b1.index() != a1.index()) {
      return false;
    }
    b1 = a1;
    if (b1.index() != a1.index()) {
      return false;
    }
    b1 = std::move(a1);
    if (b1.index() != a1.index()) {
      return false;
    }
  }

  {
    V a2(ct::in_place_index<2>);
    V b2 = a2;
    if (b2.index() != a2.index()) {
      return false;
    }
    b2 = a2;
    if (b2.index() != a2.index()) {
      return false;
    }
    b2 = std::move(a2);
    if (b2.index() != a2.index()) {
      return false;
    }
  }

  return true;
}());

// ===============================addition===============================

struct InitIncluding {
  constexpr ~InitIncluding() = default;

  constexpr InitIncluding(std::initializer_list<int> il, int addition)
      : _il(il)
      , _x(addition) {}

  friend constexpr bool operator==(const InitIncluding& lhs, const InitIncluding& rhs) {
    return lhs._x == rhs._x && lhs._il == rhs._il;
  }

  std::vector<int> _il;
  int _x;
};

static_assert([]() {
  ct::Variant<InitIncluding> v(ct::in_place_index<0>, {1, 2, 3}, 42);
  return true;
}());

static_assert([]() {
  ct::Variant<InitIncluding> v(ct::in_place_type<InitIncluding>, {1, 2, 3}, 42);
  return true;
}());

TEST(traits, initlist_constructor) {
  ct::Variant<InitIncluding> v_index(ct::in_place_index<0>, {1, 2, 3}, 42);
  ASSERT_TRUE(v_index.index() == 0);
  ASSERT_TRUE(holds_alternative<InitIncluding>(v_index));
  ASSERT_TRUE(get<InitIncluding>(v_index) == InitIncluding({1, 2, 3}, 42));

  ct::Variant<InitIncluding> v_type(ct::in_place_type<InitIncluding>, {1, 2, 3}, 42);
  ASSERT_TRUE(v_type.index() == 0);
  ASSERT_TRUE(holds_alternative<InitIncluding>(v_type));
  ASSERT_TRUE(get<InitIncluding>(v_type) == InitIncluding({1, 2, 3}, 42));
}

} // namespace ct_test
