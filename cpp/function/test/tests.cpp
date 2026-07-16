#include "function.h"

#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <exception>
#include <functional>
#include <stdexcept>
#include <utility>

namespace ct_test {

using ct::Function;

TEST(FunctionTest, DefaultCtor) {
  Function<void()> x;
  Function<void(int, int, int)> y;
  Function<double && (float&, const int&, int)> z;
}

TEST(FunctionTest, EmptyConversionToBool) {
  Function<void()> x;
  EXPECT_FALSE(static_cast<bool>(x));
}

TEST(FunctionTest, EmptyCall) {
  Function<void()> x;
  EXPECT_THROW(x(), ct::BadFunctionCall);
  try {
    x();
  } catch (const std::exception& e) {
    std::exception basic_exception;
    EXPECT_NE(e.what(), basic_exception.what());
  }
}

TEST(FunctionTest, EmptyCopyMove) {
  Function<void()> x;

  Function<void()> y = x;
  EXPECT_FALSE(static_cast<bool>(y));

  Function<void()> z = std::move(x);
  EXPECT_FALSE(static_cast<bool>(z));

  z = y;
  EXPECT_FALSE(static_cast<bool>(z));

  y = std::move(z);
  EXPECT_FALSE(static_cast<bool>(y));
}

TEST(FunctionTest, lambda) {
  Function<int()> f = [] {
    return 42;
  };
  EXPECT_EQ(f(), 42);
}

TEST(FunctionTest, PointerToFunction) {
  Function<int()> f = +[] {
    return 42;
  };
  EXPECT_EQ(f(), 42);
}

TEST(FunctionTest, CopyCtor) {
  Function<int()> f = [] {
    return 42;
  };
  Function<int()> g = f;
  EXPECT_EQ(f(), 42);
  EXPECT_EQ(g(), 42);
}

namespace {

template <typename T, typename F>
bool is_small(const F& f) {
  return std::less_equal<>{}(static_cast<const void*>(&f), static_cast<const void*>(f.template target<T>())) &&
         std::less<>{}(static_cast<const void*>(f.template target<T>()), static_cast<const void*>(&f + 1));
}

struct SmallFunc {
  explicit SmallFunc(int value) noexcept
      : value(value) {}

  SmallFunc(const SmallFunc&) = default;
  SmallFunc& operator=(const SmallFunc&) = default;

  SmallFunc(SmallFunc&& other) noexcept {
    *this = std::move(other);
  }

  SmallFunc& operator=(SmallFunc&& other) noexcept {
    value = std::exchange(other.value, 0);
    return *this;
  }

  int operator()() const {
    return value;
  }

  int get_value() const {
    return value;
  }

