#include "optional.h"

#include <gtest/gtest.h>

#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace ct_test {

namespace {

// TODO: Refactor test classes (maybe use mixins?)

struct Dummy {};

struct NoDefaultConstructor {
  NoDefaultConstructor() = delete;
};

struct ThrowingDefaultConstructor {
  ThrowingDefaultConstructor() {
    throw std::exception();
  }
};

struct ThrowingMoveAssignment {
  ThrowingMoveAssignment() = default;

  ThrowingMoveAssignment(ThrowingMoveAssignment&&) {
    throw std::exception();
  }

  ThrowingMoveAssignment& operator=(ThrowingMoveAssignment&&) = default;

  [[maybe_unused]] friend void swap(ThrowingMoveAssignment&, ThrowingMoveAssignment&) {
    throw std::exception();
  }
};

struct NoCopy {
  NoCopy(const NoCopy&) = delete;
};

struct NoMove {
  NoMove(NoMove&&) = delete;
};

struct NonTrivialCopy {
  explicit NonTrivialCopy(int x) noexcept
      : x{x} {}

  NonTrivialCopy(const NonTrivialCopy& other) noexcept
      : x{other.x + 1} {}

  int x;
};

struct NonTrivialCopyAssignment {
  explicit NonTrivialCopyAssignment(int x) noexcept
      : x{x} {}

  NonTrivialCopyAssignment& operator=(const NonTrivialCopyAssignment& other) {
    if (this != &other) {
      x = other.x + 5;
    }
    return *this;
  }

  int x;
};

struct NoMoveAssignment {
  NoMoveAssignment& operator=(NoMoveAssignment&&) = delete;
};

struct NoCopyAssignment {
  NoCopyAssignment& operator=(const NoCopyAssignment&) = delete;
};

struct MoveOnly {
  MoveOnly(const MoveOnly&) = delete;
  MoveOnly(MoveOnly&&) = default;

  MoveOnly& operator=(const MoveOnly&) = delete;
  MoveOnly& operator=(MoveOnly&&) = default;
};

struct CopyOnly {
  CopyOnly(const CopyOnly&) = default;
  CopyOnly(CopyOnly&&) = delete;

  CopyOnly& operator=(const CopyOnly&) = default;
  CopyOnly& operator=(CopyOnly&&) = delete;
};

struct Copyable {
  Copyable(const Copyable&) = default;

  Copyable& operator=(const Copyable&) & = default;
};

struct AssignableFromInt {
  explicit AssignableFromInt(int) noexcept {}

  AssignableFromInt& operator=(int) & noexcept {
    return *this;
  }
};

struct ThrowingSwappable {};

[[maybe_unused]] void swap(ThrowingSwappable&, ThrowingSwappable&) {}

struct NonSwappable {};

[[maybe_unused]] void swap(NonSwappable&, NonSwappable&) = delete;

struct ImplicitlyConstructibleFromAny {
  template <typename T>
  constexpr ImplicitlyConstructibleFromAny(T&&) {}
};

struct SwappableNoMove {
  SwappableNoMove(const SwappableNoMove&) = delete;
  SwappableNoMove& operator=(const SwappableNoMove&) = delete;
};

[[maybe_unused]] void swap(SwappableNoMove&, SwappableNoMove&) noexcept {}

struct SwappableNoMoveAssignment {
  SwappableNoMoveAssignment(const SwappableNoMoveAssignment&) = delete;
  SwappableNoMoveAssignment(SwappableNoMoveAssignment&&) = default;

  SwappableNoMoveAssignment& operator=(const SwappableNoMoveAssignment&) = delete;
  SwappableNoMoveAssignment& operator=(SwappableNoMoveAssignment&&) = delete;
};

[[maybe_unused]] void swap(SwappableNoMoveAssignment&, SwappableNoMoveAssignment&) noexcept {}

} // namespace

TEST(TraitsTest, Types) {
  static_assert(std::is_same_v<ct::Optional<int>::ValueType, int>);
  static_assert(std::is_same_v<ct::Optional<const std::string>::ValueType, const std::string>);
}

