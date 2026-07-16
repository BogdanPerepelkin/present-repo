#include "shared-generator.h"

#include <gtest/gtest.h>

#include <initializer_list>
#include <optional>
#include <ostream>
#include <utility>
#include <variant>
#include <vector>

namespace ct_test {

namespace {

template <typename... Ts>
struct Overloaded : Ts... {
  using Ts::operator()...;
};

struct CopyEvent {
  int from;
  int to;

  bool operator==(const CopyEvent&) const = default;
};

struct MoveEvent {
  int value;

  bool operator==(const MoveEvent&) const = default;
};

struct ValueEvent {
  int value;

  bool operator==(const ValueEvent&) const = default;
};

using Event = std::variant<CopyEvent, MoveEvent, ValueEvent>;

class EventSequence {
public:
  EventSequence() = default;

  EventSequence(std::initializer_list<Event> events)
      : _seq(events) {}

  template <typename E, typename... Args>
  void add(Args&&... args) {
    _seq.emplace_back(std::in_place_type<E>, std::forward<Args>(args)...);
  }

  friend bool operator==(const EventSequence&, const EventSequence&) = default;

  friend std::ostream& operator<<(std::ostream& out, const EventSequence& seq) {
    out << "{ ";
    bool is_first = true;
    for (const Event& event : seq._seq) {
      if (!std::exchange(is_first, false)) {
        out << ", ";
      }
      std::visit(
          Overloaded{
              [&out](CopyEvent e) {
                out << "Copy(" << e.from << " -> " << e.to << ")";
              },
              [&out](MoveEvent e) {
                out << "Move(" << e.value << ")";
              },
              [&out](ValueEvent e) {
                out << "Value(" << e.value << ")";
              }
          },
          event
      );
    }
    out << " }";
    return out;
  }

private:
  std::vector<Event> _seq;
};

class GeneratorYieldTest : public ::testing::Test {
protected:
  struct Element {
    explicit Element(int x, GeneratorYieldTest* test)
        : _value(x)
        , _test(test) {}

    Element(const Element& other)
        : _value(other._value * 10)
        , _test(other._test) {
//      std::cout << "[main] copy" << std::endl;
      _test->events.add<CopyEvent>(other._value, _value);
    }

    Element(Element&& other) noexcept
        : _value(std::exchange(other._value, -1))
        , _test(other._test) {
//      std::cout << "[main] move" << std::endl;
      _test->events.add<MoveEvent>(_value);
    }

    Element& operator=(const Element&) = delete;
    Element& operator=(Element&&) = delete;

    int value() const noexcept {
      return _value;
    }

    void operator&() = delete;

  private:
    int _value;
    GeneratorYieldTest* _test;
  };

  Element make_element(int value) {
    return Element(value, this);
  }

protected:
  EventSequence events;
};

} // namespace

TEST_F(GeneratorYieldTest, Prvalue) {
  auto gen = [this] -> ct::SharedGenerator<Element> {
    co_yield make_element(1);
  };

  auto g = gen();
  while (std::optional<Element> e = g.next()) {
    events.add<ValueEvent>(e->value());
  }

  EventSequence expected_events = {
      MoveEvent(1),
      ValueEvent(1),
  };
  EXPECT_EQ(events, expected_events);
}

TEST_F(GeneratorYieldTest, Lvalue) {
  auto gen = [this] -> ct::SharedGenerator<Element> {
    Element e = make_element(2);
    co_yield e;
  };

  auto g = gen();
  while (std::optional<Element> e = g.next()) {
    events.add<ValueEvent>(e->value());
  }

  EventSequence expected_events = {
      CopyEvent(2, 20),
      ValueEvent(20),
  };
  EXPECT_EQ(events, expected_events);
}

TEST_F(GeneratorYieldTest, ConstLvalue) {
  auto gen = [this] -> ct::SharedGenerator<Element> {
    const Element e = make_element(3);
    co_yield e;
  };

  auto g = gen();
  while (std::optional<Element> e = g.next()) {
    events.add<ValueEvent>(e->value());
  }

  EventSequence expected_events = {
      CopyEvent(3, 30),
      ValueEvent(30),
  };
  EXPECT_EQ(events, expected_events);
}

TEST_F(GeneratorYieldTest, Xvalue) {
  auto gen = [this] -> ct::SharedGenerator<Element> {
    Element e = make_element(4);
    co_yield std::move(e);
  };

  auto g = gen();
  while (std::optional<Element> e = g.next()) {
    events.add<ValueEvent>(e->value());
  }

  EventSequence expected_events = {
      MoveEvent(4),
      ValueEvent(4),
  };
  EXPECT_EQ(events, expected_events);
}

TEST_F(GeneratorYieldTest, ConstXvalue) {
  auto gen = [this] -> ct::SharedGenerator<Element> {
    const Element e = make_element(5);
    co_yield std::move(e);
  };

  auto g = gen();
  while (std::optional<Element> e = g.next()) {
    events.add<ValueEvent>(e->value());
  }

  EventSequence expected_events = {
      CopyEvent(5, 50),
      ValueEvent(50),
  };
  EXPECT_EQ(events, expected_events);
}

// TODO: support:
//   - SharedGenerator<Element&>
//   - SharedGenerator<Element&&>
//   - SharedGenerator<const Element>
//   - SharedGenerator<const Element&>
//   - SharedGenerator<const Element&&>

} // namespace ct_test
