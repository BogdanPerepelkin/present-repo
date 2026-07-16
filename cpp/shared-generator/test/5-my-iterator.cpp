// #include "shared-generator.h"

// #include <gtest/gtest.h>

// #include <optional>
// #include <ranges>
// #include <utility>
// #include <vector>

// namespace ct_test {

// TEST(GeneratorIteratorTest, Simple) {
//   auto gen = [] -> ct::SharedGenerator<int> {
//     co_yield 10;
//     co_yield 20;
//   };
//   auto g = gen();

// std::vector<int> forResult;

// for (auto i : g) {
//   forResult.push_back(*i);
// }

// std::vector<int> result;
// result.push_back(10);
// result.push_back(20);

// EXPECT_EQ(forResult, result);
// EXPECT_EQ(g.next(), std::nullopt);
// }

// TEST(GeneratorIteratorTest, Empty) {
//   auto gen = [] -> ct::SharedGenerator<int> {
//     co_yield 10;
//     co_yield 20;
//   };
//   auto g = gen();
//   if (!std::ranges::empty(g)) {
//     EXPECT_EQ(g.next(), 10);
//     EXPECT_EQ(g.next(), 20);
//   }
//   EXPECT_EQ(g.next(), std::nullopt);
// }

// } // namespace ct_test
