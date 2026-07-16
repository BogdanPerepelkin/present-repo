#include "intrusive-list.h"
#include "test-utils.h"

#include <concepts>
#include <iterator>

namespace ct_test {

#define EXPECT_TRAIT_TRUE(...) EXPECT_TRUE((__VA_ARGS__))
#define EXPECT_TRAIT_FALSE(...) EXPECT_FALSE((__VA_ARGS__))

TEST(IteratorTest, categories) {
  EXPECT_TRAIT_TRUE(std::bidirectional_iterator<ct::intrusive::List<Node>::Iterator>);
  EXPECT_TRAIT_TRUE(std::bidirectional_iterator<ct::intrusive::List<Node>::ConstIterator>);
}

TEST(IteratorTest, public_ctors) {
  EXPECT_TRAIT_FALSE(std::constructible_from<ct::intrusive::List<Node>::Iterator, Node*>);
  EXPECT_TRAIT_FALSE(std::constructible_from<ct::intrusive::List<Node>::ConstIterator, const Node*>);

  EXPECT_TRAIT_FALSE(std::constructible_from<ct::intrusive::List<Node>::Iterator, std::nullptr_t>);
  EXPECT_TRAIT_FALSE(std::constructible_from<ct::intrusive::List<Node>::ConstIterator, std::nullptr_t>);
}

TEST(IteratorTest, empty) {
  ct::intrusive::List<Node> list;
  EXPECT_TRUE(list.begin() == list.end());
  EXPECT_FALSE(list.begin() != list.end());

  const ct::intrusive::List<Node>& clist = list;
  EXPECT_TRUE(clist.begin() == clist.end());
  EXPECT_FALSE(clist.begin() != clist.end());
}

TEST(IteratorTest, default_ctor) {
  ct::intrusive::List<Node> list;
  Node a(1);
  list.push_back(a);

  ct::intrusive::List<Node>::Iterator i;
  ct::intrusive::List<Node>::ConstIterator j;

  i = list.begin();
  j = list.begin();

  EXPECT_EQ(*i, 1);
  EXPECT_EQ(*j, 1);

  EXPECT_NE(i, list.end());
  EXPECT_NE(j, list.end());
}

TEST(IteratorTest, copy_ctor) {
  ct::intrusive::List<Node> list;
  Node a(1);
  list.push_back(a);

  ct::intrusive::List<Node>::Iterator i = list.begin();

  ct::intrusive::List<Node>::Iterator i2 = i;
  ct::intrusive::List<Node>::ConstIterator j2 = i;

  EXPECT_EQ(*i2, 1);
  EXPECT_EQ(*j2, 1);

  EXPECT_NE(i2, list.end());
  EXPECT_NE(j2, list.end());
}

TEST(IteratorTest, increment_1) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3), d(4), e(5);
  mass_push_back(list, c, d, e);
  mass_push_front(list, b, a);

  ct::intrusive::List<Node>::Iterator i = list.begin();
  EXPECT_EQ(*i, 1);
  EXPECT_NE(i, list.end());
  EXPECT_EQ(*++i, 2);
  EXPECT_NE(i, list.end());
  EXPECT_EQ(*++i, 3);
  EXPECT_NE(i, list.end());
  EXPECT_EQ(*++i, 4);
  EXPECT_NE(i, list.end());
  EXPECT_EQ(*++i, 5);
  EXPECT_NE(i, list.end());
  EXPECT_EQ(++i, list.end());
}

TEST(IteratorTest, increment_1c) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3), d(4), e(5);
  mass_push_back(list, c, d, e);
  mass_push_front(list, b, a);

  ct::intrusive::List<Node>::ConstIterator i = list.begin();
  EXPECT_EQ(*i, 1);
  EXPECT_NE(i, list.end());
  EXPECT_EQ(*++i, 2);
  EXPECT_NE(i, list.end());
  EXPECT_EQ(*++i, 3);
  EXPECT_NE(i, list.end());
  EXPECT_EQ(*++i, 4);
  EXPECT_NE(i, list.end());
  EXPECT_EQ(*++i, 5);
  EXPECT_NE(i, list.end());
  EXPECT_EQ(++i, list.end());
}