TEST(TraitsTest, Nullopt) {
  static_assert(std::is_same_v<decltype(ct::nullopt), const ct::NullOpt>);
  static_assert(std::is_empty_v<ct::NullOpt>);
  static_assert(!std::is_default_constructible_v<ct::NullOpt>);
}

TEST(TraitsTest, NulloptPtr) {
  const ct::NullOpt* get_nullopt_ptr() noexcept;

  EXPECT_EQ(&ct::nullopt, get_nullopt_ptr());
}

TEST(TraitsTest, InPlace) {
  static_assert(std::is_same_v<decltype(ct::in_place), const ct::InPlace>);
  static_assert(std::is_empty_v<ct::InPlace>);
  static_assert(std::is_default_constructible_v<ct::InPlace>);
}

TEST(TraitsTest, InPlacePtr) {
  const ct::InPlace* get_in_place_ptr() noexcept;

  EXPECT_EQ(&ct::in_place, get_in_place_ptr());
}

TEST(TraitsTest, Destructor) {
  static_assert(std::is_trivially_destructible_v<ct::Optional<int>>);
  static_assert(!std::is_trivially_destructible_v<ct::Optional<std::string>>);
}

TEST(TraitsTest, DefaultConstructor) {
  static_assert(std::is_nothrow_default_constructible_v<ct::Optional<std::vector<int>>>);
  static_assert(std::is_nothrow_default_constructible_v<ct::Optional<NoDefaultConstructor>>);
  static_assert(std::is_nothrow_default_constructible_v<ct::Optional<ThrowingDefaultConstructor>>);
}

TEST(TraitsTest, NulloptConstructor) {
  static_assert(std::is_nothrow_constructible_v<ct::Optional<std::vector<int>>, ct::NullOpt>);
  static_assert(std::is_nothrow_constructible_v<ct::Optional<NoDefaultConstructor>, ct::NullOpt>);
  static_assert(std::is_nothrow_constructible_v<ct::Optional<ThrowingDefaultConstructor>, ct::NullOpt>);
}

TEST(TraitsTest, CopyConstructor) {
  static_assert(!std::is_copy_constructible_v<ct::Optional<NoCopy>>);
  static_assert(std::is_copy_constructible_v<ct::Optional<int>>);
  static_assert(std::is_nothrow_copy_constructible_v<ct::Optional<int>>);
  static_assert(std::is_copy_constructible_v<ct::Optional<std::vector<std::string>>>);
  static_assert(!std::is_nothrow_copy_constructible_v<ct::Optional<std::vector<std::string>>>);
  static_assert(!std::is_trivially_copy_constructible_v<ct::Optional<std::vector<std::string>>>);
  static_assert(std::is_trivially_copy_constructible_v<ct::Optional<Dummy>>);
  static_assert(!std::is_trivially_copy_constructible_v<ct::Optional<NonTrivialCopy>>);
  static_assert(std::is_trivially_copy_constructible_v<ct::Optional<CopyOnly>>);
}

TEST(TraitsTest, MoveConstructor) {
  static_assert(!std::is_move_constructible_v<ct::Optional<NoMove>>);
  static_assert(std::is_move_constructible_v<ct::Optional<std::string>>);
  static_assert(std::is_nothrow_move_constructible_v<ct::Optional<std::string>>);
  static_assert(!std::is_trivially_move_constructible_v<ct::Optional<std::string>>);
  static_assert(std::is_move_constructible_v<ct::Optional<Dummy>>);
  static_assert(std::is_trivially_move_constructible_v<ct::Optional<Dummy>>);
  static_assert(std::is_move_constructible_v<ct::Optional<ThrowingMoveAssignment>>);
  static_assert(std::is_trivially_move_constructible_v<ct::Optional<MoveOnly>>);
}

