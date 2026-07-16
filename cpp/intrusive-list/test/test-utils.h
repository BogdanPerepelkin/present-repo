#pragma once

#include "intrusive-list.h"

#include <gtest/gtest.h>

#include <ranges>

namespace ct_test {

template <typename C>
void mass_push_back(C&) {}

template <typename C, typename Element, typename... Elements>
void mass_push_back(C& cont, Element& element, Elements&... elements) {
  cont.push_back(element);
  mass_push_back(cont, elements...);
}

template <typename C>
void mass_push_front(C&) {}

template <typename C, typename Element, typename... Elements>
void mass_push_front(C& cont, Element& element, Elements&... elements) {
  cont.push_front(element);
  mass_push_front(cont, elements...);
}

template <typename E, typename A>
void expect_eq_impl(A actual_first, A actual_last, E expected_first, E expected_last) {
  for (;;) {
    bool actual_ended = actual_first == actual_last;
    bool expected_ended = expected_first == expected_last;
    EXPECT_EQ(actual_ended, expected_ended);

    if (actual_ended || expected_ended) {
      break;
    }

    EXPECT_EQ(*actual_first, *expected_first);
    ++actual_first;
    ++expected_first;
  }
}

template <typename E, typename A>
void expect_eq_impl(const A& actual, const E& expected) {
  expect_eq_impl(actual.begin(), actual.end(), expected.begin(), expected.end());
}

template <typename T, typename Tag>
void expect_eq(const ct::intrusive::List<T, Tag>& list, std::initializer_list<int> values) {
  EXPECT_EQ(list.size(), values.size());
  expect_eq_impl(list, values);
  expect_eq_impl(std::views::reverse(list), std::views::reverse(values));
}

struct CopyableNode : ct::intrusive::ListElement<> {
  explicit CopyableNode(int value)
      : value(value) {}

  operator int() const {
    return value;
  }

  int value;
};

struct Node : ct::intrusive::ListElement<> {
  explicit Node(int value)
      : value(value) {}

  Node(const Node& other) = delete;
  Node(Node&& other) = delete;
  Node& operator=(const Node& other) = delete;
  Node& operator=(Node&& other) = delete;

  operator int() const {
    return value;
  }

  void unlink(int) {
    // do nothing
  }

  int value;
};

struct MultiNode
    : ct::intrusive::ListElement<struct TagA>
    , ct::intrusive::ListElement<struct TagB> {
  explicit MultiNode(int value)
      : value(value) {}

  MultiNode(const MultiNode& other) = delete;
  MultiNode(MultiNode&& other) = delete;
  MultiNode& operator=(const MultiNode& other) = delete;
  MultiNode& operator=(MultiNode&& other) = delete;

  operator int() const {
    return value;
  }

  int value;
};

inline void magic(Node& n) {
  n.value = 42;
}

inline void magic([[maybe_unused]] const Node& n) {}

} // namespace ct_test
