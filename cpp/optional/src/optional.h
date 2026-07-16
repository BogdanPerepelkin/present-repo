#pragma once

#include "chain-optional.h"

#include <compare>

namespace ct {

struct ProxyDefConstructible {};

struct NullOpt {
  explicit constexpr NullOpt(ProxyDefConstructible) noexcept {}
};

inline constexpr NullOpt nullopt{ProxyDefConstructible{}};

struct InPlace {
  constexpr InPlace() {}
};

inline constexpr InPlace in_place = InPlace();

template <typename T>
class Optional : public detail::MoveAssignmentBase<T> {
public:
  using detail::MoveAssignmentBase<T>::MoveAssignmentBase;

  using ValueType = T;
  using Chain = detail::MoveAssignmentBase<T>;

  constexpr Optional(const Optional& other) = default;

  constexpr Optional(Optional&& other) = default;

  constexpr Optional() noexcept = default;

  constexpr Optional(NullOpt) noexcept
      : Chain() {}

  template <
      typename U = T,
      std::enable_if_t<
          !std::same_as<std::remove_cvref_t<U>, InPlace> && !std::same_as<std::remove_cvref_t<U>, Optional> &&
              std::is_constructible_v<T, U&&>,
          bool> = true>
  explicit(!std::is_convertible_v<U, T>) constexpr Optional(
      U&& value
  ) noexcept(std::is_nothrow_constructible_v<T, U&&>) {
    this->construct(std::forward<U>(value));
  }

  template <typename... Args>
  explicit constexpr Optional(InPlace, Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>) {
    this->construct(std::forward<Args>(args)...);
  }

  constexpr Optional& operator=(const Optional& other) = default;

  constexpr Optional& operator=(Optional&&) = default;

  constexpr Optional& operator=(NullOpt) noexcept {
    reset();
    return *this;
  }

  template <
      typename U = T,
      std::enable_if_t<
          !std::same_as<std::remove_cvref_t<U>, Optional> && std::is_assignable_v<T&, U> &&
              std::is_constructible_v<T, U> && (!std::is_scalar_v<T> || !std::same_as<std::decay_t<U>, T>),
          bool> = true>
  constexpr Optional&
  operator=(U&& value) noexcept(std::is_nothrow_assignable_v<T&, U&&> && std::is_nothrow_constructible_v<T, U&&>) {
    if (this->_has) {
      this->_value = std::forward<U>(value);
    } else {
      this->construct(std::forward<U>(value));
    }
    return *this;
  }

  template <typename U = T, std::enable_if_t<std::is_swappable_v<U> && std::is_move_constructible_v<U>, bool> = true>
  friend constexpr void swap(Optional& lhs, Optional& rhs) noexcept(
      std::is_nothrow_move_constructible_v<U> && std::is_nothrow_swappable_v<U>
  ) {
    if (lhs._has && rhs._has) {
      using std::swap;
      swap(lhs._value, rhs._value);
    } else if (lhs._has) {
      rhs.construct(std::move(lhs._value));
      lhs.reset();
    } else if (rhs._has) {
      lhs.construct(std::move(rhs._value));
      rhs.reset();
    }
  }

  constexpr bool has_value() const noexcept {
    return this->_has;
  }

  constexpr explicit operator bool() const noexcept {
    return has_value();
  }

  constexpr T& operator*() & noexcept {
    return this->_value;
  }

  constexpr const T& operator*() const& noexcept {
    return this->_value;
  }

  constexpr T&& operator*() && noexcept {
    return std::move(this->_value);
  }

  constexpr const T&& operator*() const&& noexcept {
    return std::move(this->_value);
  }

  constexpr T* operator->() noexcept {
    return std::addressof(this->_value);
  }

  constexpr const T* operator->() const noexcept {
    return &this->_value;
  }

  template <typename... Args>
  constexpr T& emplace(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>) {
    if (this->_has) {
      this->reset();
    }
    this->construct(std::forward<Args>(args)...);
    return this->_value;
  }

  constexpr void reset() noexcept {
    Chain::reset(*this);
  }
};

template <typename U>
constexpr void swap(Optional<U>&, Optional<U>&) = delete;

template <class U>
constexpr std::compare_three_way_result_t<U> operator<=>(const Optional<U>& lhs, const Optional<U>& rhs) {
  return lhs && rhs ? *lhs <=> *rhs : lhs.has_value() <=> rhs.has_value();
}

template <typename T, typename Comparator>
constexpr bool compare(const Optional<T>& lhs, const Optional<T>& rhs, const Comparator& comparator) {
  if (!lhs.has_value() || !rhs.has_value()) {
    return comparator(lhs.has_value(), rhs.has_value());
  }
  return comparator(*lhs, *rhs);
}

template <typename T>
constexpr bool operator==(const Optional<T>& lhs, const Optional<T>& rhs) {
  return compare(lhs, rhs, std::equal_to<>{});
}

template <typename T>
constexpr bool operator<(const Optional<T>& lhs, const Optional<T>& rhs) {
  return compare(lhs, rhs, std::less<>{});
}

template <typename T>
constexpr bool operator!=(const Optional<T>& lhs, const Optional<T>& rhs) {
  return compare(lhs, rhs, std::not_equal_to<>{});
}

template <typename T>
constexpr bool operator<=(const Optional<T>& lhs, const Optional<T>& rhs) {
  return compare(lhs, rhs, std::less_equal<>{});
}

template <typename T>
constexpr bool operator>(const Optional<T>& lhs, const Optional<T>& rhs) {
  return compare(lhs, rhs, std::greater<>{});
}

template <typename T>
constexpr bool operator>=(const Optional<T>& lhs, const Optional<T>& rhs) {
  return compare(lhs, rhs, std::greater_equal<>{});
}

template <typename T>
Optional(T) -> Optional<T>;

} // namespace ct
