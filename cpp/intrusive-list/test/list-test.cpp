#include "intrusive-list.h"
#include "test-utils.h"

#include <iterator>
#include <utility>

namespace ct_test {

TEST(ListTest, default_ctor) {
  ct::intrusive::List<Node> list;
  EXPECT_TRUE(list.empty());
  EXPECT_EQ(list.size(), 0);
}

TEST(ListTest, push_back_lifetime_1) {
  ct::intrusive::List<Node> list;
  Node a(1);
  list.push_back(a);
}

TEST(ListTest, push_back_lifetime_2) {
  Node a(1);
  ct::intrusive::List<Node> list;
  list.push_back(a);
}

TEST(ListTest, push_front_lifetime_1) {
  ct::intrusive::List<Node> list;
  Node a(1);
  list.push_front(a);
}

TEST(ListTest, push_front_lifetime_2) {
  Node a(1);
  ct::intrusive::List<Node> list;
  list.push_front(a);
}

TEST(ListTest, push_back_many) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3), d(4), e(5);
  mass_push_back(list, a, b, c, d, e);

  expect_eq(list, {1, 2, 3, 4, 5});
}

TEST(ListTest, push_front_many) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3), d(4), e(5);
  mass_push_front(list, a, b, c, d, e);

  expect_eq(list, {5, 4, 3, 2, 1});
}

TEST(ListTest, clear) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list, a, b, c, d);

  list.clear();
  EXPECT_TRUE(list.empty());
  EXPECT_EQ(list.size(), 0);
}

TEST(ListTest, back_front) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3);
  mass_push_back(list, a, b, c);

  EXPECT_EQ(list.front(), 1);
  EXPECT_EQ(list.back(), 3);
  EXPECT_EQ(std::as_const(list).front(), 1);
  EXPECT_EQ(std::as_const(list).back(), 3);
}

TEST(ListTest, back_front_ref) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3);
  mass_push_back(list, a, b, c);

  list.front().value = 6;
  list.back().value = 7;

  expect_eq(list, {6, 2, 7});
}

TEST(ListTest, back_front_cref) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3);
  mass_push_back(list, a, b, c);

  EXPECT_TRUE(&std::as_const(list).front() == &list.front());
  EXPECT_TRUE(&std::as_const(list).back() == &list.back());
}

TEST(ListTest, back_front_ncref) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3);
  mass_push_back(list, a, b, c);

  magic(std::as_const(list).front());
  magic(std::as_const(list).back());

  expect_eq(list, {1, 2, 3});
}

TEST(ListTest, multiple_tags) {
  ct::intrusive::List<MultiNode, TagA> list_a;
  ct::intrusive::List<MultiNode, TagB> list_b;
  MultiNode x(1), y(2), z(3);

  mass_push_back(list_a, x, y, z);
  mass_push_back(list_b, z, y, x);

  expect_eq(list_a, {1, 2, 3});
  expect_eq(list_b, {3, 2, 1});
}

TEST(ListTest, multiple_tags_back_front) {
  ct::intrusive::List<MultiNode, TagA> list_a;
  ct::intrusive::List<MultiNode, TagB> list_b;
  MultiNode x(1), y(2), z(3);

  mass_push_back(list_a, x, y, z);
  mass_push_back(list_b, z, y, x);

  EXPECT_EQ(list_a.front(), 1);
  EXPECT_EQ(list_a.back(), 3);
  EXPECT_EQ(std::as_const(list_a).front(), 1);
  EXPECT_EQ(std::as_const(list_a).back(), 3);
}

TEST(ListTest, pop_back) {
  Node a(1);
  ct::intrusive::List<Node> list;

  list.push_back(a);
  EXPECT_EQ(list.back(), 1);
  EXPECT_EQ(list.front(), 1);
  EXPECT_FALSE(list.empty());

  list.pop_back();
  EXPECT_TRUE(list.empty());
}

