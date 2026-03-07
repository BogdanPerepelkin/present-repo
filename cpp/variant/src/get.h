#pragma once

#include "utils.h"

#include <cstddef>
#include <type_traits>
#include <utility>

namespace ct {

template <std::size_t I, class... Ts>
constexpr decltype(auto) get(Variant<Ts...>& v) {
  return v.template getImpl<I>();
}

template <std::size_t I, class... Ts>
constexpr decltype(auto) get(Variant<Ts...>&& v) {
  return std::move(v).template getImpl<I>();
}

template <std::size_t I, class... Ts>
constexpr decltype(auto) get(const Variant<Ts...>& v) {
  return v.template getImpl<I>();
}

template <std::size_t I, class... Ts>
constexpr decltype(auto) get(const Variant<Ts...>&& v) {
  return std::move(v).template getImpl<I>();
}

template <class T, class... Ts>
constexpr decltype(auto) get(Variant<Ts...>& v) {
  return v.template getImpl<utils::IndexByType<T, Ts...>>();
}

template <class T, class... Ts>
constexpr decltype(auto) get(const Variant<Ts...>& v) {
  return v.template getImpl<utils::IndexByType<T, Ts...>>();
}

template <class T, class... Ts>
constexpr decltype(auto) get(Variant<Ts...>&& v) {
  return std::move(v).template getImpl<utils::IndexByType<T, Ts...>>();
}

template <class T, class... Ts>
constexpr decltype(auto) get(const Variant<Ts...>&& v) {
  return std::move(v).template getImpl<utils::IndexByType<T, Ts...>>();
}

template <std::size_t I, class... Types>
constexpr std::add_pointer_t<utils::TypeByIndex<I, Types...>> get_if(Variant<Types...>* pv) noexcept {
  return pv->template getIfImpl<I>();
}

template <std::size_t I, class... Types>
constexpr std::add_pointer_t<const utils::TypeByIndex<I, Types...>> get_if(const Variant<Types...>* pv) noexcept {
  return pv->template getIfImpl<I>();
}

template <class T, class... Types>
constexpr std::add_pointer_t<T> get_if(Variant<Types...>* pv) noexcept {
  return pv->template getIfImpl<utils::IndexByType<T, Types...>>();
}

template <class T, class... Types>
constexpr std::add_pointer_t<const T> get_if(const Variant<Types...>* pv) noexcept {
  return pv->template getIfImpl<utils::IndexByType<T, Types...>>();
}

} // namespace ct
