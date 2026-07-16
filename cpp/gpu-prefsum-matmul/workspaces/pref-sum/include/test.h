#pragma once

#include "../include/pref-sum.h"

#include <cstddef>
#include <vector>

namespace test {

void checkCorrectness(const std::vector<uint>& src, prefsum::GpuResult gpu, prefsum::CpuResult cpu);

void correctness(size_t);

void noiseAndDraft();

void globalReadWrite();

void testAccessibleLocalSize();

void koggeStone();

void brentStone();

void reduceStone();

void bank();

} // namespace test

namespace profiling {

void koggeStone();

void brentStone();

void bank();

void optimize();

void alex();

} // namespace profiling
