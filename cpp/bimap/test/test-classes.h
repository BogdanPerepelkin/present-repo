#pragma once

#include <cmath>
#include <functional>
#include <stdexcept>
#include <unordered_set>
#include <utility>

namespace ct_test {

class TestObject {
public:
  TestObject() = default;

  explicit TestObject(int value)
      : a(value) {}

  TestObject(const TestObject&) = delete;

  TestObject(TestObject&& other) noexcept {
    std::swap(a, other.a);
  }

  TestObject& operator=(const TestObject&) = delete;

  TestObject& operator=(TestObject&& other) noexcept {
    std::swap(a, other.a);
    return *this;
  }

  friend bool operator<(const TestObject& lhs, const TestObject& rhs) {
    return lhs.a < rhs.a;
  }

  friend bool operator==(const TestObject& lhs, const TestObject& rhs) {
    return lhs.a == rhs.a;
  }

  int a = 0;
};

class MovedFromTrackingObject {
public:
  static constexpr int MOVED_DATA = -1;
  MovedFromTrackingObject() = default;

  explicit MovedFromTrackingObject(int data) noexcept
      : data(data) {}

  MovedFromTrackingObject(MovedFromTrackingObject&& other) noexcept
      : data(std::exchange(other.data, MOVED_DATA)) {}

  MovedFromTrackingObject(const MovedFromTrackingObject& other) noexcept = default;

  MovedFromTrackingObject& operator=(const MovedFromTrackingObject& other) noexcept = default;

  MovedFromTrackingObject& operator=(MovedFromTrackingObject&& other) noexcept {
    if (this != &other) {
      data = std::exchange(other.data, MOVED_DATA);
    }
    return *this;
  }

  bool operator<(const MovedFromTrackingObject& other) const noexcept {
    return data < other.data;
  }

  bool valid_data() const noexcept {
    return data != MOVED_DATA;
  }

private:
  int data{0};
};

class VectorCompare {
public:
  using Vec = std::pair<int, int>;

  enum class DistanceType {
    euclidean,
    manhattan
  };

  explicit VectorCompare(DistanceType p = DistanceType::euclidean)
      : type(p) {}

  bool operator()(Vec lhs, Vec rhs) const {
    if (type == DistanceType::euclidean) {
      return euc(lhs) < euc(rhs);
    } else {
      return man(lhs) < man(rhs);
    }
  }

private:
  static double euc(Vec x) {
    return sqrt((x.first * x.first) + (x.second * x.second));
  }

  static double man(Vec x) {
    return abs(x.first) + abs(x.second);
  }

  DistanceType type;
};

class NonDefaultConstructible {
public:
  NonDefaultConstructible() = delete;

  explicit NonDefaultConstructible(int value)
      : a(value) {}

  NonDefaultConstructible(const NonDefaultConstructible&) = default;
  NonDefaultConstructible& operator=(const NonDefaultConstructible&) = default;

  friend bool operator<(const NonDefaultConstructible& lhs, const NonDefaultConstructible& rhs) {
    return lhs.a < rhs.a;
  }

  friend bool operator==(const NonDefaultConstructible& lhs, const NonDefaultConstructible& rhs) {
    return lhs.a == rhs.a;
  }

private:
  int a;
};

class NonCopyAssignable {
public:
  NonCopyAssignable()
      : a(0) {}

  explicit NonCopyAssignable(int value)
      : a(value) {}

  NonCopyAssignable(const NonCopyAssignable&) = default;

  NonCopyAssignable& operator=(const NonCopyAssignable&) = delete;

  friend bool operator<(const NonCopyAssignable& lhs, const NonCopyAssignable& rhs) {
    return lhs.a < rhs.a;
  }

  friend bool operator==(const NonCopyAssignable& lhs, const NonCopyAssignable& rhs) {
    return lhs.a == rhs.a;
  }

private:
  int a;
};

class AddressCheckingObject {
private:
  static std::unordered_set<const AddressCheckingObject*> addresses;

