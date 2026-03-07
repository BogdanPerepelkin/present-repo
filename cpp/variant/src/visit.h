#pragma once

#include "utils.h"

#include <array>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

namespace ct {

enum AdditionInfo : std::size_t {
  NOP,
  Indices,
};

template <class V>
constexpr decltype(auto) asVariant(V&& v) {
  return std::forward<V>(v);
}

template <class V>
using AsVariantT = decltype(asVariant(std::declval<V>()));

template <class... VariantsBranch>
struct TableFn {};

template <typename FnP, class Variant, class... Variants>
struct TableFn<FnP, Variant, Variants...> {
  std::array<TableFn<FnP, Variants...>, utils::AmountOfTypes<Variant>> _subTable;

  template <typename... Indices>
  constexpr FnP get(std::size_t curInd, Indices... restInd) const noexcept {
    return _subTable[curInd].get(restInd...);
  }
};

template <typename FnP>
struct TableFn<FnP> {
  FnP _functionPtr;

  constexpr FnP get() const noexcept {
    return _functionPtr;
  }
};

template <AdditionInfo AddInf, typename RetType, std::size_t... Indices, class Visitor, class... Variants>
constexpr RetType lastCall(Visitor&& vis, Variants&&... variants) {
  if constexpr (AddInf) {
    return std::invoke(
        std::forward<Visitor>(vis),
        std::integral_constant<std::size_t, Indices>{}...,
        get<Indices>(std::forward<Variants>(variants))...
    );
  } else {
    return std::invoke(std::forward<Visitor>(vis), get<Indices>(std::forward<Variants>(variants))...);
  }
}

template <AdditionInfo AddInf, typename RetType, typename FnP, std::size_t... Branch>
constexpr TableFn<FnP> compileInit(std::index_sequence<Branch...> /*unused*/) {
  return TableFn<FnP>{&lastCall<AddInf, RetType, Branch...>};
}

template <AdditionInfo AddInf, typename RetType, typename FnP, class Variant, class... Variants, std::size_t... Branch>
constexpr TableFn<FnP, Variant, Variants...> compileInit(std::index_sequence<Branch...> /*unused*/) {
  return TableFn<FnP, Variant, Variants...>{[&]<std::size_t... Is>(std::index_sequence<Is...> /*unused*/) {
    return std::array<TableFn<FnP, Variants...>, utils::AmountOfTypes<Variant>>{
        compileInit<AddInf, RetType, FnP, Variants...>(std::index_sequence<Branch..., Is>{})...
    };
  }(std::make_index_sequence<utils::AmountOfTypes<Variant>>{})};
}

template <AdditionInfo AddInf, typename RetType, class Visitor, class... Variants>
constexpr RetType visitImpl(Visitor&& vis, Variants&&... values) {
  using FnP = RetType (*)(Visitor&&, Variants&&...);
  static constexpr TableFn<FnP, Variants...> table =
      compileInit<AddInf, RetType, FnP, Variants...>(std::index_sequence<>{});
  return (*table.get(asVariant(values).index()...))(
      std::forward<Visitor>(vis),
      std::forward<AsVariantT<Variants>>(values)...
  );
}

namespace utils {

template <class Visitor, class... Variants>
constexpr decltype(auto) visitIndices(Visitor&& vis, Variants&&... variants) {
  return [&]<std::size_t... Is>(const std::index_sequence<Is...>&) -> decltype(auto) {
    using R = decltype(std::invoke(
        std::forward<Visitor>(vis),
        std::integral_constant<std::size_t, Is>{}...,
        get<0>(std::forward<Variants>(variants))...
    ));
    return visitImpl<AdditionInfo::Indices, R>(std::forward<Visitor>(vis), std::forward<Variants>(variants)...);
  }(std::make_index_sequence<sizeof...(Variants)>{});
}

} // namespace utils

} // namespace ct