TEST(ListTest, pop_front) {
  Node a(1);
  ct::intrusive::List<Node> list;

  list.push_front(a);
  EXPECT_EQ(list.back(), 1);
  EXPECT_EQ(list.front(), 1);
  EXPECT_FALSE(list.empty());

  list.pop_front();
  EXPECT_TRUE(list.empty());
}

TEST(ListTest, pop) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list, a, b, c, d);

  EXPECT_EQ(list.front(), 1);
  EXPECT_EQ(list.back(), 4);
  EXPECT_EQ(list.size(), 4);

  list.pop_front();
  EXPECT_EQ(list.front(), 2);
  EXPECT_EQ(list.back(), 4);
  EXPECT_EQ(list.size(), 3);

  list.pop_back();
  EXPECT_EQ(list.front(), 2);
  EXPECT_EQ(list.back(), 3);
  EXPECT_EQ(list.size(), 2);

  list.pop_front();
  EXPECT_EQ(list.front(), 3);
  EXPECT_EQ(list.back(), 3);
  EXPECT_EQ(list.size(), 1);
}

TEST(ListTest, move_ctor) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3);
  mass_push_back(list1, a, b, c);

  ct::intrusive::List<Node> list2 = std::move(list1);

  EXPECT_TRUE(list1.empty());
  expect_eq(list2, {1, 2, 3});
}

TEST(ListTest, move_ctor_empty) {
  ct::intrusive::List<Node> list1;
  ct::intrusive::List<Node> list2 = std::move(list1);

  EXPECT_TRUE(list1.empty());
  EXPECT_TRUE(list2.empty());
}

TEST(ListTest, move_assignment) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3);
  mass_push_back(list1, a, b, c);

  ct::intrusive::List<Node> list2;
  Node d(4), e(5), f(6);
  mass_push_back(list2, d, e, f);

  list1 = std::move(list2);

  expect_eq(list1, {4, 5, 6});
  EXPECT_TRUE(list2.empty());
}

TEST(ListTest, move_assignment_self) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3);
  mass_push_back(list, a, b, c);

  list = std::move(list);

  expect_eq(list, {1, 2, 3});
}

TEST(ListTest, move_assignment_empty) {
  ct::intrusive::List<Node> list1, list2;

  list1 = std::move(list2);

  EXPECT_TRUE(list1.empty());
  EXPECT_TRUE(list2.empty());
}

TEST(ListTest, move_assignment_to_empty) {
  ct::intrusive::List<Node> list1;

  ct::intrusive::List<Node> list2;
  Node a(1), b(2), c(3);
  mass_push_back(list2, a, b, c);

  list1 = std::move(list2);

  expect_eq(list1, {1, 2, 3});
  EXPECT_TRUE(list2.empty());
}

TEST(ListTest, move_assignment_from_empty) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3);
  mass_push_back(list1, a, b, c);

  ct::intrusive::List<Node> list2;

  list1 = std::move(list2);

  EXPECT_TRUE(list1.empty());
  EXPECT_TRUE(list2.empty());
}

TEST(ListTest, swap) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3);
  mass_push_back(list1, a, b, c);

  ct::intrusive::List<Node> list2;
  Node d(4), e(5), f(6);
  mass_push_back(list2, d, e, f);

  {
    using std::swap;
    swap(list1, list2);
  }

  expect_eq(list1, {4, 5, 6});
  expect_eq(list2, {1, 2, 3});
}

TEST(ListTest, insert_front) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list, b, c, d);

  auto it1 = list.begin();
  auto it2 = list.insert(it1, a);

  EXPECT_EQ(*it2, 1);
  EXPECT_EQ(*it1, 2);
  EXPECT_NE(it2, it1);
  EXPECT_EQ(it2, list.begin());

  ++it2;
  EXPECT_EQ(*it2, 2);
  EXPECT_EQ(it2, it1);

  --it1;
  EXPECT_EQ(*it1, 1);

  expect_eq(list, {1, 2, 3, 4});
}

