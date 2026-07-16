#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <type_traits>

namespace detail {

// ========================DTOR=================================

template <typename T, bool TRIVIAL_DTOR = std::is_trivially_destructible_v<T>>
struct StorageDtorBase;

template <typename T>
struct StorageDtorBase<T, false> {
  constexpr StorageDtorBase()
      : _has(false) {}

  constexpr ~StorageDtorBase() {
    reset();
  }

  constexpr void reset() {
    if (this->_has) {
      std::destroy_at(&this->_value);
      this->_has = false;
    }
  }

  union {
    T _value;
  };

  bool _has;
};

template <typename T>
struct StorageDtorBase<T, true> {
  constexpr StorageDtorBase()
      : _has(false) {}

  constexpr void reset() {
    if (this->_has) {
      std::destroy_at(&this->_value);
      this->_has = false;
    }
  }

  union {
    T _value;
  };

  bool _has;
};

// ===========================Default====================================

template <typename T>
struct DefaultBase : StorageDtorBase<T> {
  constexpr DefaultBase()
      : StorageDtorBase<T>() {}

  template <typename... Args>
  constexpr void construct(Args&&... args) {
    std::construct_at(std::addressof(this->_value), std::forward<Args>(args)...);
    this->_has = true;
  }
};

enum class SpecialMemberStatus {
  DELETED,
  TRIVIAL,
  NON_TRIVIAL,
};

template <bool Trivial, bool Exist>
constexpr auto abs_status = []() {
  if (Trivial) {
    return SpecialMemberStatus::TRIVIAL;
  } else if (Exist) {
    return SpecialMemberStatus::NON_TRIVIAL;
  } else {
    return SpecialMemberStatus::DELETED;
  }
}();

// =======================CopyCtr==============================

template <
    typename T,
    SpecialMemberStatus STATUS = abs_status<std::is_trivially_copy_constructible_v<T>, std::is_copy_constructible_v<T>>>
struct CopyCtorBase;

template <typename T>
struct CopyCtorBase<T, SpecialMemberStatus::TRIVIAL> : DefaultBase<T> {
  using DefaultBase<T>::DefaultBase;
};

template <typename T>
struct CopyCtorBase<T, SpecialMemberStatus::NON_TRIVIAL> : DefaultBase<T> {
  using DefaultBase<T>::DefaultBase;

  // constexpr ~CopyCtorBase() = default;
  // constexpr CopyCtorBase() = default;
  constexpr CopyCtorBase(CopyCtorBase&&) = default;
  constexpr CopyCtorBase& operator=(const CopyCtorBase& other) = default;
  constexpr CopyCtorBase& operator=(CopyCtorBase&& other) = default;

  constexpr CopyCtorBase(const CopyCtorBase& other) noexcept(std::is_nothrow_copy_constructible_v<T>) {
    if (other._has) {
      this->construct(other._value);
    }
  }
};

template <typename T>
struct CopyCtorBase<T, SpecialMemberStatus::DELETED> : DefaultBase<T> {
  using DefaultBase<T>::DefaultBase;
  // constexpr ~CopyCtorBase() = default;
  // constexpr CopyCtorBase() = default;
  constexpr CopyCtorBase(CopyCtorBase&&) = default;
  constexpr CopyCtorBase& operator=(const CopyCtorBase& other) = default;
  constexpr CopyCtorBase& operator=(CopyCtorBase&& other) = default;

  constexpr CopyCtorBase(const CopyCtorBase& other) = delete;
};

// =======================MoveCtr===========================

template <
    typename T,
    SpecialMemberStatus STATUS = abs_status<std::is_trivially_move_constructible_v<T>, std::is_move_constructible_v<T>>>
struct MoveCtorBase;

template <typename T>
struct MoveCtorBase<T, SpecialMemberStatus::TRIVIAL> : CopyCtorBase<T> {
  using CopyCtorBase<T>::CopyCtorBase;
};

template <typename T>
struct MoveCtorBase<T, SpecialMemberStatus::NON_TRIVIAL> : CopyCtorBase<T> {
  using CopyCtorBase<T>::CopyCtorBase;
  // constexpr ~MoveCtorBase() = default;
  // constexpr MoveCtorBase() = default;
  constexpr MoveCtorBase(const MoveCtorBase& other) = default;
  constexpr MoveCtorBase& operator=(const MoveCtorBase& other) = default;
  constexpr MoveCtorBase& operator=(MoveCtorBase&&) = default;

  constexpr MoveCtorBase(MoveCtorBase&& other) noexcept(std::is_nothrow_move_constructible_v<T>) {
    if (other._has) {
      this->construct(std::move(other._value));
    }
  }
};

template <typename T>
struct MoveCtorBase<T, SpecialMemberStatus::DELETED> : CopyCtorBase<T> {
  using CopyCtorBase<T>::CopyCtorBase;
  // constexpr ~MoveCtorBase() = default;
  // constexpr MoveCtorBase() = default;
  constexpr MoveCtorBase(const MoveCtorBase& other) = default;
  constexpr MoveCtorBase& operator=(const MoveCtorBase& other) = default;
  constexpr MoveCtorBase& operator=(MoveCtorBase&&) = default;

