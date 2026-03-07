#pragma once
#include "concepts.h"
#include "get.h"
#include "unistorage.h"
#include "utils.h"
#include "visit.h"

#include <array>
#include <compare>
#include <concepts>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

namespace ct {

template <class Visitor, class... Variants>
constexpr decltype(auto) visit(Visitor&& vis, Variants&&... values) {
  if ((std::forward<Variants>(values).valueless_by_exception() || ...)) {
    throw BadVariantAccess{"BadVariantAccess: error in visit"};
  }
  using R = decltype(std::forward<Visitor>(vis)(get<0>(std::forward<Variants>(values))...));
  return visitImpl<AdditionInfo::NOP, R>(
      std::forward<Visitor>(vis),
      utils::asVariant(std::forward<Variants>(values))...
  );
}

template <class R, class Visitor, class... Variants>
constexpr R visit(Visitor&& vis, Variants&&... values) {
  return visitImpl<AdditionInfo::NOP, R>(std::forward<Visitor>(vis), std::forward<Variants>(values)...);
}

template <typename... Types>
class Variant {
public:
  constexpr ~Variant()
    requires (all_trivially_destructible<Types...>)
  = default;

  constexpr ~Variant()
    requires (!all_trivially_destructible<Types...>)
  {
    reset();
  }

  constexpr Variant() noexcept(std::is_nothrow_default_constructible_v<std::tuple_element<0, std::tuple<Types...>>>)
    requires (!std::is_default_constructible_v<std::tuple_element_t<0, std::tuple<Types...>>>)
  = delete;

  constexpr Variant() noexcept(std::is_nothrow_default_constructible_v<std::tuple_element_t<0, std::tuple<Types...>>>)
    requires (std::is_default_constructible_v<std::tuple_element_t<0, std::tuple<Types...>>>)
      : _storage(std::integral_constant<std::size_t, 0>{})
      , _index(0) {}

  constexpr Variant(const Variant& other)
    requires (!all_copy_constructible<Types...>)
  = delete;

  constexpr Variant(const Variant& other)
    requires all_trivially_copy_constructible<Types...>
  = default;

  constexpr Variant(const Variant& other)
    requires (all_copy_constructible<Types...> && !all_trivially_copy_constructible<Types...>)
  {
    if (other.valueless_by_exception()) {
      _index = variant_npos;
    } else {
      utils::visitIndices(VisitorCst{_storage}, other);
      _index = other.index();
    }
  }

  constexpr Variant(Variant&& other)
    requires (!all_move_constructible<Types...>)
  = delete;

  constexpr Variant(Variant&& other)
    requires (all_trivially_move_constructible<Types...>)
  = default;

  constexpr Variant(Variant&& other) noexcept((std::is_nothrow_move_constructible_v<Types> && ...))
    requires (all_move_constructible<Types...> && !all_trivially_move_constructible<Types...>)
  {
    if (other.valueless_by_exception()) {
      _index = variant_npos;
    } else {
      utils::visitIndices(VisitorCst{_storage}, std::move(other));
      _index = other.index();
    }
  }

  template <typename T>
    requires (convertible_constructor<Variant<Types...>, T, Types...>)
  constexpr explicit(!std::is_convertible_v<T, utils::HelperTj<T, Types...>>) Variant(
      T&& value
  ) noexcept(std::is_nothrow_constructible_v<utils::TypeByIndex<utils::ConvertibleIndex<T, Types...>, Types...>, T>)
      : _index(utils::ConvertibleIndex<T, Types...>) {
    UniStorageT<Types...>::template construct<utils::ConvertibleIndex<T, Types...>>(
        std::addressof(_storage),
        std::forward<T>(value)
    );
  }

  template <class T, class... Args>
    requires (std::is_constructible_v<T, Args...> && utils::IsUnique<T, Types...>)
  constexpr explicit Variant(InPlaceType<T> /*unused*/, Args&&... args) noexcept(
      std::is_nothrow_constructible_v<T, Args...>
  )
      : _index(utils::ConvertibleIndex<T, Types...>) {
    UniStorageT<Types...>::template construct<utils::ConvertibleIndex<T, Types...>>(
        std::addressof(_storage),
        std::forward<Args>(args)...
    );
  }