  int set_value(int new_value) {
    return value = new_value;
  }

private:
  int value;
};

} // namespace

TEST(FunctionTest, EmptyTarget) {
  Function<int()> f;
  EXPECT_EQ(f.target<SmallFunc>(), nullptr);
  EXPECT_EQ(std::as_const(f).target<SmallFunc>(), nullptr);
}

TEST(FunctionTest, SmallFunc) {
  Function<int()> f = SmallFunc(42);
  EXPECT_EQ(f(), 42);
  EXPECT_TRUE(is_small<SmallFunc>(f));
}

TEST(FunctionTest, SmallFuncConst) {
  const Function<int()> f = SmallFunc(42);
  EXPECT_EQ(f(), 42);
  EXPECT_TRUE(is_small<SmallFunc>(f));
}

TEST(FunctionTest, SmallFuncTarget) {
  Function<int()> f = SmallFunc(42);
  EXPECT_EQ(f.target<SmallFunc>()->get_value(), 42);
  EXPECT_EQ(std::as_const(f).target<SmallFunc>()->get_value(), 42);
}

TEST(FunctionTest, SmallFuncCopyCtor) {
  Function<int()> f = SmallFunc(42);
  Function<int()> g = f;
  EXPECT_EQ(f(), 42);
  EXPECT_EQ(g(), 42);
}

TEST(FunctionTest, SmallFuncCopyCtorIndependence) {
  Function<int()> f = SmallFunc(42);
  Function<int()> g = f;
  f.target<SmallFunc>()->set_value(55);
  EXPECT_EQ(f(), 55);
  EXPECT_EQ(g(), 42);
}

TEST(FunctionTest, SmallFuncMoveCtor) {
  Function<int()> f = SmallFunc(42);
  Function<int()> g = std::move(f);
  EXPECT_EQ(g(), 42);
  EXPECT_FALSE(f);
}

TEST(FunctionTest, SmallFuncCopyAssignment) {
  Function<int()> f = SmallFunc(42);
  Function<int()> g = SmallFunc(55);
  g = f;
  EXPECT_EQ(f(), 42);
  EXPECT_EQ(g(), 42);
}

TEST(FunctionTest, SmallFuncCopyAssignmentToEmpty) {
  Function<int()> f = SmallFunc(42);
  Function<int()> g;
  g = f;
  EXPECT_EQ(f(), 42);
  EXPECT_EQ(g(), 42);
}

TEST(FunctionTest, SmallFuncCopyAssignmentFromEmpty) {
  Function<int()> f;
  Function<int()> g = SmallFunc(55);
  g = f;
  EXPECT_FALSE(f);
  EXPECT_FALSE(g);
}

TEST(FunctionTest, SmallFuncCopyAssignmentIndependence) {
  Function<int()> f = SmallFunc(42);
  Function<int()> g;
  g = f;
  f.target<SmallFunc>()->set_value(55);
  EXPECT_EQ(f(), 55);
  EXPECT_EQ(g(), 42);
}

TEST(FunctionTest, SmallFuncMoveAssignment) {
  Function<int()> f = SmallFunc(42);
  Function<int()> g = SmallFunc(55);
  g = std::move(f);
  EXPECT_EQ(g(), 42);
  EXPECT_TRUE(!f || f() == 55);
}

TEST(FunctionTest, SmallFuncMoveAssignmentToEmpty) {
  Function<int()> f = SmallFunc(42);
  Function<int()> g;
  g = std::move(f);
  EXPECT_EQ(g(), 42);
  EXPECT_FALSE(f);
}

TEST(FunctionTest, SmallFuncMoveAssignmentFromEmpty) {
  Function<int()> f;
  Function<int()> g = SmallFunc(55);
  g = std::move(f);
  EXPECT_FALSE(g);
}

TEST(FunctionTest, SmallFuncCopyAssignmentSelf) {
  Function<int()> f = SmallFunc(42);
  f = f;
  EXPECT_EQ(f(), 42);
}

TEST(FunctionTest, SmallFuncMoveAssignmentSelf) {
  Function<int()> f = SmallFunc(42);
  f = std::move(f);
  EXPECT_EQ(f(), 42);
}

namespace {

struct SmallFuncWithPointer {
  explicit SmallFuncWithPointer()
      : pointer(this) {}

  void swap(SmallFuncWithPointer& other) noexcept {
    std::swap(pointer, other.pointer);
  }

  SmallFuncWithPointer(const SmallFuncWithPointer&) noexcept
      : pointer(this) {}

  SmallFuncWithPointer& operator=(const SmallFuncWithPointer& other) noexcept {
    if (this != &other) {
      SmallFuncWithPointer(other).swap(*this);
    }
    return *this;
  }

  SmallFuncWithPointer(SmallFuncWithPointer&&) noexcept
      : pointer(this) {}

  SmallFuncWithPointer& operator=(SmallFuncWithPointer&& other) noexcept {
    if (this != &other) {
      SmallFuncWithPointer(std::move(other)).swap(*this);
    }
    return *this;
  }

  bool operator()() const {
    return pointer == this;
  }

private:
  SmallFuncWithPointer* pointer;
};

} // namespace

TEST(FunctionTest, SmallFuncWithPointer) {
  Function<int()> f = SmallFuncWithPointer();
  EXPECT_TRUE(is_small<SmallFuncWithPointer>(f));
}

TEST(FunctionTest, SmallFuncWithPointerCopyCtor) {
  Function<int()> f = SmallFuncWithPointer();
  Function<int()> g(f);
  EXPECT_TRUE(f());
  EXPECT_TRUE(g());
}

TEST(FunctionTest, SmallFuncWithPointerCopyAssignment) {
  Function<int()> f = SmallFuncWithPointer();
  Function<int()> g = SmallFuncWithPointer();
  f = g;
  EXPECT_TRUE(f());
  EXPECT_TRUE(g());
}

TEST(FunctionTest, SmallFuncWithPointerMoveCtor) {
  Function<int()> f = SmallFuncWithPointer();
  Function<int()> g(std::move(f));
  EXPECT_TRUE(g());
}

TEST(FunctionTest, SmallFuncWithPointerMoveAssignment) {
  Function<int()> f = SmallFuncWithPointer();
  Function<int()> g = SmallFuncWithPointer();
  g = std::move(f);
  EXPECT_TRUE(g());
}

namespace {

struct LargeFunc {
  LargeFunc(int value) noexcept
      : that(this)
      , value(value) {
    ++n_instances;
  }

