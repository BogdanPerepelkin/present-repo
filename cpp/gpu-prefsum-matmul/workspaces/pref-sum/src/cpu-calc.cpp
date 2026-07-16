#include "../include/pref-sum.h"

#include <chrono>

#include <vector>

ull helperGetTime() {
  using namespace std::chrono;
  auto now = steady_clock::now();
  return static_cast<ull>(duration_cast<nanoseconds>(now.time_since_epoch()).count());
}

prefsum::CpuResult prefsum::PrefSum::cpuCalculate(const CalculateParameters& parameters) {
  const std::vector<uint>& src = parameters.elements;
  std::vector<uint> result;
  if (src.size() == 0) {
    return {result, 0};
  }
  result.assign(src.size(), 0);

  ull start = helperGetTime();
  result[0] = src[0];
  ull prev = result[0];
  for (size_t i = 1; i < src.size(); i++) {
    prev = prev + src[i];
    result[i] = prev;
  }
  ull end = helperGetTime();

  return {result, end - start};
}
