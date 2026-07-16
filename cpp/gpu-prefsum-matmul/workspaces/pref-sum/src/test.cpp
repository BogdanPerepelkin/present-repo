#include "../include/test.h"

#include "../include/descriptor-types.h"
#include "../include/errors.h"
#include "../include/logger.h"
#include "../include/pref-sum.h"
#include "../include/utils.h"
// #include "../alex/scan_cuda.h"

#include <sys/types.h>

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

// 300000000 elements = 1.1175870895385742 GB

namespace DataSize {
const size_t GYGABYTE = 300000000;
const size_t M_03 = 100000000;
const size_t M_003 = 10000000;
const size_t SMALL = 1000;
} // namespace DataSize

namespace test {

template <typename T>
void printVector(const std::vector<T>& vector, size_t from, size_t to, const std::string& name) {
  logg::log("%s\n", name.data());
  for (size_t i = from; i < to; i++) {
    std::cout << vector[i] << " ";
  }
  logg::log("\n");
}

void printDescriptors(const std::vector<Descriptor>& vector, size_t from, size_t to, const std::string& name) {
  logg::log("%s\n", name.data());
  for (size_t i = from; i < to; i++) {
    std::cout << vector[i].condition << " ";
  }
  logg::log("\n");
}

void checkCorrectness(const std::vector<uint>& src, prefsum::GpuResult gpu, prefsum::CpuResult cpu) {
  auto& gpuVector = gpu._elements;
  auto& cpuVector = cpu._elements;

  if (gpuVector.size() != cpuVector.size()) {
    logg::log("different sizes: cpu = %zu, gpu = %zu\n", cpuVector.size(), gpuVector.size());
    throw errors::UncorrectError("");
  }

  for (size_t i = 0; i < gpuVector.size(); i++) {
    if (gpuVector[i] != cpuVector[i]) {
      size_t endIdx = 19;
      printVector(src, 0, endIdx, "source");
      printVector(cpuVector, 0, endIdx, "cpu");
      printVector(gpuVector, 0, endIdx, "gpu");
      logg::log("different values on %zu: cpu = %u, gpu = %u\n", i, cpuVector[i], gpuVector[i]);
      throw errors::UncorrectError("");
    }
  }
  logg::log("good\n");
}

void printPredInfo(const prefsum::CalculateParameters& parameters, const prefsum::PrefSum& evaluator) {
  logg::log("[PRED INFO]\n");
  logg::log("|\tn = %zu\n", parameters.elements.size());
  logg::log("|\tblock size = %zu\n", evaluator.gyperLibraryParameters.block.x);
  if (evaluator.gyperLibraryParameters.calculatingMode >= prefsum::V_BrentStone) {
    logg::log("|\tpartition = %zu\n", evaluator.gyperLibraryParameters.partition);
  }
}

void printPostInfo(const prefsum::GpuResult& gpuRes, const prefsum::CpuResult& cpuRes) {
  logg::log("[POST INFO]\n");
  logg::log("gpu time:\n");
  logg::log("|\traw calc = %f\n", utils::ns2ms(gpuRes._time.rawCalc));
  logg::log("|\thost 2 dev = %f\n", utils::ns2ms(gpuRes._time.host2Dev));
  logg::log("|\tdev 2 host = %f\n", utils::ns2ms(gpuRes._time.dev2Host));

  logg::log("cpu time:\n");
  logg::log("|\traw calc = %f\n", utils::ns2ms(cpuRes._time));
}

struct TestArgs {
  prefsum::GPUCalculatingMode mode;
  size_t sourceSize;
  size_t blockSize;
  size_t partition;
};

struct Statistics {
  using Result = std::pair<TestArgs, prefsum::GpuResult>;
  std::vector<Result> all;
  std::vector<TestArgs> fuckUp;
};

void correctness( TestArgs args, std::optional<Statistics>& statistics, bool release = false) {
  logg::log("==correctness test==\n");
  prefsum::PrefSum evaluator;

  prefsum::CalculateParameters parampapams;

  try {
    parampapams.elements.assign(args.sourceSize, 0);
  } catch (...) {
    throw errors::MemoryError("vector");
  }

  for (auto& i : parampapams.elements) {
    i = utils::getRand();
  }

  evaluator.gyperLibraryParameters.calculatingMode = args.mode;

  evaluator.gyperLibraryParameters.block.x = args.blockSize;
  evaluator.gyperLibraryParameters.partition = args.partition;

  printPredInfo(parampapams, evaluator);

  prefsum::GpuResult gpuResult({}, {}, {});
  try {
    gpuResult = evaluator.gpuCalculate(parampapams);
  } catch (const errors::ValidationArgsError& e) {
    logg::log(logg::LogLevel::ERROR, "[my err] parsing \n");
    logg::log(logg::LogLevel::ERROR, "%s\n", e.what());
    return;
  } catch (const std::exception& e) {
    logg::log(logg::LogLevel::ERROR, "[my err] gpuCalculate \n");
    logg::log(logg::LogLevel::ERROR, "%s\n", e.what());
    logg::log("\n==##############==\n");
    return;
  } catch (...) {
    throw;
  }
  auto cpuResult = evaluator.cpuCalculate(parampapams);

  try {
    checkCorrectness(parampapams.elements, gpuResult, cpuResult);
  } catch (const errors::UncorrectError& e) {
    logg::log("%s\n", e.what());
    logg::log("\n==##############==\n");
    if (statistics) {
      statistics->fuckUp.push_back(args);
    }
    return;
  } catch (...) {
    throw;
  }

  printPostInfo(gpuResult, cpuResult);

  if (statistics) {
    gpuResult._elements.clear();
    statistics->all.push_back({args, gpuResult});
  }

  logg::log("\n==_______________==\n");
}

std::optional<Statistics> nulloptStatistics = std::nullopt;

void noiseAndDraft() {
  std::vector<size_t> sizes = {1000, 1000, 10000, 100000, 1000000, 10000000, 100000000};
  // std::vector<size_t> sizes = {100000000};
  // std::vector<size_t> sizes;
  // for (size_t i = 0; i < 1000; i++) {
  // sizes.push_back(1000);
  // }
  for (auto& el : sizes) {
    correctness({prefsum::V_KoggeStone, el, 128, 0}, nulloptStatistics);
  }
}

void testAccessibleLocalSize() {
  std::vector<size_t> sizes;
  for (size_t i = 1; i < 20; i++) {
    sizes.push_back(64 * i);
  }

  for (auto& el : sizes) {
    correctness({prefsum::V_KoggeStone, 10000000, el, 0}, nulloptStatistics);
  }
}

void globalReadWrite() {
  logg::log("==stupid global copy-write time test==\n");

  prefsum::PrefSum evaluator;
  prefsum::CalculateParameters parampapams;

  size_t n = DataSize::GYGABYTE;

  try {
    parampapams.elements.assign(n, 0);
  } catch (...) {
    throw errors::MemoryError("vector");
  }

  for (auto& i : parampapams.elements) {
    i = utils::getRand();
  }

  evaluator.gyperLibraryParameters.calculatingMode = prefsum::V_GlobalTransfer;

  evaluator.gyperLibraryParameters.block.x = 512;
  evaluator.gyperLibraryParameters.partition = 0;

  prefsum::GpuResult gpuResult({}, {}, {});
  try {
    gpuResult = evaluator.gpuCalculate(parampapams);
  } catch (const std::exception& e) {
    logg::log(logg::LogLevel::ERROR, "[my err] gpuCalculate \n");
    logg::log(logg::LogLevel::ERROR, "%s\n", e.what());
  } catch (...) {
    throw;
  }

  logg::log("n = %zu\ntime = %f\n", n, utils::ns2ms(gpuResult._time.rawCalc));

  logg::log("\n==_______________==\n");
}

void printStatictics(Statistics& statistics, prefsum::GPUCalculatingMode mode) {
  std::sort(
      statistics.all.begin(),
      statistics.all.end(),
      [](const Statistics::Result& lhs, const Statistics::Result& rhs) {
        return lhs.second._time.rawCalc < rhs.second._time.rawCalc;
      }
  );

  logg::log("$$$test result$$$\n");
  logg::log("amount good = %zu\n", statistics.all.size());

  for (auto& el : statistics.all) {
    logg::log("TIME = %f, block size = %zu", utils::ns2ms(el.second._time.rawCalc), el.first.blockSize);
    if (mode >= prefsum::GPUCalculatingMode::V_BrentStone) {
      logg::log(", partition = %zu, low range = %zu", el.first.partition, el.first.blockSize / el.first.partition);
    }
    logg::log("\n");
  }
  logg::log("fucked up (%zu):\n", statistics.fuckUp.size());
  for (auto& el : statistics.fuckUp) {
    logg::log("block size = %zu", el.blockSize);
    if (mode >= prefsum::GPUCalculatingMode::V_BrentStone) {
      logg::log(", partition = %zu, low range = %zu", el.partition, el.blockSize / el.partition);
    }
    logg::log("\n");
  }
}

void koggeStone() {
  std::optional<Statistics> statistics;
  statistics.emplace();
  std::vector<size_t> blockSizes = {512, 1024};
  for (auto& i : blockSizes) {
    correctness({prefsum::V_KoggeStone, DataSize::GYGABYTE, i, 0}, statistics);
  }
  printStatictics(*statistics, prefsum::V_KoggeStone);
}

void brentModify(std::vector<prefsum::GPUCalculatingMode> modes, bool release = false) {
  for (auto& mode : modes) {
    std::unordered_map<size_t, std::vector<size_t>> blockSize2Partition = {
        {32, {1, 2, 4, 8}},
        {64, {4, 8, 16}},
        {128, {4, 8, 16}},
        {256, {8, 16, 32}},
        {512, {16, 32, 64}},
        {1024, {32, 64, 128}}
    };

    std::optional<Statistics> statistics;
    statistics.emplace();

    // std::unordered_map<size_t, std::vector<size_t>> blockSize2Partition = {{512, {16}}, {1024, {32}}};

    for (auto& testPair : blockSize2Partition) {
      size_t blockSize = testPair.first;
      for (size_t partition : testPair.second) {
        correctness({mode, DataSize::GYGABYTE, blockSize, partition}, statistics);
      }
    }

    printStatictics(*statistics, mode);
  }
}

void brentStone() {
  brentModify({prefsum::V_BrentStone});
}

void reduceStone() {
  brentModify({prefsum::V_ReduceStone});
}

void bank() {
  brentModify({prefsum::V_Bank});
}

} // namespace test

