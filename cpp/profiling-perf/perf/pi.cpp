#include <benchmark/benchmark.h>

#include <cstdint>
#include <cstdlib>
#include <random>

namespace {

thread_local std::mt19937 rng(std::mt19937::default_seed);

// Computes PI using Monte Carlo method
// DO NOT change the algorithm
struct MonteCarlo {
  uint64_t total = 0;
  uint64_t inside_circle = 0;

  MonteCarlo() {}

  void operator()() {
    std::uniform_int_distribution<uint64_t> distrib(0, SQUARE_SIZE - 1);
    uint64_t x = distrib(rng);
    uint64_t y = distrib(rng);

    ++total;
    if (x * x + y * y < SQUARE_SIZE * SQUARE_SIZE) {
      ++inside_circle;
    }
  }

  double get_pi() const {
    return 4 * static_cast<double>(inside_circle) / static_cast<double>(total);
  }

private:
  static constexpr size_t SQUARE_SIZE = 1000;
};

void bm_compute_pi(benchmark::State& state) {
  MonteCarlo mc;

  for (auto _ : state) {
    mc();
  }

  state.counters["pi"] = mc.get_pi() / state.threads();
}

} // namespace

BENCHMARK(bm_compute_pi)->Threads(2);

BENCHMARK_MAIN();