TEST(IteratorTest, increment_2) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3), d(4), e(5);
  mass_push_back(list, c, d, e);
  mass_push_front(list, b, a);

  ct::intrusive::List<Node>::Iterator i = std::next(list.begin(), 1);
  EXPECT_EQ(*i, 2);
  ++(++i);
  EXPECT_EQ(*i, 4);
}

TEST(IteratorTest, increment_2c) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3), d(4), e(5);
  mass_push_back(list, c, d, e);
  mass_push_front(list, b, a);

  ct::intrusive::List<Node>::ConstIterator i = std::next(list.begin(), 1);
  EXPECT_EQ(*i, 2);
  ++(++i);
  EXPECT_EQ(*i, 4);
}

TEST(IteratorTest, increment_3) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3);
  mass_push_back(list, b, c);
  list.push_front(a);

  ct::intrusive::List<Node>::Iterator i = list.begin();
  EXPECT_EQ(*i, 1);
  ct::intrusive::List<Node>::Iterator j = i++;
  EXPECT_EQ(*i, 2);
  EXPECT_EQ(*j, 1);
  j = i++;
  EXPECT_EQ(*i, 3);
  EXPECT_EQ(*j, 2);
  j = i++;
  EXPECT_EQ(i, list.end());
  EXPECT_EQ(*j, 3);
}

TEST(IteratorTest, increment_3c) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3);
  mass_push_back(list, b, c);
  list.push_front(a);

  ct::intrusive::List<Node>::ConstIterator i = list.begin();
  EXPECT_EQ(*i, 1);
  ct::intrusive::List<Node>::ConstIterator j = i++;
  EXPECT_EQ(*i, 2);
  EXPECT_EQ(*j, 1);
  j = i++;
  EXPECT_EQ(*i, 3);
  EXPECT_EQ(*j, 2);
  j = i++;
  EXPECT_EQ(i, list.end());
  EXPECT_EQ(*j, 3);
}

TEST(IteratorTest, decrement_1) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3), d(4), e(5);
  mass_push_back(list, c, d, e);
  mass_push_front(list, b, a);

  ct::intrusive::List<Node>::Iterator i = list.end();
  EXPECT_NE(i, list.begin());
  EXPECT_EQ(*--i, 5);
  EXPECT_NE(i, list.begin());
  EXPECT_EQ(*--i, 4);
  EXPECT_NE(i, list.begin());
  EXPECT_EQ(*--i, 3);
  EXPECT_NE(i, list.begin());
  EXPECT_EQ(*--i, 2);
  EXPECT_NE(i, list.begin());
  EXPECT_EQ(*--i, 1);
  EXPECT_EQ(i, list.begin());
}

TEST(IteratorTest, decrement_1c) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3), d(4), e(5);
  mass_push_back(list, c, d, e);
  mass_push_front(list, b, a);

  ct::intrusive::List<Node>::ConstIterator i = list.end();
  EXPECT_NE(i, list.begin());
  EXPECT_EQ(*--i, 5);
  EXPECT_NE(i, list.begin());
  EXPECT_EQ(*--i, 4);
  EXPECT_NE(i, list.begin());
  EXPECT_EQ(*--i, 3);
  EXPECT_NE(i, list.begin());
  EXPECT_EQ(*--i, 2);
  EXPECT_NE(i, list.begin());
  EXPECT_EQ(*--i, 1);
  EXPECT_EQ(i, list.begin());
}

TEST(IteratorTest, decrement_2) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3), d(4), e(5);
  mass_push_back(list, c, d, e);
  mass_push_front(list, b, a);

  ct::intrusive::List<Node>::Iterator i = std::prev(list.end(), 2);
  EXPECT_EQ(*i, 4);
  --(--i);
  EXPECT_EQ(*i, 2);
}