namespace profiling {

void koggeStone() {
  test::correctness({prefsum::V_KoggeStone, DataSize::M_03, 32, 0}, test::nulloptStatistics);
}

void brentStone() {
  test::correctness({prefsum::V_BrentStone, DataSize::M_03, 1024, 32}, test::nulloptStatistics);
}

void bank() {
  test::correctness({prefsum::V_Bank, DataSize::GYGABYTE, 1024, 32}, test::nulloptStatistics);
}

void optimize() {
  test::correctness({prefsum::V_Optimize, DataSize::GYGABYTE, 1024, 32}, test::nulloptStatistics);
}

// void alex() {
//   using namespace std;

//   // size for quick profiling; change if needed on server
//   uint32_t n = static_cast<uint32_t>(DataSize::GYGABYTE);

//   vector<uint32_t> input(n);
//   vector<uint32_t> output(n);

//   for (uint32_t i = 0; i < n; ++i) {
//     input[i] = utils::getRand();
//   }

//   CudaScanConfig cfg;
//   cfg.blockThreads = 256;
//   cfg.itemsPerThread = 8;
//   cfg.warmup = 1;
//   cfg.repeat = 3;

//   try {
//     CudaScanResult res = scanCuda(input.data(), output.data(), n, cfg);
//     logg::log("alex: GPU time = %f ms\n", res.milliseconds);
//     for (const auto &k : res.kernels) {
//       logg::log("%s time = %f ms\n", k.name.c_str(), k.milliseconds);
//     }
//   } catch (const std::exception &e) {
//     logg::log(logg::LogLevel::ERROR, "alex: scanCuda failed: %s\n", e.what());
//   }

// }


} // namespace profiling
