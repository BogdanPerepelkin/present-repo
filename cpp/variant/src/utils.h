#pragma once

#include <compare>
#include <concepts>
#include <cstddef>
#include <exception>
#include <string>
#include <type_traits>
#include <utility>

namespace ct {

template <typename... Args>
class Variant;

struct monostate {};

template <class T>
struct InPlaceType {
  explicit InPlaceType() = default;
};

template <std::size_t Index>
struct InPlaceIndex {
  explicit InPlaceIndex() = default;
};

class BadVariantAccess : public std::exception {
public:
  BadVariantAccess() noexcept = default;

  explicit BadVariantAccess(std::string m) noexcept
      : message(std::move(m)) {}

  const char* what() const noexcept override {
    return "BadVariantAccess error";
  }

  std::string message;
};

template <class T>
constexpr InPlaceType<T> in_place_type{};

template <std::size_t I>
constexpr InPlaceIndex<I> in_place_index{};

} // namespace ct

namespace ct::utils {

inline constexpr std::size_t variant_npos = -1;

template <class... Ts>
constexpr auto&& asVariant(Variant<Ts...>& value) {
  return value;
}

template <class... Ts>
constexpr auto&& asVariant(const Variant<Ts...>& value) {
  return value;
}

template <class... Ts>
constexpr auto&& asVariant(Variant<Ts...>&& value) {
  return value;
}

template <class... Ts>
constexpr auto&& asVariant(const Variant<Ts...>&& value) {
  return value;
}

template <class Derived>
using AsVariant = decltype(asVariant(std::declval<Derived>()));

template <typename Tar, std::size_t Cnt, class... Ts>
struct IndexByTypeImpl {
  static constexpr std::size_t value = variant_npos;
};

template <typename Tar, std::size_t Cnt>
struct IndexByTypeImpl<Tar, Cnt> {
  static constexpr std::size_t value = variant_npos;
};

template <typename Tar, std::size_t Cnt, class Cur, class... Ts>
struct IndexByTypeImpl<Tar, Cnt, Cur, Ts...> {
  static constexpr std::size_t value = std::same_as<Tar, Cur> ? Cnt : IndexByTypeImpl<Tar, Cnt + 1, Ts...>::value;
};

template <typename Tar, typename... Types>
constexpr std::size_t IndexByType = IndexByTypeImpl<std::remove_reference_t<Tar>, 0, Types...>::value;

struct UnSuccess {};

template <std::size_t I, std::size_t Cnt, typename... Types>
struct TypeByIndexImpl {
  using type = UnSuccess;
};

template <std::size_t I, std::size_t Cnt, typename Cur, typename... Types>
struct TypeByIndexImpl<I, Cnt, Cur, Types...> {
  using type = typename TypeByIndexImpl<I, Cnt + 1, Types...>::type;
};

template <std::size_t I, typename Cur, typename... Types>
struct TypeByIndexImpl<I, I, Cur, Types...> {
  using type = Cur;
};

template <std::size_t I, typename... Types>
using TypeByIndex = utils::TypeByIndexImpl<I, 0, Types...>::type;

template <std::size_t I, class... Types>
struct VariantAlternativeProxy {
  using type = void;
};

template <std::size_t I, class... Types>
struct VariantAlternativeProxy<I, Variant<Types...>> {
  using type = TypeByIndex<I, Types...>;
};

template <std::size_t I, class... Types>
struct VariantAlternativeProxy<I, const Variant<Types...>> {
  using type = const TypeByIndex<I, Types...>;
};

template <class T>
struct is_inplace_type {
  static constexpr bool value = false;
};

template <class T>
struct is_inplace_type<ct::InPlaceType<T>> {
  static constexpr bool value = true;
};

template <class I>
struct is_inplace_index {
  static constexpr bool value = false;
};

template <std::size_t I>
struct is_inplace_index<ct::InPlaceIndex<I>> {
  static constexpr bool value = true;
};

template <class T, class Ti>
concept is_correct_overload = requires (T&& t) { new Ti[1]{std::forward<T>(t)}; };

template <class T, class Ti, std::size_t I>
struct Overload {
  static constexpr std::integral_constant<std::size_t, variant_npos> overload(UnSuccess);
};

template <class T, class Ti, std::size_t I>
  requires is_correct_overload<T, Ti>
struct Overload<T, Ti, I> {
  static constexpr std::integral_constant<std::size_t, I> overload(Ti);
};

template <class T, class... TsInd>
struct AccumOverloads {};

template <class T, class... Ts, std::size_t... Indices>
struct AccumOverloads<T, std::tuple<Ts...>, std::integer_sequence<std::size_t, Indices...>>
    : Overload<T, Ts, Indices>... {
  using Overload<T, Ts, Indices>::overload...;

  static constexpr std::integral_constant<std::size_t, variant_npos> overload(...);
};

template <class T, class... Ts>
struct ChooseOverload {
  static constexpr std::size_t index =
      decltype(AccumOverloads<T, std::tuple<Ts...>, std::make_index_sequence<sizeof...(Ts)>>::overload(
          std::declval<T>()
      ))::value;

  static constexpr bool isCorrect = (index != variant_npos);
};

template <typename T, typename... Ts>
constexpr std::size_t ConvertibleIndex = ChooseOverload<T, Ts...>::index;

template <typename T, class... Ts>
using HelperTj = TypeByIndex<ConvertibleIndex<T, Ts...>, Ts...>;

template <class Variant>
struct AmountOfTypesProxy {};

template <class... Types>
struct AmountOfTypesProxy<ct::Variant<Types...>> {
  static constexpr std::size_t value = sizeof...(Types);
};

template <class Var>
constexpr std::size_t AmountOfTypes = AmountOfTypesProxy<std::remove_cvref_t<Var>>::value;

template <class T, class Type, class... Types>
struct Count {
  static constexpr std::size_t value = ((std::same_as<T, Type>) ? 1 : 0) + Count<T, Types...>::value;
};

template <class T, class Type>
struct Count<T, Type> {
  static constexpr std::size_t value = ((std::same_as<T, Type>) ? 1 : 0);
};

template <class T, class... Types>
constexpr bool IsUnique = Count<T, Types...>::value == 1;

} // namespace ct::utils

namespace ct {

using utils::variant_npos;

template <class Var>
constexpr std::size_t variant_size = utils::AmountOfTypes<Var>;

template <std::size_t I, typename Var>
using variant_alternative = utils::VariantAlternativeProxy<I, Var>::type;

constexpr std::strong_ordering operator<=>(ct::monostate, monostate) noexcept {
  return std::strong_ordering::equal;
}

} // namespace ct