TEST(IteratorTest, decrement_2c) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3), d(4), e(5);
  mass_push_back(list, c, d, e);
  mass_push_front(list, b, a);

  ct::intrusive::List<Node>::ConstIterator i = std::prev(list.end(), 2);
  EXPECT_EQ(*i, 4);
  --(--i);
  EXPECT_EQ(*i, 2);
}

TEST(IteratorTest, decrement_3) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3);
  mass_push_back(list, b, c);
  list.push_front(a);

  ct::intrusive::List<Node>::Iterator i = list.end();
  ct::intrusive::List<Node>::Iterator j = i--;
  EXPECT_EQ(*i, 3);
  EXPECT_EQ(j, list.end());
  j = i--;
  EXPECT_EQ(*i, 2);
  EXPECT_EQ(*j, 3);
  j = i--;
  EXPECT_EQ(*i, 1);
  EXPECT_EQ(*j, 2);
}

TEST(IteratorTest, decrement_3c) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3);
  mass_push_back(list, b, c);
  list.push_front(a);

  ct::intrusive::List<Node>::ConstIterator i = list.end();
  ct::intrusive::List<Node>::ConstIterator j = i--;
  EXPECT_EQ(*i, 3);
  EXPECT_EQ(list.end(), j);
  j = i--;
  EXPECT_EQ(*i, 2);
  EXPECT_EQ(*j, 3);
  j = i--;
  EXPECT_EQ(*i, 1);
  EXPECT_EQ(*j, 2);
}

TEST(IteratorTest, swap) {
  ct::intrusive::List<Node> list1;
  Node a(1), b(2), c(3);
  mass_push_back(list1, a, b, c);

  ct::intrusive::List<Node> list2;
  Node d(4), e(5), f(6);
  mass_push_back(list2, d, e, f);

  ct::intrusive::List<Node>::Iterator i = std::next(list1.begin(), 2);
  ct::intrusive::List<Node>::Iterator j = list2.begin();

  {
    using std::swap;
    swap(i, j);
  }

  list1.erase(j);
  list2.erase(i);

  expect_eq(list1, {1, 2});
  expect_eq(list2, {5, 6});
}

TEST(IteratorTest, conversions) {
  ct::intrusive::List<Node> list;

  ct::intrusive::List<Node>::Iterator i = list.end();
  ct::intrusive::List<Node>::ConstIterator ci = i;

  EXPECT_TRUE(i == i);
  EXPECT_TRUE(ci == ci);
  EXPECT_TRUE(i == ci);
  EXPECT_TRUE(ci == i);
  EXPECT_FALSE(i != i);
  EXPECT_FALSE(ci != ci);
  EXPECT_FALSE(i != ci);
  EXPECT_FALSE(ci != i);

  EXPECT_TRUE(std::as_const(i) == i);
  EXPECT_TRUE(std::as_const(ci) == ci);
  EXPECT_TRUE(std::as_const(i) == ci);
  EXPECT_TRUE(std::as_const(ci) == i);
  EXPECT_FALSE(std::as_const(i) != i);
  EXPECT_FALSE(std::as_const(ci) != ci);
  EXPECT_FALSE(std::as_const(i) != ci);
  EXPECT_FALSE(std::as_const(ci) != i);

  EXPECT_TRUE(i == std::as_const(i));
  EXPECT_TRUE(ci == std::as_const(ci));
  EXPECT_TRUE(i == std::as_const(ci));
  EXPECT_TRUE(ci == std::as_const(i));
  EXPECT_FALSE(i != std::as_const(i));
  EXPECT_FALSE(ci != std::as_const(ci));
  EXPECT_FALSE(i != std::as_const(ci));
  EXPECT_FALSE(ci != std::as_const(i));

  EXPECT_TRUE(std::as_const(i) == std::as_const(i));
  EXPECT_TRUE(std::as_const(ci) == std::as_const(ci));
  EXPECT_TRUE(std::as_const(i) == std::as_const(ci));
  EXPECT_TRUE(std::as_const(ci) == std::as_const(i));
  EXPECT_FALSE(std::as_const(i) != std::as_const(i));
  EXPECT_FALSE(std::as_const(ci) != std::as_const(ci));
  EXPECT_FALSE(std::as_const(i) != std::as_const(ci));
  EXPECT_FALSE(std::as_const(ci) != std::as_const(i));
}

