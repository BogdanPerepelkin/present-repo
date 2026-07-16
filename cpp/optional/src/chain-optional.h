#pragma once

#include <functional>
#include <type_traits>

namespace detail {

template <class Derived, class T>
struct ResetBase {
  static constexpr void reset(Derived& self) noexcept(std::is_nothrow_destructible_v<T>) {
    if (self._has) {
      std::destroy_at(std::addressof(self._value));
      self._has = false;
    }
  }
};

template <typename T, bool TRIVIAL_DTOR = std::is_trivially_destructible_v<T>>
struct StorageDtorBase;

template <typename T>
struct StorageDtorBase<T, false> : public ResetBase<StorageDtorBase<T, false>, T> {
  constexpr StorageDtorBase()
      : _has(false) {}

  constexpr ~StorageDtorBase() {
    this->reset(*this);
  }

  union {
    T _value;
  };

  bool _has;
};

template <typename T>
struct StorageDtorBase<T, true> : public ResetBase<StorageDtorBase<T, true>, T> {
  constexpr StorageDtorBase()
      : _has(false) {}

  union {
    T _value;
  };

  bool _has;
};

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

template <
    typename T,
    SpecialMemberStatus STATUS = abs_status<std::is_trivially_copy_constructible_v<T>, std::is_copy_constructible_v<T>>>
struct CopyCtorBase;

template <typename T>
struct CopyCtorBase<T, SpecialMemberStatus::TRIVIAL> : public DefaultBase<T> {
  using DefaultBase<T>::DefaultBase;
};

template <typename T>
struct CopyCtorBase<T, SpecialMemberStatus::NON_TRIVIAL> : public DefaultBase<T> {
  using DefaultBase<T>::DefaultBase;

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
struct CopyCtorBase<T, SpecialMemberStatus::DELETED> : public DefaultBase<T> {
  using DefaultBase<T>::DefaultBase;
  constexpr CopyCtorBase(CopyCtorBase&&) = default;
  constexpr CopyCtorBase& operator=(const CopyCtorBase& other) = default;
  constexpr CopyCtorBase& operator=(CopyCtorBase&& other) = default;

  constexpr CopyCtorBase(const CopyCtorBase& other) = delete;
};

template <
    typename T,
    SpecialMemberStatus STATUS = abs_status<std::is_trivially_move_constructible_v<T>, std::is_move_constructible_v<T>>>
struct MoveCtorBase;

template <typename T>
struct MoveCtorBase<T, SpecialMemberStatus::TRIVIAL> : public CopyCtorBase<T> {
  using CopyCtorBase<T>::CopyCtorBase;
};

template <typename T>
struct MoveCtorBase<T, SpecialMemberStatus::NON_TRIVIAL> : public CopyCtorBase<T> {
  using CopyCtorBase<T>::CopyCtorBase;
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
struct MoveCtorBase<T, SpecialMemberStatus::DELETED> : public CopyCtorBase<T> {
  using CopyCtorBase<T>::CopyCtorBase;
  constexpr MoveCtorBase(const MoveCtorBase& other) = default;
  constexpr MoveCtorBase& operator=(const MoveCtorBase& other) = default;
  constexpr MoveCtorBase& operator=(MoveCtorBase&&) = default;

  constexpr MoveCtorBase(MoveCtorBase&& other) = delete;
};

template <
    typename T,
    SpecialMemberStatus STATUS = abs_status<
        std::is_trivially_copy_assignable_v<T> && std::is_trivially_copy_constructible_v<T>,
        std::is_copy_assignable_v<T> && std::is_copy_constructible_v<T>>>
struct CopyAssignmentBase;

template <typename T>
struct CopyAssignmentBase<T, SpecialMemberStatus::TRIVIAL> : public MoveCtorBase<T> {
  using MoveCtorBase<T>::MoveCtorBase;
};

template <typename T>
struct CopyAssignmentBase<T, SpecialMemberStatus::NON_TRIVIAL> : public MoveCtorBase<T> {
  using MoveCtorBase<T>::MoveCtorBase;
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
        this->reset(*this);
      }
    }
    return *this;
  }
};

template <typename T>
struct CopyAssignmentBase<T, SpecialMemberStatus::DELETED> : public MoveCtorBase<T> {
  using MoveCtorBase<T>::MoveCtorBase;
  constexpr CopyAssignmentBase(const CopyAssignmentBase& other) = default;
  constexpr CopyAssignmentBase(CopyAssignmentBase&& other) = default;
  constexpr CopyAssignmentBase& operator=(CopyAssignmentBase&&) = default;

  constexpr CopyAssignmentBase& operator=(const CopyAssignmentBase& other) = delete;
};

template <
    typename T,
    SpecialMemberStatus STATUS = abs_status<
        std::is_trivially_move_assignable_v<T> && std::is_trivially_move_constructible_v<T>,
        std::is_move_assignable_v<T> && std::is_move_constructible_v<T>>>
struct MoveAssignmentBase;

template <typename T>
struct MoveAssignmentBase<T, SpecialMemberStatus::TRIVIAL> : public CopyAssignmentBase<T> {
  using CopyAssignmentBase<T>::CopyAssignmentBase;
};

template <typename T>
struct MoveAssignmentBase<T, SpecialMemberStatus::NON_TRIVIAL> : public CopyAssignmentBase<T> {
  using CopyAssignmentBase<T>::CopyAssignmentBase;
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
        this->reset(*this);
      }
    }
    return *this;
  }
};

template <typename T>
struct MoveAssignmentBase<T, SpecialMemberStatus::DELETED> : public CopyAssignmentBase<T> {
  using CopyAssignmentBase<T>::CopyAssignmentBase;
  constexpr MoveAssignmentBase(const MoveAssignmentBase& other) = default;
  constexpr MoveAssignmentBase(MoveAssignmentBase&& other) = default;
  constexpr MoveAssignmentBase& operator=(const MoveAssignmentBase& other) = default;

  constexpr MoveAssignmentBase& operator=(MoveAssignmentBase&& other) = delete;
};

} // namespace detail