  LargeFunc(const LargeFunc& other) noexcept
      : that(this)
      , value(other.value) {
    ++n_instances;
  }

  LargeFunc& operator=(const LargeFunc& rhs) noexcept {
    value = rhs.value;
    return *this;
  }

  ~LargeFunc() {
    assert(this == that);
    --n_instances;
  }

  int operator()() const noexcept {
    assert(this == that);
    return value;
  }

  static void assert_no_instances() {
    assert(n_instances == 0);
  }

  int get_value() const {
    return value;
  }

  int set_value(int new_value) {
    return value = new_value;
  }

private:
  [[maybe_unused]] LargeFunc* that;
  int value;
  [[maybe_unused]] int payload[1000]{};

  inline static size_t n_instances = 0;
};

} // namespace

TEST(FunctionTest, LargeFunc) {
  {
    Function<int()> f = LargeFunc(42);
    EXPECT_EQ(f(), 42);
  }
  LargeFunc::assert_no_instances();
}

TEST(FunctionTest, LargeFuncConst) {
  {
    const Function<int()> f = LargeFunc(42);
    EXPECT_EQ(f(), 42);
  }
  LargeFunc::assert_no_instances();
}

TEST(FunctionTest, LargeFuncTarget) {
  Function<int()> f = LargeFunc(42);
  EXPECT_EQ(f.target<LargeFunc>()->get_value(), 42);
  EXPECT_EQ(std::as_const(f).target<LargeFunc>()->get_value(), 42);
}

TEST(FunctionTest, LargeFuncCopyCtor) {
  Function<int()> f = LargeFunc(42);
  Function<int()> g = f;
  EXPECT_EQ(f(), 42);
  EXPECT_EQ(g(), 42);
}

TEST(FunctionTest, LargeFuncCopyCtorIndependence) {
  Function<int()> f = LargeFunc(42);
  Function<int()> g = f;
  f.target<LargeFunc>()->set_value(55);
  EXPECT_EQ(f(), 55);
  EXPECT_EQ(g(), 42);
}

TEST(FunctionTest, LargeFuncMoveCtor) {
  Function<int()> f = LargeFunc(42);
  Function<int()> g = std::move(f);
  EXPECT_EQ(g(), 42);
  EXPECT_FALSE(f);
}

TEST(FunctionTest, LargeFuncCopyAssignment) {
  Function<int()> f = LargeFunc(42);
  Function<int()> g = LargeFunc(55);
  g = f;
  EXPECT_EQ(f(), 42);
  EXPECT_EQ(g(), 42);
}

TEST(FunctionTest, LargeFuncCopyAssignmentToEmpty) {
  Function<int()> f = LargeFunc(42);
  Function<int()> g;
  g = f;
  EXPECT_EQ(f(), 42);
  EXPECT_EQ(g(), 42);
}

TEST(FunctionTest, LargeFuncCopyAssignmentFromEmpty) {
  Function<int()> f;
  Function<int()> g = LargeFunc(55);
  g = f;
  EXPECT_FALSE(f);
  EXPECT_FALSE(g);
}

TEST(FunctionTest, LargeFuncCopyAssignmentIndependence) {
  Function<int()> f = LargeFunc(42);
  Function<int()> g;
  g = f;
  f.target<LargeFunc>()->set_value(55);
  EXPECT_EQ(f(), 55);
  EXPECT_EQ(g(), 42);
}

TEST(FunctionTest, LargeFuncMoveAssignment) {
  Function<int()> f = LargeFunc(42);
  Function<int()> g = LargeFunc(55);
  g = std::move(f);
  EXPECT_EQ(g(), 42);
  EXPECT_TRUE(!f || f() == 55);
}

TEST(FunctionTest, LargeFuncMoveAssignmentToEmpty) {
  Function<int()> f = LargeFunc(42);
  Function<int()> g;
  g = std::move(f);
  EXPECT_EQ(g(), 42);
  EXPECT_FALSE(f);
}

TEST(FunctionTest, LargeFuncMoveAssignmentFromEmpty) {
  Function<int()> f;
  Function<int()> g = LargeFunc(55);
  g = std::move(f);
  EXPECT_FALSE(g);
}

TEST(FunctionTest, LargeFuncCopyAssignmentSelf) {
  Function<int()> f = LargeFunc(42);
  f = f;
  EXPECT_EQ(f(), 42);
}

TEST(FunctionTest, LargeFuncMoveAssignmentSelf) {
  Function<int()> f = LargeFunc(42);
  f = std::move(f);
  EXPECT_EQ(f(), 42);
}

namespace {

struct ThrowingCopy {
  struct exception final : std::exception {
    using std::exception::exception;
  };