TEST(ListTest, insert_back) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list, a, b, c);

  auto it1 = list.end();
  auto it2 = list.insert(it1, d);

  EXPECT_EQ(*it2, 4);
  --it2;
  EXPECT_EQ(*it2, 3);
  ++it2;
  EXPECT_EQ(*it2, 4);
  ++it2;
  EXPECT_EQ(it2, it1);
  --it1;
  EXPECT_EQ(*it1, 4);
}

TEST(ListTest, insert_middle) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3), d(4);

  list.push_back(a);
  list.push_back(b);
  list.push_back(d);

  auto it1 = std::next(list.begin(), 2);
  auto it2 = list.insert(it1, c);

  EXPECT_EQ(*it2, 3);
  --it2;
  EXPECT_EQ(*it2, 2);
  ++it2;
  EXPECT_EQ(*it2, 3);
  ++it2;
  EXPECT_EQ(*it2, 4);
  EXPECT_EQ(it2, it1);
  --it1;
  EXPECT_EQ(*it1, 3);
}

TEST(ListTest, reinsert_to_same_list) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3);
  mass_push_back(list, a, b, c);

  auto b_iter = std::next(list.begin());

  list.insert(b_iter, b);
  expect_eq(list, {1, 2, 3});

  list.insert(b_iter, a);
  expect_eq(list, {1, 2, 3});

  list.insert(b_iter, c);
  expect_eq(list, {1, 3, 2});
}

TEST(ListTest, reinsert_to_another_list) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3);
  mass_push_back(list1, a, b, c);

  ct::intrusive::List<Node> list2;
  Node d(4), e(5), f(6);
  mass_push_back(list2, d, e, f);

  auto b_iter = std::next(list1.begin());
  auto e_iter = std::next(list2.begin());

  list1.insert(b_iter, f);
  expect_eq(list1, {1, 6, 2, 3});
  expect_eq(list2, {4, 5});

  list2.insert(e_iter, b);
  expect_eq(list1, {1, 6, 3});
  expect_eq(list2, {4, 2, 5});

  list2.insert(b_iter, f);
  expect_eq(list1, {1, 3});
  expect_eq(list2, {4, 6, 2, 5});
}

TEST(ListTest, erase_front) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3);
  mass_push_back(list, a, b, c);

  auto erased_it = list.begin();
  auto it = list.erase(erased_it);

  EXPECT_EQ(*it, 2);
  EXPECT_EQ(it, list.begin());

  expect_eq(list, {2, 3});
}

TEST(ListTest, erase_back) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3);
  mass_push_back(list, a, b, c);

  auto erased_it = std::prev(list.end());
  auto it = list.erase(erased_it);

  EXPECT_EQ(it, list.end());
  --it;
  EXPECT_EQ(*it, 2);

  expect_eq(list, {1, 2});
}

TEST(ListTest, erase_middle) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3);
  mass_push_back(list, a, b, c);

  auto erased_it = std::next(list.begin(), 1);
  auto it = list.erase(erased_it);

  EXPECT_EQ(*it, 3);

  --it;
  EXPECT_EQ(*it, 1);
  EXPECT_EQ(it, list.begin());

  ++it;
  EXPECT_EQ(*it, 3);

  expect_eq(list, {1, 3});
}

TEST(ListTest, insert_and_erase_1) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3);

  list.push_back(b);
  list.push_back(c);

  auto it1 = list.insert(list.begin(), a);
  auto it2 = std::next(it1);

  list.erase(it1);
  EXPECT_EQ(it2, list.begin());

  expect_eq(list, {2, 3});
}

TEST(ListTest, insert_and_erase_2) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3);

  list.push_back(b);
  list.push_back(c);

  auto it1 = list.insert(list.begin(), a);
  auto it2 = std::next(it1);

  list.erase(it2);

  EXPECT_EQ(it1, list.begin());
  ++it1;
  EXPECT_EQ(*it1, 3);

  expect_eq(list, {1, 3});
}

