#include "intrusive-list.h"
#include "test-utils.h"

#include <utility>

namespace ct_test {

TEST(ElementTest, default_ctor) {
  struct DefaultConstructibleNode : ct::intrusive::ListElement<> {};

  [[maybe_unused]] DefaultConstructibleNode node;
}

TEST(ElementTest, copy_ctor) {
  ct::intrusive::List<CopyableNode> list;
  CopyableNode a(1);

  list.push_back(a);

  CopyableNode b = a;

  list.push_back(b);

  EXPECT_EQ(&list.front(), &a);
  EXPECT_EQ(&list.back(), &b);

  expect_eq(list, {1, 1});
}

TEST(ElementTest, copy_ctor_empty) {
  CopyableNode a(1);
  CopyableNode b = a;

  ct::intrusive::List<CopyableNode> list;
  list.push_back(b);

  EXPECT_EQ(list.front(), 1);
}

TEST(ElementTest, copy_assignment) {
  ct::intrusive::List<CopyableNode> list1;
  CopyableNode a(1), b(2), c(3);
  mass_push_back(list1, a, b, c);

  ct::intrusive::List<CopyableNode> list2;
  CopyableNode d(4), e(5), f(6);
  mass_push_back(list2, d, e, f);

  a = f;

  EXPECT_EQ(&list2.back(), &f);
  expect_eq(list1, {2, 3});
  expect_eq(list2, {4, 5, 6});
}

TEST(ElementTest, copy_assignment_self) {
  ct::intrusive::List<CopyableNode> list;
  CopyableNode a(1);
  list.push_back(a);

  a = a;

  EXPECT_FALSE(list.empty());
  EXPECT_EQ(&list.front(), &a);
}

TEST(ElementTest, copy_assignment_empty) {
  CopyableNode a(1), b(2);
  a = b;

  ct::intrusive::List<CopyableNode> list;
  list.push_back(a);

  EXPECT_EQ(list.front(), 2);
}

TEST(ElementTest, copy_assignment_empty_self) {
  CopyableNode a(1);
  a = a;

  ct::intrusive::List<CopyableNode> list;
  list.push_back(a);

  EXPECT_EQ(list.front(), 1);
}

TEST(ElementTest, copy_assignment_to_empty) {
  ct::intrusive::List<CopyableNode> list;
  CopyableNode a(1), b(2);

  list.push_back(b);

  a = b;

  EXPECT_FALSE(list.empty());
  EXPECT_EQ(&list.front(), &b);

  list.push_back(a);

  EXPECT_EQ(&list.back(), &a);

  expect_eq(list, {2, 2});
}

TEST(ElementTest, copy_assignment_from_empty) {
  ct::intrusive::List<CopyableNode> list;
  CopyableNode a(1), b(2);

  list.push_back(a);

  a = b;

  EXPECT_TRUE(list.empty());

  list.push_back(a);
  list.push_back(b);

  expect_eq(list, {2, 2});
}

TEST(ElementTest, move_ctor) {
  ct::intrusive::List<CopyableNode> list;
  CopyableNode a(1);

  list.push_back(a);

  CopyableNode b = std::move(a);

  EXPECT_FALSE(list.empty());
  EXPECT_EQ(&list.front(), &b);

  list.push_back(a);

  EXPECT_EQ(&list.back(), &a);

  expect_eq(list, {1, 1});
}

TEST(ElementTest, move_ctor_empty) {
  CopyableNode a(1);
  CopyableNode b = std::move(a);

  ct::intrusive::List<CopyableNode> list;
  list.push_back(b);

  EXPECT_EQ(list.front(), 1);
}

TEST(ElementTest, move_assignment) {
  ct::intrusive::List<CopyableNode> list1;
  CopyableNode a(1), b(2), c(3);
  mass_push_back(list1, a, b, c);

  ct::intrusive::List<CopyableNode> list2;
  CopyableNode d(4), e(5), f(6);
  mass_push_back(list2, d, e, f);

  a = std::move(f);

  EXPECT_EQ(&list2.back(), &a);
  expect_eq(list1, {2, 3});
  expect_eq(list2, {4, 5, 6});
}

TEST(ElementTest, move_assignment_self) {
  ct::intrusive::List<CopyableNode> list;
  CopyableNode a(1);
  list.push_back(a);

  a = std::move(a);

  EXPECT_FALSE(list.empty());
  EXPECT_EQ(&a, &list.front());
}

TEST(ElementTest, move_assignment_empty) {
  CopyableNode a(1), b(2);
  a = std::move(b);

  ct::intrusive::List<CopyableNode> list;
  list.push_back(a);

  EXPECT_EQ(list.front(), 2);
}

TEST(ElementTest, move_assignment_empty_self) {
  CopyableNode a(1);
  a = std::move(a);

  ct::intrusive::List<CopyableNode> list;
  list.push_back(a);

  EXPECT_EQ(list.front(), 1);
}

TEST(ElementTest, move_assignment_to_empty) {
  ct::intrusive::List<CopyableNode> list;
  CopyableNode a(1), b(2);

  list.push_back(b);

  a = std::move(b);

  EXPECT_FALSE(list.empty());
  EXPECT_EQ(&list.front(), &a);

  list.push_back(b);

  EXPECT_EQ(&list.back(), &b);

  expect_eq(list, {2, 2});
}

TEST(ElementTest, move_assignment_from_empty) {
  ct::intrusive::List<CopyableNode> list;
  CopyableNode a(1), b(2);

  list.push_back(a);

  a = std::move(b);

  EXPECT_TRUE(list.empty());

  list.push_back(a);
  list.push_back(b);

  expect_eq(list, {2, 2});
}

} // namespace ct_test