TEST(IteratorTest, dereference_star) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3);
  mass_push_back(list, a, b, c);

  ct::intrusive::List<Node>::Iterator i = std::next(list.begin());
  EXPECT_EQ(*i, 2);
  EXPECT_EQ(&*i, &b);

  magic(*i);
  expect_eq(list, {1, 42, 3});

  ct::intrusive::List<Node>::ConstIterator j = std::next(list.begin(), 2);
  EXPECT_EQ(*j, 3);
  EXPECT_EQ(&*j, &c);

  magic(*j);
  expect_eq(list, {1, 42, 3});
}

TEST(IteratorTest, dereference_star_const) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3);
  mass_push_back(list, a, b, c);

  const ct::intrusive::List<Node>::Iterator i = std::next(list.begin());
  EXPECT_EQ(*i, 2);
  EXPECT_EQ(&*i, &b);

  magic(*i);
  expect_eq(list, {1, 42, 3});

  const ct::intrusive::List<Node>::ConstIterator j = std::next(list.begin(), 2);
  EXPECT_EQ(*j, 3);
  EXPECT_EQ(&*j, &c);

  magic(*j);
  expect_eq(list, {1, 42, 3});
}

TEST(IteratorTest, dereference_arrow) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3);
  mass_push_back(list, a, b, c);

  ct::intrusive::List<Node>::Iterator i = std::next(list.begin());
  EXPECT_EQ(i->value, 2);
  EXPECT_EQ(i.operator->(), &b);

  magic(*i.operator->());
  expect_eq(list, {1, 42, 3});

  ct::intrusive::List<Node>::ConstIterator j = std::next(list.begin(), 2);
  EXPECT_EQ(j->value, 3);
  EXPECT_EQ(j.operator->(), &c);

  magic(*j.operator->());
  expect_eq(list, {1, 42, 3});
}

TEST(IteratorTest, dereference_arrow_const) {
  ct::intrusive::List<Node> list;
  Node a(1), b(2), c(3);
  mass_push_back(list, a, b, c);

  const ct::intrusive::List<Node>::Iterator i = std::next(list.begin());
  EXPECT_EQ(i->value, 2);
  EXPECT_EQ(i.operator->(), &b);

  magic(*i.operator->());
  expect_eq(list, {1, 42, 3});

  const ct::intrusive::List<Node>::ConstIterator j = std::next(list.begin(), 2);
  EXPECT_EQ(j->value, 3);
  EXPECT_EQ(j.operator->(), &c);

  magic(*j.operator->());
  expect_eq(list, {1, 42, 3});
}

TEST(IteratorTest, list_modifications) {
  ct::intrusive::List<Node> list;

  ct::intrusive::List<Node>::Iterator it1 = list.begin();
  ct::intrusive::List<Node>::Iterator it2 = list.end();

  Node a(1);
  list.push_back(a);

  ct::intrusive::List<Node>::Iterator it3 = list.begin();
  ct::intrusive::List<Node>::Iterator it4 = list.end();

  EXPECT_EQ(it1, it4);
  EXPECT_EQ(it2, it4);
  EXPECT_NE(it3, it4);

  --it1;
  --it2;

  EXPECT_EQ(it1, it3);
  EXPECT_EQ(it2, it3);
}

} // namespace ct_test
