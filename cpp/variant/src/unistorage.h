#pragma once

#include "concepts.h"

#include <type_traits>
#include <utility>

namespace ct {

template <std::size_t Depth, typename... Ts>
struct UniStorage {};

template <std::size_t Depth, typename CurType, typename... Rest>
struct UniStorage<Depth, CurType, Rest...> {
  union {
    std::remove_cvref_t<CurType> _type;
    UniStorage<Depth + 1, Rest...> _rest;
  };

  constexpr UniStorage() noexcept
      : _rest() {}

  template <std::size_t I, typename... Args>
    requires (Depth < I)
  constexpr UniStorage(std::integral_constant<std::size_t, I>, Args&&... args)
      : _rest(std::integral_constant<std::size_t, I>{}, std::forward<Args>(args)...) {}

  template <std::size_t I, typename... Args>
    requires (Depth == I)
  constexpr UniStorage(std::integral_constant<std::size_t, I>, Args&&... args)
      : _type(std::forward<Args>(args)...) {}

  constexpr UniStorage(const UniStorage&) noexcept = default;

  constexpr UniStorage(const UniStorage&) noexcept
    requires (!all_trivially_copy_constructible<CurType, Rest...>)
  {}

  constexpr UniStorage(UniStorage&&) noexcept = default;

  constexpr UniStorage(UniStorage&&) noexcept
    requires (!all_trivially_move_constructible<CurType, Rest...>)
  {}

  constexpr UniStorage& operator=(const UniStorage&) noexcept = default;

  constexpr UniStorage& operator=(const UniStorage&) noexcept
    requires (!all_trivially_copy_assignable<CurType, Rest...>)
  {}

  constexpr UniStorage& operator=(UniStorage&&) noexcept = default;

  constexpr UniStorage& operator=(UniStorage&&) noexcept
    requires (!all_trivially_move_assignable<CurType, Rest...>)
  {}

  constexpr ~UniStorage() noexcept = default;

  constexpr ~UniStorage() noexcept
    requires (!all_trivially_destructible<CurType, Rest...>)
  {}

  template <std::size_t TarIndex, typename... Args>
    requires (TarIndex == Depth)
  static constexpr void construct(UniStorage* storage, Args&&... args) {
    std::construct_at(std::addressof(storage->_type), std::forward<Args>(args)...);
  }

  template <std::size_t I, typename... Args>
    requires (I > Depth)
  static constexpr void construct(UniStorage* storage, Args&&... args) {
    std::construct_at(std::addressof(storage->_rest));
    UniStorage<Depth + 1, Rest...>::template construct<I>(std::addressof(storage->_rest), std::forward<Args>(args)...);
  }

  template <std::size_t I, typename... Args>
  static constexpr void safeConstruct(UniStorage* storage, std::size_t& variantIndex, Args&&... args) {
    try {
      construct<I>(storage, std::forward<Args>(args)...);
    } catch (...) {
      variantIndex = variant_npos;
      throw;
    }
  }

  template <std::size_t I>
    requires (I == Depth)
  static constexpr void destroy(UniStorage* storage) {
    std::destroy_at(std::addressof(storage->_type));
  }

  template <std::size_t I>
    requires (I > Depth)
  static constexpr void destroy(UniStorage* storage) {
    UniStorage<Depth + 1, Rest...>::template destroy<I>(std::addressof(storage->_rest));
  }

  template <std::size_t I>
  constexpr decltype(auto) storageGet(this auto&& self) {
    if constexpr (I == Depth) {
      return std::forward_like<decltype(self)>(self._type);
    } else {
      return std::forward<decltype(self)>(self)._rest.template storageGet<I>();
    }
  }
};

template <typename... Types>
using UniStorageT = UniStorage<0, Types...>;

} // namespace ct