TEST(TraitsTest, CopyAssignment) {
  static_assert(!std::is_copy_assignable_v<ct::Optional<NoCopy>>);
  static_assert(!std::is_copy_assignable_v<ct::Optional<NoCopyAssignment>>);
  static_assert(std::is_copy_assignable_v<ct::Optional<NonTrivialCopyAssignment>>);
  static_assert(std::is_copy_assignable_v<ct::Optional<NonTrivialCopy>>);
  static_assert(std::is_copy_assignable_v<ct::Optional<Dummy>>);
  static_assert(std::is_nothrow_copy_assignable_v<ct::Optional<Dummy>>);
  static_assert(std::is_copy_assignable_v<ct::Optional<std::string>>);
  static_assert(!std::is_nothrow_copy_assignable_v<ct::Optional<std::string>>);
  static_assert(!std::is_trivially_copy_assignable_v<ct::Optional<NonTrivialCopyAssignment>>);
  static_assert(!std::is_trivially_copy_assignable_v<ct::Optional<NonTrivialCopy>>);
  static_assert(std::is_trivially_copy_assignable_v<ct::Optional<Dummy>>);
}

TEST(TraitsTest, MoveAssignment) {
  static_assert(!std::is_move_assignable_v<ct::Optional<NoMove>>);
  static_assert(!std::is_move_assignable_v<ct::Optional<NoMoveAssignment>>);
  static_assert(std::is_move_assignable_v<ct::Optional<std::vector<double>>>);
  static_assert(std::is_move_assignable_v<ct::Optional<std::string>>);
  static_assert(std::is_nothrow_move_assignable_v<ct::Optional<std::string>>);
  static_assert(std::is_move_assignable_v<ct::Optional<Dummy>>);
  static_assert(std::is_nothrow_move_assignable_v<ct::Optional<Dummy>>);
  static_assert(!std::is_trivially_move_assignable_v<ct::Optional<std::vector<double>>>);
  static_assert(!std::is_trivially_move_assignable_v<ct::Optional<std::string>>);
  static_assert(std::is_trivially_move_assignable_v<ct::Optional<Dummy>>);
  static_assert(std::is_move_assignable_v<ct::Optional<ThrowingMoveAssignment>>);
}

TEST(TraitsTest, Swap) {
  static_assert(std::is_nothrow_swappable_v<ct::Optional<Dummy>>);
  static_assert(std::is_nothrow_swappable_v<ct::Optional<std::string>>);
  static_assert(std::is_nothrow_swappable_v<ct::Optional<MoveOnly>>);
  static_assert(std::is_nothrow_swappable_v<ct::Optional<Copyable>>);
  static_assert(std::is_nothrow_swappable_v<ct::Optional<SwappableNoMoveAssignment>>);

  static_assert(std::is_swappable_v<ct::Optional<ThrowingSwappable>>);
  static_assert(!std::is_nothrow_swappable_v<ct::Optional<ThrowingSwappable>>);

  // See https://wg21.link/LWG2766
  static_assert(!std::is_swappable_v<ct::Optional<NonSwappable>>);
  static_assert(!std::is_swappable_v<ct::Optional<CopyOnly>>);
  static_assert(!std::is_swappable_v<ct::Optional<NoMove>>);
  static_assert(!std::is_swappable_v<ct::Optional<SwappableNoMove>>);
}