  template <class T, class U, class... Args>
    requires (std::is_constructible_v<T, std::initializer_list<U>&, Args...>, utils::IsUnique<T, Types...>)
  constexpr explicit Variant(InPlaceType<T> /*unused*/, std::initializer_list<U> il, Args&&... args)
      : _index(utils::IndexByType<T, Types...>) {
    std::construct_at(
        std::addressof(_storage),
        std::integral_constant<std::size_t, utils::IndexByType<T, Types...>>{},
        il,
        std::forward<Args>(args)...
    );
  }

  template <std::size_t I, class... Args>
    requires (std::is_constructible_v<utils::TypeByIndex<I, Types...>, Args...> && I < sizeof...(Types))
  constexpr explicit Variant(InPlaceIndex<I> /*unused*/, Args&&... args) {
    std::construct_at(std::addressof(_storage), std::integral_constant<std::size_t, I>{}, std::forward<Args>(args)...);
    _index = I;
  }

  template <std::size_t I, class U, class... Args>
    requires (
        std::is_constructible_v<utils::TypeByIndex<I, Types...>, std::initializer_list<U>&, Args...> &&
        I < sizeof...(Types)
    )
  constexpr explicit Variant(InPlaceIndex<I> /*unused*/, std::initializer_list<U> il, Args&&... args) {
    std::construct_at(
        std::addressof(_storage),
        std::integral_constant<std::size_t, I>{},
        il,
        std::forward<Args>(args)...
    );
    _index = I;
  }

  constexpr Variant& operator=(const Variant& rhs)
    requires (!all_copy_assignable<Types...>)
  = delete;

  constexpr Variant& operator=(const Variant& rhs)
    requires (all_trivially_copy_assignable<Types...>)
  = default;

  constexpr Variant& operator=(const Variant& rhs)
    requires (all_copy_assignable<Types...> && !all_trivially_copy_assignable<Types...>)
  {
    if (valueless_by_exception() && rhs.valueless_by_exception()) {
    } else if (rhs.valueless_by_exception() && !valueless_by_exception()) {
      reset();
    } else if (index() == rhs.index()) {
      assignSameType(rhs);
    } else {
      utils::visitIndices(
          [&]<std::size_t I0>(std::integral_constant<std::size_t, I0>, auto&& other) {
            if constexpr (operator_copy_assign_guaranteed_case<std::remove_cvref_t<decltype(other)>>) {
              this->emplace<I0>(std::forward<decltype(other)>(other));
            } else {
              this->operator=(Variant(rhs));
            }
          },
          rhs
      );
    }
    return *this;
  }

  constexpr Variant& operator=(Variant&& rhs) noexcept
    requires (!all_move_assignable<Types...>)
  = delete;

  constexpr Variant& operator=(Variant&& rhs) noexcept
    requires (all_trivially_move_assignable<Types...>)
  = default;

  constexpr Variant& operator=(Variant&& rhs) noexcept(
      ((std::is_nothrow_move_constructible_v<Types> && std::is_nothrow_move_assignable_v<Types>) && ...)
  )
    requires (all_move_constructible<Types...> && !all_trivially_move_assignable<Types...>)
  {
    if (valueless_by_exception() && rhs.valueless_by_exception()) {
    } else if (rhs.valueless_by_exception() && !valueless_by_exception()) {
      reset();
    } else if (index() == rhs.index()) {
      assignSameType(std::move(rhs));
    } else {
      utils::visitIndices(VisitorSafeCst{this}, std::move(rhs));
      _index = rhs.index();
    }
    return *this;
  }