  ThrowingCopy() = default;

  int operator()() const {
    return 43;
  }

  ThrowingCopy(const ThrowingCopy&) {
    throw exception();
  }

  ThrowingCopy(ThrowingCopy&&) noexcept {}

  ThrowingCopy& operator=(const ThrowingCopy&) = delete;
  ThrowingCopy& operator=(ThrowingCopy&&) = delete;
};

} // namespace

TEST(FunctionTest, SmallFuncThrowingCopy) {
  Function<int()> f = SmallFunc(42);
  Function<int()> g = ThrowingCopy();

  EXPECT_TRUE(is_small<ThrowingCopy>(g));

  EXPECT_THROW(f = g, ThrowingCopy::exception);
  EXPECT_EQ(f(), 42);

  EXPECT_NO_THROW(f = std::move(g));
  EXPECT_EQ(f(), 43);
}

TEST(FunctionTest, LargeFuncThrowingCopy) {
  Function<int()> f = LargeFunc(42);
  Function<int()> g = ThrowingCopy();

  EXPECT_TRUE(is_small<ThrowingCopy>(g));

  EXPECT_THROW(f = g, ThrowingCopy::exception);
  EXPECT_EQ(f(), 42);

  EXPECT_NO_THROW(f = std::move(g));
  EXPECT_EQ(f(), 43);
}

namespace {

struct ThrowingMove {
  struct exception final : std::exception {
    using std::exception::exception;
  };

  ThrowingMove() = default;

  int operator()() const {
    return 43;
  }

  ThrowingMove(const ThrowingMove& other)
      : enable_exception(other.enable_exception) {
    throw exception();
  }

  ThrowingMove(ThrowingMove&& other)
      : enable_exception(other.enable_exception) {
    if (enable_exception) {
      throw exception();
    }
  }

  ThrowingMove& operator=(const ThrowingMove&) = delete;
  ThrowingMove& operator=(ThrowingMove&&) = delete;

  bool enable_exception = false;
};

} // namespace

TEST(FunctionTest, ThrowingMove) {
  Function<int()> f = SmallFunc(42);
  Function<int()> g = ThrowingMove();

  g.target<ThrowingMove>()->enable_exception = true;

  EXPECT_NO_THROW(f = std::move(g));
  EXPECT_EQ(f(), 43);
}

TEST(FunctionTest, Arguments) {
  Function<int(int, int)> f = [](int a, int b) {
    return a + b;
  };

  int x = 2;
  EXPECT_EQ(f(40, x), 42);
}

TEST(FunctionTest, ArgumentsRef) {
  Function<int&(int&)> f = [](int& a) -> int& {
    return a;
  };

  int x = 42;
  EXPECT_EQ(&x, &f(x));
}

TEST(FunctionTest, ArgumentsCref) {
  Function<const int&(const int&)> f = [](const int& a) -> const int& {
    return a;
  };

  const int x = 42;
  EXPECT_EQ(&x, &f(x));
}

namespace {

struct NonCopyable {
  NonCopyable() {}