TEST(ListTest, splice_begin_begin) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list1, a, b, c, d);

  ct::intrusive::List<Node> list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(list1.begin(), list2, list2.begin(), std::next(list2.begin(), 2));

  expect_eq(list1, {5, 6, 1, 2, 3, 4});
  expect_eq(list2, {7, 8});
}

TEST(ListTest, splice_begin_middle) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list1, a, b, c, d);

  ct::intrusive::List<Node> list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(list1.begin(), list2, std::next(list2.begin()), std::next(list2.begin(), 2));

  expect_eq(list1, {6, 1, 2, 3, 4});
  expect_eq(list2, {5, 7, 8});
}

TEST(ListTest, splice_begin_end) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list1, a, b, c, d);

  ct::intrusive::List<Node> list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(list1.begin(), list2, std::next(list2.begin(), 2), list2.end());

  expect_eq(list1, {7, 8, 1, 2, 3, 4});
  expect_eq(list2, {5, 6});
}

TEST(ListTest, splice_begin_whole) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list1, a, b, c, d);

  ct::intrusive::List<Node> list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(list1.begin(), list2, list2.begin(), list2.end());

  expect_eq(list1, {5, 6, 7, 8, 1, 2, 3, 4});
  EXPECT_TRUE(list2.empty());
}

TEST(ListTest, splice_begin_empty) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list1, a, b, c, d);

  ct::intrusive::List<Node> list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(list1.begin(), list2, std::next(list2.begin(), 2), std::next(list2.begin(), 2));

  expect_eq(list1, {1, 2, 3, 4});
  expect_eq(list2, {5, 6, 7, 8});
}

TEST(ListTest, splice_middle_begin) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list1, a, b, c, d);

  ct::intrusive::List<Node> list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(std::next(list1.begin(), 2), list2, list2.begin(), std::next(list2.begin(), 2));

  expect_eq(list1, {1, 2, 5, 6, 3, 4});
  expect_eq(list2, {7, 8});
}

TEST(ListTest, splice_middle_middle) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list1, a, b, c, d);

  ct::intrusive::List<Node> list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(std::next(list1.begin(), 2), list2, std::next(list2.begin()), std::next(list2.begin(), 3));

  expect_eq(list1, {1, 2, 6, 7, 3, 4});
  expect_eq(list2, {5, 8});
}

TEST(ListTest, splice_middle_end) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list1, a, b, c, d);

  ct::intrusive::List<Node> list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(std::next(list1.begin(), 2), list2, std::next(list2.begin(), 2), list2.end());

  expect_eq(list1, {1, 2, 7, 8, 3, 4});
  expect_eq(list2, {5, 6});
}

TEST(ListTest, splice_middle_whole) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list1, a, b, c, d);

  ct::intrusive::List<Node> list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(std::next(list1.begin(), 2), list2, list2.begin(), list2.end());

  expect_eq(list1, {1, 2, 5, 6, 7, 8, 3, 4});
  EXPECT_TRUE(list2.empty());
}

TEST(ListTest, splice_middle_empty) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list1, a, b, c, d);

  ct::intrusive::List<Node> list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(std::next(list1.begin(), 2), list2, std::next(list2.begin(), 2), std::next(list2.begin(), 2));

  expect_eq(list1, {1, 2, 3, 4});
  expect_eq(list2, {5, 6, 7, 8});
}

TEST(ListTest, splice_end_begin) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list1, a, b, c, d);

  ct::intrusive::List<Node> list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(list1.end(), list2, list2.begin(), std::next(list2.begin(), 2));

  expect_eq(list1, {1, 2, 3, 4, 5, 6});
  expect_eq(list2, {7, 8});
}