  template <class T>
    requires (convertible_assign<Variant, T, Types...>)
  Variant& operator=(T&& t) noexcept(
      std::is_nothrow_assignable_v<utils::HelperTj<T, Types...>&, T> &&
      std::is_nothrow_constructible_v<utils::HelperTj<T, Types...>, T>
  ) {
    using RType = utils::HelperTj<T, Types...>;
    constexpr std::size_t I = utils::IndexByType<RType, Types...>;
    if (valueless_by_exception()) {
      UniStorageT<Types...>::template construct<I>(std::addressof(_storage), std::forward<T>(t));
    } else if (index() == I) {
      visit(
          [&](auto&& selected) {
            if constexpr (std::is_assignable_v<decltype(selected), T>) {
              std::forward<decltype(selected)>(selected) = std::forward<T>(t);
            }
          },
          *this
      );
    } else {
      if constexpr (operator_converting_assign_guaranteed_case<RType, T>) {
        this->emplace<I>(std::forward<T>(t));
      } else {
        this->emplace<I>(utils::HelperTj<T, Types...>(std::forward<T>(t)));
      }
    }
    return *this;
  }

  constexpr std::size_t index() const noexcept {
    return _index;
  }

  constexpr bool valueless_by_exception() const noexcept {
    return index() == variant_npos;
  }

  template <class T, class... Args>
    requires (std::is_constructible_v<T, Args...> && utils::IsUnique<T, Types...>)
  constexpr T& emplace(Args&&... args) {
    return emplace<utils::IndexByType<T, Types...>>(std::forward<Args>(args)...);
  }

  template <class T, class U, class... Args>
    requires (std::is_constructible_v<T, std::initializer_list<U>&, Args...> && utils::IsUnique<T, Types...>)
  constexpr T& emplace(std::initializer_list<U> il, Args&&... args) {
    return emplace<utils::IndexByType<T, Types...>>(il, std::forward<Args>(args)...);
  }

  template <std::size_t I, class... Args>
    requires (std::is_constructible_v<utils::TypeByIndex<I, Types...>, Args...>)
  constexpr utils::TypeByIndex<I, Types...>& emplace(Args&&... args) {
    static_assert(I <= sizeof...(Types), "my_emplace_compile_err : I > sizeof...(Types)");
    reset();
    UniStorageT<Types...>::template safeConstruct<I>(std::addressof(_storage), _index, std::forward<Args>(args)...);
    _index = I;
    return get<I>(*this);
  }

  template <std::size_t I, class U, class... Args>
    requires (std::is_constructible_v<utils::TypeByIndex<I, Types...>, std::initializer_list<U>&, Args...>)
  constexpr utils::TypeByIndex<I, Types...>& emplace(std::initializer_list<U> il, Args&&... args) {
    static_assert(I <= sizeof...(Types), "my_emplace_compile_err : I > sizeof...(Types)");
    reset();
    UniStorageT<Types...>::template safeConstruct<I>(std::addressof(_storage), _index, il, std::forward<Args>(args)...);
    _index = I;
    return get<I>(*this);
  }

  constexpr void swap(Variant& rhs) noexcept(all_nothrow_swapable<Types...>) {
    if (valueless_by_exception() && rhs.valueless_by_exception()) {
      return;
    }
    if (valueless_by_exception() && !rhs.valueless_by_exception()) {
      utils::visitIndices(
          [&]<std::size_t I0>(std::integral_constant<std::size_t, I0>, auto&& selected) {
            UniStorageT<Types...>::template construct<I0>(std::addressof(_storage), std::move(selected));
            UniStorageT<Types...>::template destroy<I0>(std::addressof(rhs._storage));
          },
          rhs
      );
    } else if (!valueless_by_exception() && rhs.valueless_by_exception()) {
      (rhs).swap(*this);
      return;
    } else if (index() == rhs.index()) {
      visit(
          [](auto&& fst, auto&& sec) {
            if constexpr (std::same_as<decltype(fst), decltype(sec)>) {
              using std::swap;
              swap(fst, sec);
            }
          },
          *this,
          rhs
      );
    } else {
      utils::visitIndices(
          [&]<std::size_t I0, std::size_t I1>(
              std::integral_constant<std::size_t, I0>,
              std::integral_constant<std::size_t, I1>,
              auto&& fst,
              auto&& sec
          ) {
            auto buf = std::move(fst);
            UniStorageT<Types...>::template destroy<I0>(std::addressof(_storage));
            if constexpr (std::is_constructible_v<utils::TypeByIndex<I1, Types...>, decltype(std::move(sec))>) {
              UniStorageT<Types...>::template construct<I1>(std::addressof(_storage), std::move(sec));
            }
            UniStorageT<Types...>::template destroy<I1>(std::addressof(rhs._storage));
            if constexpr (std::is_constructible_v<utils::TypeByIndex<I0, Types...>, decltype(std::move(buf))>) {
              UniStorageT<Types...>::template construct<I0>(std::addressof(rhs._storage), std::move(buf));
            }
          },
          *this,
          rhs
      );
    }
    std::swap(_index, rhs._index);
  }

