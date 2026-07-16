#include "shared-generator.h"

#include <gtest/gtest.h>

#include <cstddef>
#include <optional>
#include <random>
#include <vector>

// 98
// 49

namespace ct_test {

TEST(GeneratorSharedTest, CopyConstructor) {
  auto gen = [] -> ct::SharedGenerator<int> {
    // std::cout << "[cor] 1" << std::endl;
    co_yield 10;
    // std::cout << "[cor] 2" << std::endl;
    co_yield 20;
    // std::cout << "[cor] 3" << std::endl;
    co_yield 30;
    // std::cout << "[cor] 4" << std::endl;
    co_yield 40;
    // std::cout << "[cor] 5" << std::endl;
    co_yield 50;
    // std::cout << "[cor] 6" << std::endl;
    co_yield 60;
    // std::cout << "[cor] 7" << std::endl;
  };

  auto g1 = gen();
  // тут все хорошо в плане общности фрейма PromiseType для g1 и 2.
  auto g2 = g1;

  EXPECT_EQ(g1.next(), 10);
  // std::cout << "[main] copy ctor 1" << std::endl;
  EXPECT_EQ(g2.next(), 20);

  auto g3 = g2;

  EXPECT_EQ(g2.next(), 30);
  EXPECT_EQ(g3.next(), 40);
  EXPECT_EQ(g1.next(), 50);
  EXPECT_EQ(g3.next(), 60);

  EXPECT_EQ(g1.next(), std::nullopt);
  EXPECT_EQ(g2.next(), std::nullopt);
  EXPECT_EQ(g3.next(), std::nullopt);
  // std::cout << "[main] copy ctor 15" << std::endl;
}

TEST(GeneratorSharedTest, CopyConstructorFromEmpty) {
  ct::SharedGenerator<int> g1;
  ct::SharedGenerator<int> g2 = g1;

  EXPECT_EQ(g1.next(), std::nullopt);
  EXPECT_EQ(g2.next(), std::nullopt);
}

TEST(GeneratorSharedTest, CopyAssignment) {
  auto gen = [](int x) -> ct::SharedGenerator<int> {
    while (true) {
      co_yield x;
      x += 10;
    }
  };

  auto g1 = gen(10);
  auto g2 = gen(15);
  auto g3 = gen(17);

  EXPECT_EQ(g1.next(), 10);
  EXPECT_EQ(g2.next(), 15);

  // std::cout << "[test] before g3 = g1" << std::endl;
  g3 = g1;
  EXPECT_EQ(g1.next(), 20);
  EXPECT_EQ(g2.next(), 25);
  EXPECT_EQ(g3.next(), 30);

  g1 = g2;
  EXPECT_EQ(g1.next(), 35);
  EXPECT_EQ(g2.next(), 45);
  EXPECT_EQ(g3.next(), 40);
}

TEST(GeneratorUniqueTest, CopyAssignmentEmptyFromEmpty) {
  ct::SharedGenerator<int> g1;
  ct::SharedGenerator<int> g2;

  g2 = g1;
  // EXPECT_EQ(g1.next(), std::nullopt);
  // EXPECT_EQ(g2.next(), std::nullopt);
}

TEST(GeneratorUniqueTest, CopyAssignmentEmptyFromNonEmpty) {
  auto gen = [] -> ct::SharedGenerator<int> { // leak
    co_yield 10;
    co_yield 20;
  };

  ct::SharedGenerator<int> g1;
  ct::SharedGenerator<int> g2 = gen();

  g1 = g2;
  EXPECT_EQ(g1.next(), 10); // here
  EXPECT_EQ(g2.next(), 20);
}

TEST(GeneratorUniqueTest, CopyAssignmentNonEmptyFromEmpty) {
  auto gen = [] -> ct::SharedGenerator<int> { // leak
    co_yield 10;
    co_yield 20;
  };

  ct::SharedGenerator<int> g1 = gen(); // here
  ct::SharedGenerator<int> g2;

  g1 = g2;
  EXPECT_EQ(g1.next(), std::nullopt);
  EXPECT_EQ(g2.next(), std::nullopt);
}

TEST(GeneratorSharedTest, Exception) {
  struct Error {};

  auto gen = [] -> ct::SharedGenerator<int> {
    co_yield 10;
    throw Error{};
  };

  auto g1 = gen();
  auto g2 = g1;

  EXPECT_EQ(g1.next(), 10);
  EXPECT_THROW(g2.next(), Error);
  EXPECT_EQ(g1.next(), std::nullopt);
  EXPECT_EQ(g2.next(), std::nullopt);
}

TEST(GeneratorSharedTest, HeavyConcurrency) {
  constexpr std::size_t N_VALUES = 100'000;
  constexpr std::size_t N_GENERATORS = 20;

  auto gen = [] -> ct::SharedGenerator<int> {
    for (int i = 1; i <= static_cast<int>(N_VALUES); ++i) {
      co_yield i;
    }
  };

  std::vector generators(N_GENERATORS, gen());

  std::minstd_rand rng(1337);
  std::uniform_int_distribution dist(0, static_cast<int>(N_GENERATORS - 1));

  for (int i = 1; i <= static_cast<int>(N_VALUES); ++i) {
    ASSERT_EQ(generators[dist(rng)].next(), i);
  }
  ASSERT_EQ(generators[dist(rng)].next(), std::nullopt);
}

} // namespace ct_test
