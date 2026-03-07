#pragma once

#include "utils.h"

#include <concepts>
#include <type_traits>

template <class... Ts>
concept all_trivially_destructible = (std::is_trivially_destructible_v<Ts> && ...);

template <class... Ts>
concept all_copy_constructible = (std::is_copy_constructible_v<Ts> && ...);

template <class... Ts>
concept all_trivially_copy_constructible = (std::is_trivially_copy_constructible_v<Ts> && ...);

template <class... Ts>
concept all_move_constructible = (std::is_move_constructible_v<Ts> && ...);

template <class... Ts>
concept all_trivially_move_constructible = (std::is_trivially_move_constructible_v<Ts> && ...);

template <class... Ts>
concept all_trivially_default_constructible = (std::is_trivially_default_constructible_v<Ts> && ...);

template <class... Ts>
concept all_copy_assignable = all_copy_constructible<Ts...> && (std::is_copy_assignable_v<Ts> && ...);

template <class... Ts>
concept all_trivially_copy_assignable =
    all_trivially_copy_constructible<Ts...> && (std::is_trivially_copy_assignable_v<Ts> && ...) &&
    all_trivially_destructible<Ts...>;

template <class Type>
concept operator_copy_assign_guaranteed_case =
    std::is_nothrow_copy_constructible_v<Type> || !std::is_nothrow_move_constructible_v<Type>;

template <class RType, class T>
concept operator_converting_assign_guaranteed_case =
    std::is_nothrow_constructible_v<RType, T> || !std::is_nothrow_move_constructible_v<RType>;

template <class... Ts>
concept all_move_assignable = all_move_constructible<Ts...> && (std::is_move_assignable_v<Ts> && ...);

template <class... Ts>
concept all_trivially_move_assignable =
    all_trivially_move_constructible<Ts...> && (std::is_trivially_move_assignable_v<Ts> && ...) &&
    all_trivially_destructible<Ts...>;

template <class... Ts>
concept all_nothrow_swapable =
    (std::is_nothrow_move_constructible_v<Ts> && ...) && (std::is_nothrow_swappable_v<Ts> && ...);

template <typename Var, typename T, class... Ts>
concept convertible_constructor =
    sizeof...(Ts) > 0 && !std::same_as<std::remove_cvref_t<T>, Var> &&
    !ct::utils::is_inplace_type<std::remove_cvref_t<T>>::value &&
    !ct::utils::is_inplace_index<std::remove_cvref_t<T>>::value &&
    std::is_constructible_v<ct::utils::HelperTj<T, Ts...>, T> && ct::utils::ChooseOverload<T, Ts...>::isCorrect;

template <typename Var, typename T, class... Ts>
concept convertible_assign =
    !std::same_as<std::remove_cvref_t<T>, Var> && std::is_assignable_v<ct::utils::HelperTj<T, Ts...>&, T> &&
    std::is_constructible_v<ct::utils::HelperTj<T, Ts...>, T> && std::is_constructible_v<T, T> &&
    ct::utils::ChooseOverload<T, Ts...>::isCorrect;