  constexpr MoveCtorBase(MoveCtorBase&& other) = delete;
};

// ====================CopyAss========================

template <
    typename T,
    SpecialMemberStatus STATUS = abs_status<
        std::is_trivially_copy_assignable_v<T> && std::is_trivially_copy_constructible_v<T>,
        std::is_copy_assignable_v<T> && std::is_copy_constructible_v<T>>>
struct CopyAssignmentBase;

template <typename T>
struct CopyAssignmentBase<T, SpecialMemberStatus::TRIVIAL> : MoveCtorBase<T> {
  using MoveCtorBase<T>::MoveCtorBase;
};

template <typename T>
struct CopyAssignmentBase<T, SpecialMemberStatus::NON_TRIVIAL> : MoveCtorBase<T> {
  using MoveCtorBase<T>::MoveCtorBase;
  // constexpr ~CopyAssignmentBase() = default;
  // constexpr CopyAssignmentBase() = default;
  constexpr CopyAssignmentBase(const CopyAssignmentBase& other) = default;
  constexpr CopyAssignmentBase(CopyAssignmentBase&& other) = default;
  constexpr CopyAssignmentBase& operator=(CopyAssignmentBase&&) = default;

  constexpr CopyAssignmentBase& operator=(
      const CopyAssignmentBase& other
  ) noexcept(std::is_nothrow_copy_assignable_v<T> && std::is_nothrow_copy_constructible_v<T>) {
    if (this != &other) {
      if (other._has) {
        if (this->_has) {
          this->_value = other._value;
        } else {
          this->construct(other._value);
        }
      } else {
        this->reset();
      }
    }
    return *this;
  }
};

template <typename T>
struct CopyAssignmentBase<T, SpecialMemberStatus::DELETED> : MoveCtorBase<T> {
  using MoveCtorBase<T>::MoveCtorBase;
  // constexpr ~CopyAssignmentBase() = default;
  // constexpr CopyAssignmentBase() = default;
  constexpr CopyAssignmentBase(const CopyAssignmentBase& other) = default;
  constexpr CopyAssignmentBase(CopyAssignmentBase&& other) = default;
  constexpr CopyAssignmentBase& operator=(CopyAssignmentBase&&) = default;

  constexpr CopyAssignmentBase& operator=(const CopyAssignmentBase& other) = delete;
};

// ====================MoveAss=============================

template <
    typename T,
    SpecialMemberStatus STATUS = abs_status<
        std::is_trivially_move_assignable_v<T> && std::is_trivially_move_constructible_v<T>,
        std::is_move_assignable_v<T> && std::is_move_constructible_v<T>>>
struct MoveAssignmentBase;

template <typename T>
struct MoveAssignmentBase<T, SpecialMemberStatus::TRIVIAL> : CopyAssignmentBase<T> {
  using CopyAssignmentBase<T>::CopyAssignmentBase;
};

template <typename T>
struct MoveAssignmentBase<T, SpecialMemberStatus::NON_TRIVIAL> : CopyAssignmentBase<T> {
  using CopyAssignmentBase<T>::CopyAssignmentBase;
  // constexpr ~MoveAssignmentBase() = default;
  // constexpr MoveAssignmentBase() = default;
  constexpr MoveAssignmentBase(const MoveAssignmentBase& other) = default;
  constexpr MoveAssignmentBase(MoveAssignmentBase&& other) = default;
  constexpr MoveAssignmentBase& operator=(const MoveAssignmentBase& other) = default;

  constexpr MoveAssignmentBase& operator=(
      MoveAssignmentBase&& other
  ) noexcept(std::is_nothrow_move_assignable_v<T> && std::is_nothrow_move_constructible_v<T>) {
    if (this != &other) {
      if (other._has) {
        if (this->_has) {
          this->_value = std::move(other._value);
        } else {
          this->construct(std::move(other._value));
        }
      } else {
        this->reset();
      }
    }
    return *this;
  }
};

template <typename T>
struct MoveAssignmentBase<T, SpecialMemberStatus::DELETED> : CopyAssignmentBase<T> {
  using CopyAssignmentBase<T>::CopyAssignmentBase;
  // constexpr ~MoveAssignmentBase() = default;
  // constexpr MoveAssignmentBase() = default;
  constexpr MoveAssignmentBase(const MoveAssignmentBase& other) = default;
  constexpr MoveAssignmentBase(MoveAssignmentBase&& other) = default;
  constexpr MoveAssignmentBase& operator=(const MoveAssignmentBase& other) = default;

  constexpr MoveAssignmentBase& operator=(MoveAssignmentBase&& other) = delete;
};
} // namespace detail