  void add_instance() const;
  void remove_instance(bool nothrow = false) const;
  void assert_exists() const;

  int value = 0;

  static size_t copy_throw_countdown;
  static void process_copying();

public:
  static void expect_no_instances();

  static void set_copy_throw_countdown(size_t new_countdown);

  operator int() const;

  AddressCheckingObject();
  AddressCheckingObject(int value);

  AddressCheckingObject(const AddressCheckingObject& other);
  AddressCheckingObject& operator=(const AddressCheckingObject& other);

  ~AddressCheckingObject() noexcept(false);
};

class StateComparator {
public:
  explicit StateComparator(bool flag = false)
      : is_inverted(flag) {}

  bool operator()(int lhs, int rhs) const {
    if (is_inverted) {
      return rhs < lhs;
    } else {
      return lhs < rhs;
    }
  }

private:
  bool is_inverted;
};

class NonCopyableComparator : public std::less<> {
public:
  NonCopyableComparator() = default;

  NonCopyableComparator(const NonCopyableComparator&) = delete;
  NonCopyableComparator(NonCopyableComparator&&) = default;

  NonCopyableComparator& operator=(const NonCopyableComparator&) = delete;
  NonCopyableComparator& operator=(NonCopyableComparator&&) = default;

  ~NonCopyableComparator() = default;
};

class NonDefaultConstructibleComparator : public std::less<> {
private:
  NonDefaultConstructibleComparator() = default;

public:
  static NonDefaultConstructibleComparator create() noexcept {
    return {};
  }

  NonDefaultConstructibleComparator(const NonDefaultConstructibleComparator&) = default;
  NonDefaultConstructibleComparator(NonDefaultConstructibleComparator&&) = default;

  NonDefaultConstructibleComparator& operator=(const NonDefaultConstructibleComparator&) = default;
  NonDefaultConstructibleComparator& operator=(NonDefaultConstructibleComparator&&) = default;

  ~NonDefaultConstructibleComparator() = default;
};

class IncomparableIntCustomComparator;

class IncomparableInt {
public:
  IncomparableInt(int value)
      : val(value) {}

private:
  int val;

  friend class IncomparableIntCustomComparator;
};

class IncomparableIntCustomComparator {
public:
  bool operator()(const IncomparableInt& lhs, const IncomparableInt& rhs) const {
    return lhs.val < rhs.val;
  }
};

class ExpiringComparator {
public:
  ExpiringComparator() = default;

  ExpiringComparator(const ExpiringComparator&) = default;

  ExpiringComparator(ExpiringComparator&& other) noexcept
      : has_expired(std::exchange(other.has_expired, true)) {}

  ExpiringComparator& operator=(const ExpiringComparator&) = default;

  ExpiringComparator& operator=(ExpiringComparator&& other) noexcept {
    if (this != &other) {
      has_expired = std::exchange(other.has_expired, true);
    }
    return *this;
  }

  ~ExpiringComparator() = default;

  template <typename L, typename R>
  bool operator()(L&& left, R&& right) const {
    if (has_expired) {
      throw std::runtime_error("Attempt to call an expired comparator");
    }
    return std::less<>()(std::forward<L>(left), std::forward<R>(right));
  }

private:
  bool has_expired = false;
};

class AlwaysThrowingComparator {
public:
  AlwaysThrowingComparator() = default;

  template <typename L, typename R>
  bool operator()(L&& /*left*/, R&& /*right*/) const {
    throw std::runtime_error("Attempt to call an always throwing comparator");
  }
};

class TrackingComparator {
public:
  explicit TrackingComparator(bool* called)
      : called(called) {}

  template <typename L, typename R>
  bool operator()(L&& left, R&& right) const {
    *called = true;
    return std::less<>()(std::forward<L>(left), std::forward<R>(right));
  }

private:
  bool* called;
};

} // namespace ct_test