TEST(TraitsTest, ValueConstructor) {
  static_assert(std::is_constructible_v<ct::Optional<int>, int>);
  static_assert(std::is_convertible_v<int, ct::Optional<int>>);

  static_assert(std::is_constructible_v<ct::Optional<int>, long>);
  static_assert(std::is_convertible_v<long, ct::Optional<int>>);

  static_assert(std::is_constructible_v<ct::Optional<const int>, long>);
  static_assert(std::is_convertible_v<long, ct::Optional<const int>>);

  static_assert(std::is_constructible_v<ct::Optional<std::string>, const char*>);
  static_assert(std::is_convertible_v<const char*, ct::Optional<std::string>>);

  static_assert(std::is_constructible_v<ct::Optional<std::string>, std::string_view>);
  static_assert(!std::is_convertible_v<std::string_view, ct::Optional<std::string>>);

  static_assert(!std::is_constructible_v<ct::Optional<std::string>, int>);

  using opt = ct::Optional<ImplicitlyConstructibleFromAny>;
  static_assert(!std::is_convertible_v<ct::InPlace, opt>);
  static_assert(!std::is_convertible_v<ct::InPlace&, opt>);
  static_assert(!std::is_convertible_v<const ct::InPlace&, opt>);
  static_assert(!std::is_convertible_v<ct::InPlace&&, opt>);
  static_assert(!std::is_convertible_v<const ct::InPlace&&, opt>);
}

TEST(TraitsTest, ValueAssignment) {
  static_assert(std::is_nothrow_assignable_v<ct::Optional<int>&, int>);
  static_assert(!std::is_assignable_v<const ct::Optional<int>&, int>);

  static_assert(std::is_nothrow_assignable_v<ct::Optional<int>&, long>);
  static_assert(!std::is_assignable_v<const ct::Optional<int>&, long>);

  static_assert(!std::is_nothrow_assignable_v<ct::Optional<const int>&, long>);
  static_assert(!std::is_assignable_v<const ct::Optional<const int>&, long>);

  static_assert(std::is_assignable_v<ct::Optional<std::string>&, const char*>);
  static_assert(!std::is_nothrow_assignable_v<ct::Optional<std::string>&, const char*>);
  static_assert(!std::is_assignable_v<const ct::Optional<std::string>&, const char*>);

  static_assert(std::is_assignable_v<ct::Optional<std::string>&, std::string_view>);
  static_assert(!std::is_nothrow_assignable_v<ct::Optional<std::string>&, std::string_view>);
  static_assert(!std::is_assignable_v<const ct::Optional<std::string>&, std::string_view>);

  static_assert(!std::is_assignable_v<ct::Optional<std::string>&, int>);
  static_assert(!std::is_assignable_v<const ct::Optional<std::string>&, int>);

  static_assert(std::is_nothrow_assignable_v<ct::Optional<AssignableFromInt>&, int>);
  static_assert(!std::is_assignable_v<const ct::Optional<AssignableFromInt>&, int>);
}

namespace {

template <bool B>
struct ConditionallyNoexcept1 {
  ConditionallyNoexcept1() noexcept(B);
  ConditionallyNoexcept1(const ConditionallyNoexcept1&) noexcept(B);

  ConditionallyNoexcept1(int) noexcept(B);

  ConditionallyNoexcept1(const ConditionallyNoexcept1<!B>&) noexcept(B);

  ConditionallyNoexcept1& operator=(const ConditionallyNoexcept1&) noexcept(false);
};

template <bool B>
struct ConditionallyNoexcept2 {
  ConditionallyNoexcept2() noexcept(B);

  ConditionallyNoexcept2(const ConditionallyNoexcept2&) noexcept(B);
  ConditionallyNoexcept2& operator=(const ConditionallyNoexcept2&) noexcept(B);

  ConditionallyNoexcept2(int) noexcept(B);
  ConditionallyNoexcept2& operator=(int) noexcept(false);

  ConditionallyNoexcept2(void*) noexcept(true);
  ConditionallyNoexcept2& operator=(void*) noexcept(B);

  ConditionallyNoexcept2(const ConditionallyNoexcept2<!B>&) noexcept(B);
  ConditionallyNoexcept2& operator=(const ConditionallyNoexcept2<!B>&) noexcept(B);
};

} // namespace