  template <std::size_t I, class... Ts>
  friend constexpr decltype(auto) get(Variant<Ts...>&);
  template <std::size_t I, class... Ts>
  friend constexpr decltype(auto) get(Variant<Ts...>&&);
  template <std::size_t I, class... Ts>
  friend constexpr decltype(auto) get(const Variant<Ts...>&);
  template <std::size_t I, class... Ts>
  friend constexpr decltype(auto) get(const Variant<Ts...>&&);

  template <class T, class... Ts>
  friend constexpr decltype(auto) get(Variant<Ts...>&);
  template <class T, class... Ts>
  friend constexpr decltype(auto) get(Variant<Ts...>&&);
  template <class T, class... Ts>
  friend constexpr decltype(auto) get(const Variant<Ts...>&);
  template <class T, class... Ts>
  friend constexpr decltype(auto) get(const Variant<Ts...>&&);

  template <std::size_t I, class... Ts>
  friend constexpr std::add_pointer_t<utils::TypeByIndex<I, Ts...>> get_if(Variant<Ts...>* pv) noexcept;
  template <std::size_t I, class... Ts>
  friend constexpr std::add_pointer_t<const utils::TypeByIndex<I, Ts...>> get_if(const Variant<Ts...>* pv) noexcept;
  template <class T, class... Ts>
  friend constexpr std::add_pointer_t<T> get_if(Variant<Ts...>* pv) noexcept;
  template <class T, class... Ts>
  friend constexpr std::add_pointer_t<const T> get_if(const Variant<Ts...>* pv) noexcept;

private:
  UniStorageT<Types...> _storage;
  std::size_t _index = variant_npos;

  constexpr void reset() {
    if (index() != variant_npos) {
      utils::visitIndices(
          [&]<std::size_t I0>(std::integral_constant<std::size_t, I0>, auto&&) {
            UniStorageT<Types...>::template destroy<I0>(std::addressof(_storage));
          },
          *this
      );
    }
    _index = variant_npos;
  }

  template <std::size_t I>
  constexpr decltype(auto) getImpl(this auto&& self) {
    static_assert(I < sizeof...(Types), "[my] getImpl I >= sizof(types)");
    if (self.index() != I) {
      throw BadVariantAccess{};
    }
    return std::forward<decltype(self)>(self)._storage.template storageGet<I>();
  }

  template <std::size_t I>
  constexpr utils::TypeByIndex<I, Types...>* getIfImpl(this auto&& self) {
    static_assert(I < sizeof...(Types), "[my] getIfImpl I >= sizof(types)");
    if (self.index() != I) {
      return nullptr;
    }
    return const_cast<utils::TypeByIndex<I, Types...>*>(
        std::addressof(std::forward<decltype(self)>(self)._storage.template storageGet<I>())
    );
  }

  template <class RhsT>
  constexpr void assignSameType(RhsT&& rhs) {
    utils::visitIndices(
        [&]<std::size_t I0, std::size_t I1>(
            std::integral_constant<std::size_t, I0>,
            std::integral_constant<std::size_t, I1>,
            auto&& fst,
            auto&& sec
        ) {
          if constexpr (std::is_assignable_v<decltype(fst), decltype(sec)>) {
            std::forward<decltype(fst)>(fst) = std::forward<decltype(sec)>(sec);
          }
        },
        *this,
        std::forward<RhsT>(rhs)
    );
  }

  struct VisitorCst {
    UniStorageT<Types...>& _storage;

    template <std::size_t I0>
    constexpr void operator()(std::integral_constant<std::size_t, I0>, auto&& selected) {
      std::construct_at(
          std::addressof(_storage),
          std::integral_constant<std::size_t, I0>{},
          std::forward<decltype(selected)>(selected)
      );
    }
  };