  NonCopyable(const NonCopyable&) = delete;
  NonCopyable(NonCopyable&&) = default;
};

} // namespace

TEST(FunctionTest, ArgumentByValue) {
  Function<NonCopyable(NonCopyable)> f = [](NonCopyable a) {
    return a;
  };
  [[maybe_unused]] NonCopyable a = f(NonCopyable());
}

TEST(FunctionTest, ArgumentByValueLarge) {
  int big_array[1000]{};
  Function<NonCopyable(NonCopyable)> f = [big_array](NonCopyable a) {
    (void) big_array;
    return a;
  };
  [[maybe_unused]] NonCopyable a = f(NonCopyable());
}

TEST(FunctionTest, RecursiveTest) {
  Function<int(int)> fib = [&fib](int n) -> int {
    switch (n) {
    case 0:
      return 0;
    case 1:
      return 1;
    default:
      return fib(n - 1) + fib(n - 2);
    }
  };
  EXPECT_EQ(fib(10), 55);
}

TEST(FunctionTest, Target) {
  struct Foo {
    void operator()() const {}
  };

  struct Bar {
    void operator()() const {}
  };

  Function<void()> f = Foo();
  EXPECT_NE(f.target<Foo>(), nullptr);
  EXPECT_EQ(f.target<Bar>(), nullptr);
  EXPECT_NE(std::as_const(f).target<Foo>(), nullptr);
  EXPECT_EQ(std::as_const(f).target<Bar>(), nullptr);
  f = Bar();
  EXPECT_EQ(f.target<Foo>(), nullptr);
  EXPECT_NE(f.target<Bar>(), nullptr);
  EXPECT_EQ(std::as_const(f).target<Foo>(), nullptr);
  EXPECT_NE(std::as_const(f).target<Bar>(), nullptr);
}

TEST(FunctionTest, NonCallableTarget) {
  Function<void()> f = [] {
  };
  EXPECT_EQ(f.target<int>(), nullptr);
  EXPECT_EQ(f.target<void>(), nullptr);
  EXPECT_EQ(f.target<ct::BadFunctionCall>(), nullptr);
}

TEST(FunctionTest, MutableSmall) {
  Function<int()> f = [x = 0]() mutable {
    return ++x;
  };
  EXPECT_EQ(f(), 1);
  EXPECT_EQ(f(), 2);
}

TEST(FunctionTest, MutableSmallConst) {
  const Function<int()> f = [x = 0]() mutable {
    return ++x;
  };
  EXPECT_EQ(f(), 1);
  EXPECT_EQ(f(), 2);
}

TEST(FunctionTest, MutableLarge) {
  int big_array[1000]{};
  Function<int()> f = [x = 0, big_array]() mutable {
    (void) big_array;
    return ++x;
  };
  EXPECT_EQ(f(), 1);
  EXPECT_EQ(f(), 2);
}

TEST(FunctionTest, MutableLargeConst) {
  int big_array[1000]{};
  const Function<int()> f = [x = 0, big_array]() mutable {
    (void) big_array;
    return ++x;
  };
  EXPECT_EQ(f(), 1);
  EXPECT_EQ(f(), 2);
}

struct TrackingFunc {
  struct TrackerType {
    size_t destructors = 0;
    size_t moves = 0;
  };

  TrackingFunc(TrackerType* tracker) noexcept
      : tracker(tracker) {}

  TrackingFunc(const TrackingFunc& other)
      : tracker(other.tracker) {
    throw std::logic_error("This constructor should never be called");
  }

  TrackingFunc(TrackingFunc&& other) noexcept
      : tracker(other.tracker) {
    ++tracker->moves;
  }

  TrackingFunc& operator=(const TrackingFunc&) = delete;
  TrackingFunc& operator=(TrackingFunc&&) = delete;

  void operator()() const noexcept {}

  ~TrackingFunc() {
    ++tracker->destructors;
  }

private:
  TrackerType* tracker;
};

struct LargeTrackingFunc : TrackingFunc {
  using TrackingFunc::TrackingFunc;