TEST(TraitsTest, NoexceptConstructor) {
  using X = ConditionallyNoexcept1<true>;
  using Y = ConditionallyNoexcept1<false>;
  using OX = ct::Optional<X>;
  using OY = ct::Optional<Y>;

  static_assert(std::is_nothrow_constructible_v<OX>);
  static_assert(std::is_nothrow_constructible_v<OX, ct::NullOpt>);
  static_assert(std::is_nothrow_constructible_v<OX, const X&>);
  static_assert(std::is_nothrow_constructible_v<OX, X>);
  static_assert(std::is_nothrow_constructible_v<OX, ct::InPlace, short>);
  static_assert(std::is_nothrow_constructible_v<OX, const Y&>);
  static_assert(std::is_nothrow_constructible_v<OX, Y>);

  static_assert(std::is_nothrow_constructible_v<OY>);
  static_assert(std::is_nothrow_constructible_v<OY, ct::NullOpt>);
  static_assert(!std::is_nothrow_constructible_v<OY, const Y&>);
  static_assert(!std::is_nothrow_constructible_v<OY, Y>);
  static_assert(!std::is_nothrow_constructible_v<OY, ct::InPlace, short>);
  static_assert(!std::is_nothrow_constructible_v<OY, const X&>);
  static_assert(!std::is_nothrow_constructible_v<OY, X>);
}

TEST(TraitsTest, NoexceptAssignment) {
  using X = ConditionallyNoexcept2<true>;
  using Y = ConditionallyNoexcept2<false>;
  using OX = ct::Optional<X>;
  using OY = ct::Optional<Y>;

  static_assert(std::is_nothrow_assignable_v<OX, ct::NullOpt>);
  static_assert(std::is_nothrow_assignable_v<OX, const X&>);
  static_assert(std::is_nothrow_assignable_v<OX, X>);
  static_assert(!std::is_nothrow_assignable_v<OX, int>);
  static_assert(std::is_nothrow_assignable_v<OX, void*>);
  static_assert(std::is_nothrow_assignable_v<OX, const Y&>);
  static_assert(std::is_nothrow_assignable_v<OX, Y>);

  static_assert(std::is_nothrow_assignable_v<OY, ct::NullOpt>);
  static_assert(!std::is_nothrow_assignable_v<OY, const Y&>);
  static_assert(!std::is_nothrow_assignable_v<OY, Y>);
  static_assert(!std::is_nothrow_assignable_v<OY, int>);
  static_assert(!std::is_nothrow_assignable_v<OY, void*>);
  static_assert(!std::is_nothrow_assignable_v<OY, const X&>);
  static_assert(!std::is_nothrow_assignable_v<OY, X>);

  static_assert(noexcept(std::declval<OX&>().emplace()));
  static_assert(noexcept(std::declval<OX&>().emplace(std::declval<X&>())));
  static_assert(noexcept(std::declval<OX&>().emplace(1)));
  static_assert(noexcept(std::declval<OX&>().emplace(nullptr)));
  static_assert(noexcept(std::declval<OX&>().emplace(std::declval<Y&>())));

  static_assert(!noexcept(std::declval<OY&>().emplace()));
  static_assert(!noexcept(std::declval<OY&>().emplace(std::declval<Y&>())));
  static_assert(!noexcept(std::declval<OY&>().emplace(1)));
  static_assert(noexcept(std::declval<OY&>().emplace(nullptr)));
  static_assert(!noexcept(std::declval<OY&>().emplace(std::declval<X&>())));
}

TEST(TraitsTest, Lwg2762) {
  struct S {
    void can_throw();
    void cannot_throw() noexcept;
  };

  static_assert(!noexcept(std::declval<ct::Optional<S>&>()->can_throw()));
  static_assert(noexcept(std::declval<ct::Optional<S>&>()->cannot_throw()));

  static_assert(noexcept(std::declval<ct::Optional<S>&>().operator->()));
  static_assert(noexcept(std::declval<ct::Optional<int>&>().operator->()));

  static_assert(noexcept(*std::declval<ct::Optional<int>&>()));
  static_assert(noexcept(*std::declval<const ct::Optional<int>&>()));
  static_assert(noexcept(*std::declval<ct::Optional<int>&&>()));
  static_assert(noexcept(*std::declval<const ct::Optional<int>&&>()));
}

} // namespace ct_test