TEST(ListTest, splice_end_middle) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list1, a, b, c, d);

  ct::intrusive::List<Node> list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(list1.end(), list2, std::next(list2.begin()), std::next(list2.begin(), 3));

  expect_eq(list1, {1, 2, 3, 4, 6, 7});
  expect_eq(list2, {5, 8});
}

TEST(ListTest, splice_end_end) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list1, a, b, c, d);

  ct::intrusive::List<Node> list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(list1.end(), list2, std::next(list2.begin(), 2), list2.end());

  expect_eq(list1, {1, 2, 3, 4, 7, 8});
  expect_eq(list2, {5, 6});
}

TEST(ListTest, splice_end_whole) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list1, a, b, c, d);

  ct::intrusive::List<Node> list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(list1.end(), list2, list2.begin(), list2.end());

  expect_eq(list1, {1, 2, 3, 4, 5, 6, 7, 8});
  EXPECT_TRUE(list2.empty());
}

TEST(ListTest, splice_end_empty) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list1, a, b, c, d);

  ct::intrusive::List<Node> list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(list1.end(), list2, std::next(list2.begin(), 2), std::next(list2.begin(), 2));

  expect_eq(list1, {1, 2, 3, 4});
  expect_eq(list2, {5, 6, 7, 8});
}

TEST(ListTest, splice_empty_begin) {
  ct::intrusive::List<Node> list1, list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(list1.end(), list2, list2.begin(), std::next(list2.begin(), 2));

  expect_eq(list1, {5, 6});
  expect_eq(list2, {7, 8});
}

TEST(ListTest, splice_empty_middle) {
  ct::intrusive::List<Node> list1, list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(list1.end(), list2, std::next(list2.begin(), 1), std::next(list2.begin(), 3));

  expect_eq(list1, {6, 7});
  expect_eq(list2, {5, 8});
}

TEST(ListTest, splice_empty_end) {
  ct::intrusive::List<Node> list1, list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(list1.end(), list2, std::next(list2.begin(), 2), list2.end());

  expect_eq(list1, {7, 8});
  expect_eq(list2, {5, 6});
}

TEST(ListTest, splice_empty_whole) {
  ct::intrusive::List<Node> list1, list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(list1.end(), list2, list2.begin(), list2.end());

  expect_eq(list1, {5, 6, 7, 8});
  EXPECT_TRUE(list2.empty());
}

TEST(ListTest, splice_empty_empty) {
  ct::intrusive::List<Node> list1, list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  list1.splice(list1.end(), list2, list2.begin(), list2.begin());

  EXPECT_TRUE(list1.empty());
  expect_eq(list2, {5, 6, 7, 8});
}

TEST(ListTest, splice_self) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3), d(4), e(5);
  mass_push_back(list1, a, b, c, d, e);

  list1.splice(std::next(list1.begin()), list1, std::next(list1.begin(), 2), std::prev(list1.end()));

  expect_eq(list1, {1, 3, 4, 2, 5});
}

TEST(ListTest, splice_iterators) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3), d(4);
  mass_push_back(list1, a, b, c, d);

  ct::intrusive::List<Node> list2;
  Node e(5), f(6), g(7), h(8);
  mass_push_back(list2, e, f, g, h);

  ct::intrusive::List<Node>::ConstIterator i = std::next(list1.begin(), 2);
  ct::intrusive::List<Node>::ConstIterator j = std::next(list2.begin());
  ct::intrusive::List<Node>::ConstIterator k = std::prev(list2.end());

  list1.splice(i, list2, j, k);

  expect_eq(list1, {1, 2, 6, 7, 3, 4});
  expect_eq(list2, {5, 8});

  EXPECT_EQ(*i, 3);
  EXPECT_EQ(*j, 6);
  EXPECT_EQ(*k, 8);

  EXPECT_EQ(*std::prev(i), 7);
  EXPECT_EQ(*std::prev(j), 2);
  EXPECT_EQ(*std::prev(k), 5);
}

} // namespace ct_test