  [[maybe_unused]] int payload[1000]{};
};

TEST(FunctionTest, MoveAssignmentLargeToSmall) {
  TrackingFunc::TrackerType ts;
  TrackingFunc::TrackerType tl;

  {
    Function<void()> small = TrackingFunc(&ts);
    EXPECT_TRUE(is_small<TrackingFunc>(small));
    EXPECT_EQ(ts.moves, 1);
    ts = {};

    Function<void()> large = LargeTrackingFunc(&tl);
    EXPECT_FALSE(is_small<LargeTrackingFunc>(large));
    EXPECT_EQ(tl.moves, 1);
    tl = {};

    small = std::move(large);
    EXPECT_EQ(tl.moves, 0);
    EXPECT_EQ(tl.destructors, 0);
    EXPECT_LE(ts.destructors, 1);
  }

  EXPECT_EQ(tl.moves, 0);
  EXPECT_EQ(tl.destructors, 1);
  EXPECT_EQ(ts.destructors, ts.moves + 1);
}

TEST(FunctionTest, MoveAssignmentSmallToLarge) {
  TrackingFunc::TrackerType ts;
  TrackingFunc::TrackerType tl;

  {
    Function<void()> small = TrackingFunc(&ts);
    EXPECT_TRUE(is_small<TrackingFunc>(small));
    EXPECT_EQ(ts.moves, 1);
    ts = {};

    Function<void()> large = LargeTrackingFunc(&tl);
    EXPECT_FALSE(is_small<LargeTrackingFunc>(large));
    EXPECT_EQ(tl.moves, 1);
    tl = {};

    large = std::move(small);
    EXPECT_EQ(tl.moves, 0);
    EXPECT_EQ(tl.destructors, 1);
    EXPECT_LE(ts.moves, 1);
  }

  EXPECT_EQ(tl.moves, 0);
  EXPECT_EQ(tl.destructors, 1);
  EXPECT_EQ(ts.destructors, ts.moves + 1);
}

TEST(FunctionTest, ArgumentsEfficientForwarding) {
  TrackingFunc::TrackerType t;

  Function<int(TrackingFunc)> f = [](TrackingFunc) {
    return 42;
  };

  EXPECT_EQ(f(TrackingFunc(&t)), 42);
  EXPECT_LE(t.moves, 1);
}

namespace {

template <size_t... Is, typename F>
void static_for_each(std::index_sequence<Is...>, F f) {
  (f.template operator()<Is>(), ...);
}

template <size_t SIZE>
struct SizedFunc {
  size_t operator()() const noexcept {
    return SIZE;
  }

  std::array<std::byte, SIZE> payload{};
};

} // namespace

TEST(FunctionTest, DifferentSizes) {
  using BaseSizes = std::index_sequence<1, 2, 4, 8, 16, 32, 64, 128, 256>;
  using Multipliers = std::index_sequence<1, 3, 5, 7, 9, 11, 13, 15>;

  static_for_each(BaseSizes{}, []<size_t BASE_SIZE>() {
    static_for_each(Multipliers{}, []<size_t MULTIPLIER>() {
      static constexpr size_t SIZE = BASE_SIZE * MULTIPLIER;
      using F = SizedFunc<SIZE>;

      Function<size_t()> f = F();
      f.target<F>()->payload.fill(std::byte{});
      EXPECT_EQ(f(), SIZE);
      if constexpr (SIZE <= sizeof(F) - alignof(void*)) {
        EXPECT_TRUE(is_small<F>(f));
      }
    });
  });
}

TEST(FunctionTest, Alignment) {
  using Alignments = std::index_sequence<1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096>;

  static_for_each(Alignments{}, []<size_t ALIGNMENT>() {
    using Sizes = std::index_sequence<1, ALIGNMENT>;

    static_for_each(Sizes{}, []<size_t SIZE>() {
      struct alignas(ALIGNMENT) AlignedFunc : SizedFunc<SIZE> {};

      Function<size_t()> f = AlignedFunc();
      auto* target = f.target<AlignedFunc>();
      target->payload.fill(std::byte{});
      EXPECT_EQ(reinterpret_cast<uintptr_t>(target) % ALIGNMENT, 0);
      EXPECT_EQ(f(), SIZE);
    });
  });
}

TEST(FunctionTest, OptimalSmallSize) {
  struct alignas(alignof(void*)) SmallFunc : SizedFunc<sizeof(void*)> {};

  Function<int()> f = SmallFunc();
  EXPECT_TRUE(is_small<SmallFunc>(f));
}

} // namespace ct_test