  struct VisitorSafeCst {
    Variant<Types...>* variant;

    template <std::size_t I0>
    void operator()(std::integral_constant<std::size_t, I0>, auto&& selected) {
      variant->reset();
      UniStorageT<Types...>::template safeConstruct<I0>(
          std::addressof(variant->_storage),
          variant->_index,
          std::forward<decltype(selected)>(selected)
      );
    }
  };

  friend VisitorSafeCst;
};

template <typename R, class Op, class... Ts>
R genericCompareVisitor(Op op, const ct::Variant<Ts...>& lhs, const ct::Variant<Ts...>& rhs) {
  R result = utils::visitIndices(
      [&]<std::size_t I0, std::size_t I1, typename Type0, typename Type1>(
          std::integral_constant<std::size_t, I0>,
          std::integral_constant<std::size_t, I1>,
          Type0&& fst,
          Type1&& sec
      ) -> R {
        if constexpr (I0 == I1) {
          return op(std::forward<Type0>(fst), std::forward<Type1>(sec));
        } else {
          return op(I0, I1);
        }
      },
      lhs,
      rhs
  );
  return result;
}

template <class Op, class... Ts>
constexpr bool compare(Op op, const ct::Variant<Ts...>& lhs, const ct::Variant<Ts...>& rhs) {
  bool empty0 = lhs.valueless_by_exception();
  bool empty1 = rhs.valueless_by_exception();

  if (empty0 && empty1) {
    return op(0, 0);
  }
  if (empty0) {
    return op(-1, 0);
  }
  if (empty1) {
    return op(1, 0);
  }

  return genericCompareVisitor<bool>(op, lhs, rhs);
}

template <class... Ts>
  requires (std::three_way_comparable<Ts> && ...)
constexpr std::common_comparison_category_t<std::compare_three_way_result_t<Ts>...>
operator<=>(const ct::Variant<Ts...>& lhs, const ct::Variant<Ts...>& rhs) {
  using ComparisonCategory = std::common_comparison_category_t<std::compare_three_way_result_t<Ts>...>;
  bool empty0 = lhs.valueless_by_exception();
  bool empty1 = rhs.valueless_by_exception();

  if (empty0 && empty1) {
    return ComparisonCategory::equivalent;
  }
  if (empty0) {
    return ComparisonCategory::less;
  }
  if (empty1) {
    return ComparisonCategory::greater;
  }

  return genericCompareVisitor<ComparisonCategory>(std::compare_three_way{}, lhs, rhs);
}

template <class... Types>
constexpr bool operator==(const ct::Variant<Types...>& lhs, const ct::Variant<Types...>& rhs) {
  return compare(std::equal_to{}, lhs, rhs);
}

template <class... Types>
constexpr bool operator!=(const ct::Variant<Types...>& lhs, const ct::Variant<Types...>& rhs) {
  return compare(std::not_equal_to{}, lhs, rhs);
}

template <class... Types>
constexpr bool operator<(const ct::Variant<Types...>& lhs, const ct::Variant<Types...>& rhs) {
  return compare(std::less{}, lhs, rhs);
}

template <class... Types>
constexpr bool operator>(const ct::Variant<Types...>& lhs, const ct::Variant<Types...>& rhs) {
  return compare(std::greater{}, lhs, rhs);
}

template <class... Types>
constexpr bool operator<=(const ct::Variant<Types...>& lhs, const ct::Variant<Types...>& rhs) {
  return compare(std::less_equal{}, lhs, rhs);
}

template <class... Types>
constexpr bool operator>=(const ct::Variant<Types...>& lhs, const ct::Variant<Types...>& rhs) {
  return compare(std::greater_equal{}, lhs, rhs);
}

template <class T, class... Ts>
constexpr bool holds_alternative(const Variant<Ts...>& v) noexcept {
  return v.index() == utils::IndexByType<T, Ts...>;
}

template <class... Ts>
void swap(Variant<Ts...>& lhs, Variant<Ts...>& rhs) noexcept(all_nothrow_swapable<Ts...>) {
  lhs.swap(rhs);
}

} // namespace ct
