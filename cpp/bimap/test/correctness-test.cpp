#include "bimap.h"
#include "test-classes.h"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <iostream>
#include <random>

template class ct::Bimap<int, ct_test::NonDefaultConstructible>;
template class ct::Bimap<ct_test::NonDefaultConstructible, int>;

namespace ct_test {

using ct::Bimap;

TEST_CASE("Simple") {
  Bimap<int, int> b;
  b.insert(4, 4);
  CHECK(b.at_left(4) == 4);
  CHECK(b.at_right(4) == 4);
}

TEST_CASE("Leaks") {
  static constexpr size_t N = 10'000;

  Bimap<std::uint64_t, std::uint64_t> b;

  std::mt19937 rng(std::mt19937::default_seed);
  std::uniform_int_distribution<std::uint64_t> dist(0, N);

  for (size_t i = 0; i < N; i++) {
    std::uint64_t left = dist(rng);
    std::uint64_t right = dist(rng);
    b.insert(left, right);
  }

  SUCCEED();
}

TEST_CASE("Custom comparator") {
  Bimap<int, int, std::greater<>> b;
  b.insert(3, 4);
  b.insert(1, 5);
  b.insert(10, -10);

  int prev = *b.begin_left();
  for (auto it = std::next(b.begin_left()); it != b.end_left(); it++) {
    REQUIRE(prev > *it);
    prev = *it;
  }
  prev = *b.begin_right();
  for (auto it = std::next(b.begin_right()); it != b.end_right(); it++) {
    REQUIRE(prev < *it);
    prev = *it;
  }
}

TEST_CASE("Custom parameterized comparator") {
  using Vec = std::pair<int, int>;
  Bimap<Vec, Vec, VectorCompare, VectorCompare> b(VectorCompare{VectorCompare::DistanceType::manhattan});
  b.insert({0, 1}, {35, 3});
  b.insert({20, -20}, {20, -20});
  b.insert({35, 3}, {3, -1});
  b.insert({3, -1}, {0, 1});

  std::vector<Vec> correct_left = {{0, 1}, {3, -1}, {35, 3}, {20, -20}};
  std::vector<Vec> correct_right = {{0, 1}, {3, -1}, {20, -20}, {35, 3}};

  auto lit = b.begin_left();
  auto rit = b.begin_right();
  for (int i = 0; i < 4; i++) {
    REQUIRE(*lit++ == correct_left[i]);
    REQUIRE(*rit++ == correct_right[i]);
  }
}

TEST_CASE("Comparator with state") {
  Bimap<int, int, StateComparator, StateComparator> a(StateComparator(true));
  a.insert(1, 2);
  a.insert(3, 4);
  a.insert(5, 6);
  CHECK(*a.begin_left() == 5);
  CHECK(*a.begin_right() == 2);

  Bimap a_copy = a;
  CHECK(a == a_copy);

  Bimap<int, int, StateComparator, StateComparator> b(StateComparator(false), StateComparator(true));
  b.insert(11, 12);
  b.insert(13, 14);
  b.insert(15, 16);
  CHECK(*b.begin_left() == 11);
  CHECK(*b.begin_right() == 16);

  using std::swap;
  swap(a, b);
  CHECK(*a.begin_left() == 11);
  CHECK(*a.begin_right() == 16);
  CHECK(*b.begin_left() == 5);
  CHECK(*b.begin_right() == 2);
}

TEST_CASE("Non-copyable comparator") {
  Bimap<int, int, NonCopyableComparator, NonCopyableComparator> a;
  a.insert(1, 4);
  a.insert(8, 8);
  a.insert(25, 17);
  a.insert(13, 37);

  Bimap<int, int, NonCopyableComparator, NonCopyableComparator> b = std::move(a);
  CHECK(b.size() == 4);
  CHECK(a.size() == 0);

  a = std::move(b);
  CHECK(a.size() == 4);
  CHECK(b.size() == 0);
}

TEST_CASE("Non-default-constructible comparator") {
  using Cmp = NonDefaultConstructibleComparator;

  Bimap<int, int, Cmp, Cmp> a(Cmp::create(), Cmp::create());
  a.insert(1, 4);
  a.insert(8, 8);
  a.insert(25, 17);
  a.insert(13, 37);

  Bimap<int, int, Cmp, Cmp> b = a;
  CHECK(a == b);

  Bimap<int, int, Cmp, Cmp> c = std::move(a);
  CHECK(b == c);
}

TEST_CASE("Copying") {
  Bimap<int, int> b;
  b.insert(3, 4);

  Bimap<int, int> b1 = b;
  CHECK(*b.find_left(3).flip() == 4);

  b1.insert(4, 5);
  CHECK(b.find_left(4) == b.end_left());

  b1.insert(10, -10);
  b = b1;
  CHECK(b.find_right(-10) != b.end_right());
}

TEST_CASE("Insert") {
  Bimap<int, int> b;
  auto it1 = b.insert(4, 10);
  auto it2 = b.insert(10, 4);
  CHECK(b.find_left(4) == it1);
  CHECK(b.find_right(4).flip() == it2);
  CHECK(*b.find_right(4).flip() == 10);
  CHECK(b.at_left(10) == 4);
}

TEST_CASE("Insert existing") {
  Bimap<int, int> b;
  b.insert(1, 2);
  b.insert(2, 3);
  b.insert(3, 4);
  CHECK(b.size() == 3);

  SECTION("Existing left") {
    auto it = b.insert(2, -1);
    CHECK(it == b.end_left());
    CHECK(b.at_left(2) == 3);
    CHECK(b.size() == 3);
  }

  SECTION("Existing right") {
    auto it = b.insert(-1, 2);
    CHECK(it == b.end_left());
    CHECK(b.at_right(2) == 1);
    CHECK(b.size() == 3);
  }
}

TEST_CASE("Insert xvalue") {
  SECTION("Move right") {
    Bimap<int, TestObject> b;
    TestObject x1(3);
    TestObject x2(3);
    b.insert(4, std::move(x1));
    CHECK(x1.a == 0);
    CHECK(b.at_right(x2) == 4);
    CHECK(b.at_left(4) == x2);
  }
  SECTION("Move left") {
    Bimap<TestObject, int> b;
    TestObject x1(4);
    TestObject x2(4);
    b.insert(std::move(x1), 3);
    CHECK(x1.a == 0);
    CHECK(b.at_left(x2) == 3);
    CHECK(b.at_right(3) == x2);
  }
  SECTION("Move both") {
    Bimap<TestObject, TestObject> b;
    TestObject x1(6);
    TestObject x2(2);
    b.insert(std::move(x1), std::move(x2));
    CHECK(x1.a == 0);
    CHECK(x2.a == 0);
    auto it = b.find_left(TestObject(6));
    CHECK(it->a == 6);
    CHECK(it.flip()->a == 2);
  }
  SECTION("Already exists") {
    Bimap<TestObject, TestObject> b;
    b.insert(TestObject(5), TestObject(2));

    TestObject x1(6);
    TestObject x2(2);

    b.insert(std::move(x1), std::move(x2));

    CHECK(x1.a == 6);
    CHECK(x2.a == 2);
    CHECK(b.size() == 1);

    auto it = b.find_right(TestObject(2));
    CHECK(it.flip()->a == 5);
  }
}

TEST_CASE("At") {
  Bimap<int, int> b;
  b.insert(4, 3);

  CHECK_THROWS_AS(b.at_left(1), std::out_of_range);
  CHECK_THROWS_AS(b.at_right(300), std::out_of_range);
  CHECK(b.at_left(4) == 3);
  CHECK(b.at_right(3) == 4);
}

TEST_CASE("At-or-default") {
  Bimap<int, int> b;
  b.insert(4, 2);

  CHECK(b.at_left_or_default(4) == 2);
  CHECK(b.at_right_or_default(2) == 4);

  CHECK(b.at_left_or_default(5) == 0);
  CHECK(b.at_right(0) == 5);

  CHECK(b.at_right_or_default(1) == 0);
  CHECK(b.at_left(0) == 1);

  CHECK(b.at_left_or_default(42) == 0); // (5, 0) is replaced with (42, 0)
  CHECK(b.at_right(0) == 42);
  CHECK(b.at_left(42) == 0);

  CHECK(b.at_left_or_default(-42) == 0); // (42, 0) is replaced with (-42, 0)
  CHECK(b.at_right(0) == -42);
  CHECK(b.at_left(-42) == 0);

  CHECK(b.at_right_or_default(1000) == 0); // (0, 1) is replaced with (0, 1000)
  CHECK(b.at_left(0) == 1000);
  CHECK(b.at_right(1000) == 0);

  CHECK(b.at_right_or_default(-1000) == 0); // (0, 1000) is replaced with (0, -1000)
  CHECK(b.at_left(0) == -1000);
  CHECK(b.at_right(-1000) == 0);
}

TEST_CASE("At-or-default does not invoke copy assignment") {
  Bimap<NonCopyAssignable, NonCopyAssignable> b;
  b.insert(NonCopyAssignable(4), NonCopyAssignable(2));

  CHECK(b.at_left_or_default(NonCopyAssignable(4)) == NonCopyAssignable(2));
  CHECK(b.at_right_or_default(NonCopyAssignable(2)) == NonCopyAssignable(4));

  CHECK(b.at_left_or_default(NonCopyAssignable(5)) == NonCopyAssignable(0));
  CHECK(b.at_right_or_default(NonCopyAssignable(1)) == NonCopyAssignable(0));
}

TEST_CASE("Flip end iterator") {
  Bimap<int, int> b;
  CHECK(b.end_left().flip() == b.end_right());
  CHECK(b.end_right().flip() == b.end_left());

  b.insert(1, 2);
  b.insert(-3, 5);
  b.insert(1000, -100000);

  CHECK(b.end_left().flip() == b.end_right());
  CHECK(b.end_right().flip() == b.end_left());
}

TEST_CASE("Flip end iterator with custom comparator") {
  Bimap<int, int, StateComparator, StateComparator> b;
  CHECK(b.end_left().flip() == b.end_right());
  CHECK(b.end_right().flip() == b.end_left());

  b.insert(1, 2);
  b.insert(-3, 5);
  b.insert(1000, -100000);

  CHECK(b.end_left().flip() == b.end_right());
  CHECK(b.end_right().flip() == b.end_left());
}

TEST_CASE("Double flip") {
  Bimap<int, int> b;
  b.insert(100, -100);
  b.insert(-100, 100);
  b.insert(-10, 10);
  b.insert(-12, -10);

  auto lit = b.begin_left();
  auto rit = b.begin_right();
  while (lit != b.end_left() && rit != b.end_right()) {
    CHECK(lit.flip().flip() == lit);
    CHECK(rit.flip().flip() == rit);
    ++lit;
    ++rit;
  }
}

TEST_CASE("Find") {
  Bimap<int, int> b;
  b.insert(3, 4);
  b.insert(4, 5);
  b.insert(42, 1000);

  CHECK(*b.find_left(3).flip() == 4);
  CHECK(*b.find_right(5).flip() == 4);
  CHECK(b.find_left(3436) == b.end_left());
  CHECK(b.find_right(-1000) == b.end_right());
}

TEST_CASE("Find with non-copyable type") {
  Bimap<TestObject, TestObject> b;
  b.insert(TestObject(3), TestObject(4));
  b.insert(TestObject(4), TestObject(5));
  b.insert(TestObject(42), TestObject(1000));

  CHECK(*b.find_right(TestObject(5)).flip() == TestObject(4));
  CHECK(*b.find_left(TestObject(3)).flip() == TestObject(4));
  CHECK(b.find_left(TestObject(3436)) == b.end_left());
  CHECK(b.find_right(TestObject(-1000)) == b.end_right());
}

TEST_CASE("Empty") {
  Bimap<int, int> b;
  CHECK(b.empty());

  auto it = b.insert(1, 1);
  CHECK_FALSE(b.empty());

  b.erase_left(it);
  CHECK(b.empty());
}

TEST_CASE("Erase iterator") {
  Bimap<int, int> b;

  auto it = b.insert(1, 2);
  b.insert(5, 10);
  b.insert(100, 200);

  auto it1 = b.erase_left(it);
  CHECK(b.size() == 2);
  CHECK(*it1 == 5);

  it = b.insert(-1, -2);
  auto itr = b.erase_right(it.flip());
  CHECK(b.size() == 2);
  CHECK(*itr == 10);
}

TEST_CASE("Erase value") {
  Bimap<int, int> b;

  b.insert(111, 222);
  b.insert(333, 444);
  CHECK(b.erase_left(111));
  CHECK(b.size() == 1);
  CHECK_FALSE(b.erase_right(333333));
  CHECK(b.size() == 1);
  CHECK(b.erase_right(444));
  CHECK(b.empty());
}

TEST_CASE("Erase range") {
  Bimap<int, int> b;

  b.insert(1, 2);
  auto f = b.insert(2, 3);
  b.insert(3, 4);
  auto l = b.insert(4, 5);
  b.insert(5, 6);

  auto it = b.erase_left(f, l);
  CHECK(*it == 4);
  CHECK(b.size() == 3);

  auto f1 = b.insert(100, 4).flip();
  auto l1 = b.insert(200, 10).flip();

  auto it1 = b.erase_right(f1, l1);
  CHECK(*it1 == 10);
  CHECK(b.size() == 2);

  b.erase_left(b.begin_left(), b.end_left());
  CHECK(b.empty());
}

TEST_CASE("Lower bound") {
  std::vector<std::pair<int, int>> data = {{1, 2}, {2, 3}, {3, 4}, {8, 16}, {32, 66}};

  std::ranges::sort(data);

  do {
    Bimap<int, int> b;

    for (const auto& p : data) {
      b.insert(p.first, p.second);
    }

    CHECK(*b.lower_bound_left(5) == 8);
    CHECK(*b.lower_bound_right(4) == 4);
    CHECK(*b.lower_bound_left(4).flip() == 16);
    CHECK(b.lower_bound_right(100) == b.end_right());
    CHECK(b.lower_bound_left(100) == b.end_left());

  } while (std::ranges::next_permutation(data).found);
}

TEST_CASE("Lower bound with non-copyable type") {
  Bimap<TestObject, TestObject> b;
  b.insert(TestObject(1), TestObject(2));
  b.insert(TestObject(2), TestObject(3));
  b.insert(TestObject(3), TestObject(4));
  b.insert(TestObject(8), TestObject(16));
  b.insert(TestObject(32), TestObject(66));

  CHECK(*b.lower_bound_left(TestObject(5)) == TestObject(8));
  CHECK(*b.lower_bound_right(TestObject(4)) == TestObject(4));
  CHECK(*b.lower_bound_left(TestObject(4)).flip() == TestObject(16));
  CHECK(b.lower_bound_right(TestObject(100)) == b.end_right());
  CHECK(b.lower_bound_left(TestObject(100)) == b.end_left());
}

TEST_CASE("Upper bound") {
  std::vector<std::pair<int, int>> data = {{1, 2}, {2, 3}, {3, 4}, {8, 16}, {32, 66}};

  std::ranges::sort(data);

  do {
    Bimap<int, int> b;

    for (const auto& p : data) {
      b.insert(p.first, p.second);
    }

    CHECK(*b.upper_bound_left(5) == 8);
    CHECK(*b.upper_bound_right(-100) == 2);
    CHECK(b.upper_bound_right(100) == b.end_right());
    CHECK(b.upper_bound_left(400) == b.end_left());

  } while (std::ranges::next_permutation(data).found);
}

TEST_CASE("Upper bound with non-copyable type") {
  Bimap<TestObject, TestObject> b;
  b.insert(TestObject(1), TestObject(2));
  b.insert(TestObject(2), TestObject(3));
  b.insert(TestObject(3), TestObject(4));
  b.insert(TestObject(8), TestObject(16));
  b.insert(TestObject(32), TestObject(66));

  CHECK(*b.upper_bound_left(TestObject(5)) == TestObject(8));
  CHECK(*b.upper_bound_right(TestObject(-100)) == TestObject(2));
  CHECK(b.upper_bound_right(TestObject(100)) == b.end_right());
  CHECK(b.upper_bound_left(TestObject(400)) == b.end_left());
}

TEST_CASE("Copy constructor") {
  Bimap<int, int> a;
  a.insert(1, 4);
  a.insert(8, 8);
  a.insert(25, 17);
  a.insert(13, 37);

  auto b = a;
  CHECK(a.size() == b.size());
  CHECK(a == b);
}

TEST_CASE("Copy assignment") {
  Bimap<int, int> a;
  a.insert(1, 4);
  a.insert(8, 8);
  a.insert(25, 17);
  a.insert(13, 37);

  Bimap<int, int> b;
  b.insert(2, 5);
  b.insert(5, 2);

  b = a;
  CHECK(a.size() == b.size());
  CHECK(a == b);
}

TEST_CASE("Copy assignment to self") {
  Bimap<int, int> a;
  a.insert(1, 4);
  a.insert(8, 8);
  a.insert(25, 17);
  a.insert(13, 37);

  Bimap<int, int> a_copy = a;
  a = a;
  CHECK(a.size() == a_copy.size());
  CHECK(a == a_copy);
}

TEST_CASE("Move constructor") {
  Bimap<AddressCheckingObject, int> a;
  a.insert(1, 4);
  a.insert(8, 8);
  a.insert(25, 17);
  a.insert(13, 37);

  {
    Bimap<AddressCheckingObject, int> a_copy = a;
    AddressCheckingObject::set_copy_throw_countdown(1);

    Bimap<AddressCheckingObject, int> b = std::move(a);
    CHECK(b.size() == a_copy.size());
    CHECK(b == a_copy);
  }

  AddressCheckingObject::expect_no_instances();
  AddressCheckingObject::set_copy_throw_countdown(0);
}

TEST_CASE("Move constructor with expiring comparator") {
  using Bimap = Bimap<int, int, ExpiringComparator, ExpiringComparator>;

  Bimap a;
  a.insert(1, 4);
  a.insert(8, 8);
  a.insert(25, 17);
  a.insert(13, 37);

  Bimap a_copy = a;

  Bimap b = std::move(a);
  CHECK(b == a_copy);
}

TEST_CASE("Move constructor with tracking comparator") {
  using Bimap = Bimap<int, int, TrackingComparator, TrackingComparator>;

  bool cmp_left = false;
  bool cmp_right = false;

  Bimap a(TrackingComparator{&cmp_left}, TrackingComparator{&cmp_right});
  a.insert(1, 4);
  a.insert(8, 8);

  Bimap b = std::move(a);

  cmp_left = cmp_right = false;

  CHECK(b.at_left(1) == 4);
  CHECK(b.at_left(8) == 8);
  CHECK(cmp_left == true);
  CHECK(cmp_right == false);

  CHECK(b.at_right(4) == 1);
  CHECK(b.at_right(8) == 8);
  CHECK(cmp_right == true);
}

TEST_CASE("Move assignment") {
  Bimap<AddressCheckingObject, int> a;
  a.insert(1, 4);
  a.insert(8, 8);
  a.insert(25, 17);
  a.insert(13, 37);

  Bimap<AddressCheckingObject, int> b;
  b.insert(2, 5);
  b.insert(5, 2);

  Bimap<AddressCheckingObject, int> a_copy = a;
  AddressCheckingObject::set_copy_throw_countdown(1);

  b = std::move(a);
  CHECK(b.size() == a_copy.size());
  CHECK(b == a_copy);

  AddressCheckingObject::set_copy_throw_countdown(0);
}

TEST_CASE("Move assignment to self") {
  Bimap<int, int> a;
  a.insert(1, 4);
  a.insert(8, 8);
  a.insert(25, 17);
  a.insert(13, 37);

  Bimap<int, int> a_copy = a;
  a = std::move(a);
  CHECK(a.size() == a_copy.size());
  CHECK(a == a_copy);
}

TEST_CASE("Move assignment with expiring comparator") {
  using Bimap = Bimap<int, int, ExpiringComparator, ExpiringComparator>;

  Bimap a;
  a.insert(1, 4);
  a.insert(8, 8);
  a.insert(25, 17);
  a.insert(13, 37);

  Bimap b;
  b.insert(2, 5);
  b.insert(5, 2);

  Bimap a_copy = a;

  b = std::move(a);
  CHECK(b == a_copy);
}

TEST_CASE("Move assignment with tracking comparator") {
  using Bimap = Bimap<int, int, TrackingComparator, TrackingComparator>;

  bool cmp1_left = false;
  bool cmp1_right = false;

  Bimap a(TrackingComparator{&cmp1_left}, TrackingComparator{&cmp1_right});
  a.insert(1, 4);
  a.insert(8, 8);

  bool cmp2_left = false;
  bool cmp2_right = false;
  Bimap b(TrackingComparator{&cmp2_left}, TrackingComparator{&cmp2_right});
  b.insert(2, 5);
  b.insert(5, 2);

  b = std::move(a);

  cmp1_left = cmp1_right = cmp2_left = cmp2_right = false;

  CHECK(b.at_left(1) == 4);
  CHECK(b.at_left(8) == 8);
  CHECK(cmp1_left == true);
  CHECK(cmp1_right == false);

  CHECK(b.at_right(4) == 1);
  CHECK(b.at_right(8) == 8);
  CHECK(cmp1_right == true);

  CHECK(cmp2_left == false);
  CHECK(cmp2_right == false);
}

TEST_CASE("Equivalence") {
  Bimap<int, int> a;
  Bimap<int, int> b;
  CHECK(a == b);
  CHECK_FALSE(a != b);

  a.insert(1, 2);
  a.insert(3, 4);
  b.insert(1, 2);
  CHECK_FALSE(a == b);
  CHECK(a != b);

  b.erase_left(1);
  b.insert(1, 4);
  b.insert(3, 2);
  CHECK_FALSE(a == b);
  CHECK(a != b);

  CHECK(a.end_left().flip() == a.end_right());
  CHECK(a.end_right().flip() == a.end_left());

  a.erase_left(1);
  a.erase_right(4);
  a.insert(3, 2);
  a.insert(1, 4);
  CHECK(a == b);
  CHECK_FALSE(a != b);
}

TEST_CASE("Equivalence with custom comparator") {
  using Element = IncomparableInt;
  using Comparator = IncomparableIntCustomComparator;
  Bimap<Element, Element, Comparator, Comparator> a;
  Bimap<Element, Element, Comparator, Comparator> b;
  a.insert(1, 2);
  a.insert(3, 4);
  b.insert(1, 2);
  b.insert(3, 4);
  CHECK(a == b);
  CHECK_FALSE(a != b);
}

TEST_CASE("Iterator traits") {
  using BM = Bimap<int, double>;
  STATIC_CHECK(std::bidirectional_iterator<BM::LeftIterator>);
  STATIC_CHECK(std::bidirectional_iterator<BM::RightIterator>);
  STATIC_CHECK(std::is_same_v<std::iterator_traits<BM::LeftIterator>::value_type, int>);
  STATIC_CHECK(std::is_same_v<std::iterator_traits<BM::RightIterator>::value_type, double>);
  STATIC_CHECK(std::is_same_v<std::iterator_traits<BM::LeftIterator>::reference, const int&>);
  STATIC_CHECK(std::is_same_v<std::iterator_traits<BM::RightIterator>::reference, const double&>);
  STATIC_CHECK(std::is_same_v<std::iterator_traits<BM::LeftIterator>::pointer, const int*>);
  STATIC_CHECK(std::is_same_v<std::iterator_traits<BM::RightIterator>::pointer, const double*>);
}

TEST_CASE("Iterator sizeof") {
  using BM = Bimap<int, double>;
  STATIC_CHECK(sizeof(BM::LeftIterator) <= sizeof(void*));
  STATIC_CHECK(sizeof(BM::RightIterator) <= sizeof(void*));
}

TEST_CASE("Iterator operations") {
  Bimap<int, int> b;
  b.insert(3, 4);
  b.insert(100, 10);
  auto it = b.insert(-10, 100);

  auto it_next = it;
  CHECK(it_next++ == it);

  CHECK(++it == it_next--);
  CHECK(--it == it_next);
}

TEST_CASE("Iteration") {
  Bimap<int, int> b;
  b.insert(1, 0);
  b.insert(2, 10);
  b.insert(3, 100);

  std::vector<int> left_values;
  for (auto it = b.begin_left(); it != b.end_left(); ++it) {
    left_values.push_back(*it);
  }
  std::vector<int> left_values_inv;
  for (auto it = b.end_left(); it != b.begin_left();) {
    --it;
    left_values_inv.push_back(*it);
  }
  std::ranges::reverse(left_values_inv);
  CHECK(left_values == left_values_inv);

  std::vector<int> right_values;
  for (auto it = b.begin_right(); it != b.end_right(); ++it) {
    right_values.push_back(*it);
  }
  std::vector<int> right_values_inv;
  for (auto it = b.end_right(); it != b.begin_right();) {
    --it;
    right_values_inv.push_back(*it);
  }
  std::ranges::reverse(right_values_inv);
  CHECK(right_values == right_values_inv);
}

TEST_CASE("Swap") {
  Bimap<int, int> b;
  Bimap<int, int> b1;
  b.insert(3, 4);
  b1.insert(4, 3);
  CHECK(*b.find_left(3) == 3);
  CHECK(*b1.find_right(3) == 3);

  using std::swap;
  swap(b, b1);
  CHECK(*b1.find_left(3) == 3);
  CHECK(*b.find_right(3) == 3);
}

TEST_CASE("Swap with tracking comparator") {
  using Bimap = Bimap<int, int, TrackingComparator, TrackingComparator>;

  bool cmp1_left = false;
  bool cmp1_right = false;
  bool cmp2_left = false;
  bool cmp2_right = false;

  Bimap b1(TrackingComparator{&cmp1_left}, TrackingComparator{&cmp1_right});
  Bimap b2(TrackingComparator{&cmp2_left}, TrackingComparator{&cmp2_right});
  b2.insert(3, 4);
  b2.insert(4, 5);

  using std::swap;
  swap(b1, b2);

  cmp1_left = cmp1_right = cmp2_left = cmp2_right = false;

  CHECK(b1.at_left(3) == 4);
  CHECK(b1.at_left(4) == 5);
  CHECK(cmp2_left == true);
  CHECK(cmp2_right == false);

  CHECK(b1.at_right(4) == 3);
  CHECK(b1.at_right(5) == 4);
  CHECK(cmp2_right == true);

  CHECK(cmp1_left == false);
  CHECK(cmp1_right == false);
}

} // namespace ct_test
