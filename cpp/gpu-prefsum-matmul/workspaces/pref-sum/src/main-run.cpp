#include "../include/main-run.h"

#include "../include/errors.h"
#include "../include/logger.h"
#include "../include/pref-sum.h"
#include "../include/test.h"
#include "../include/utils.h"

#include <assert.h>
#include <omp.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <cstdint>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

#define RELEASE 0

namespace parsing {

struct Files {
  std::string input;
  std::string output;
};

Files files;

} // namespace parsing

void myBusinesLogic() {
  printf("=========&& hello from business logic &&=========\n");

  try {
    // test::draft(1000);
    // test::NoiseAndDraft();
    // test::AccessibleLocalSize();
    // test::GlobalReadWrite();

    // test::koggeStone();
    // test::brentStone();
    // test::reduceStone();
    test::bank();

    // profiling::koggeStone();
    // profiling::brentStone();
    // profiling::optimize();
    // profiling::alex();
    // profiling::bank();

  } catch (...) {
    logg::log(logg::LogLevel::ERROR, "catch error on top (myBusinesLogic), its bad...\n");
  }
}

bool equals(const char* fst, const char* sec) {
  return std::string(fst) == std::string(sec);
}

void checkGenericInfo(prefsum::PrefSum& prefsum, int argc, char* argv[], size_t i, size_t* futureI) {
  if (std::string(argv[i]) == std::string("--help")) {
    logg::log(
        "lab0.exe < --input file_name > \n < --output file_name > \n [ --device-type { dgpu | igpu | gpu | cpu | all } "
        "] \n [ --device-index index ]"
    );
    return;
  }

  bool isNotFoundAny = false;

  if (std::string(argv[i]) == std::string("--input")) {
    if (i + 1 < argc) {
      parsing::files.input = argv[i + 1];
    } else {
      throw errors::ValidationArgsError("expected arg after key input");
    }
  } else if (std::string(argv[i]) == std::string("--output")) {
    if (i + 1 < argc) {
      parsing::files.output = argv[i + 1];
    } else {
      throw errors::ValidationArgsError("expected value after --output");
    }
  } else if (equals(argv[i], "--device-index")) {
    size_t deviceIndex = 0;
    if (i + 1 < argc) {
      deviceIndex = atoi(argv[i + 1]);
    } else {
      throw errors::ValidationArgsError("expected value after --device-index key");
    }
    prefsum.ini.deviceIndex = deviceIndex;
  } else {
    isNotFoundAny = true;
  }

  if (!isNotFoundAny) {
    *futureI = i + 1;
  }
}

void checkSpecializeInfo(prefsum::PrefSum& prefsum, int argc, char* argv[], size_t i, size_t* futureI) {
  bool isNotFoundAny = false;

  if (equals(argv[i], "--verify")) {
    prefsum.gyperLibraryParameters.enableCpuProfiling = true;
  } else if (equals(argv[i], "--realization")) {
    if (i + 1 < argc) {
      prefsum::GPUCalculatingMode mode = prefsum::V_BrentStone;
      // todo увеличить когда разрастется
      if (mode > 2 || mode < 0) {
        throw errors::ValidationArgsError("gpu calculating mode not in [0,2]");
      }
      prefsum.gyperLibraryParameters.calculatingMode = mode;
    } else {
      throw errors::ValidationArgsError("expected value after --device-index key");
    }
  } else {
    isNotFoundAny = true;
  }

  if (!isNotFoundAny) {
    *futureI = i + 1;
  }
}

void processInitializing(prefsum::PrefSum& prefsum, int argc, char* argv[]) {
  for (size_t i = 0; i < argc; i++) {
    checkSpecializeInfo(prefsum, argc, argv, i, &i);
    checkGenericInfo(prefsum, argc, argv, i, &i);
  }
}

void readInputFile(std::vector<uint32_t>& out) {
  std::ifstream in(parsing::files.input, std::ios::binary);
  if (!in.is_open()) {
    throw errors::ValidationArgsError(std::string("cannot open input file: ") + parsing::files.input);
  }

  uint32_t n = 0;
  in.read(reinterpret_cast<char*>(&n), sizeof(n));
  if (!in) {
    throw errors::ValidationArgsError(std::string("failed read n from: ") + parsing::files.input);
  }

  out.clear();
  out.resize(n);

  if (n > 0) {
    in.read(reinterpret_cast<char*>(out.data()), static_cast<std::streamsize>(n * sizeof(uint32_t)));
    if (!in) {
      throw errors::ValidationArgsError(std::string("failed read data from: ") + parsing::files.input);
    }
  }
}

void writeOutputFile(const std::vector<uint32_t>& inVec) {
  std::vector<uint32_t> outVec;
  outVec.reserve(inVec.size());
  uint32_t acc = 0;
  for (uint32_t v : inVec) {
    acc += v;
    outVec.push_back(acc);
  }

  std::ofstream out(parsing::files.output, std::ios::binary);
  if (!out.is_open()) {
    throw errors::ValidationArgsError(std::string("cannot open output file: ") + parsing::files.output);
  }

  uint32_t nn = static_cast<uint32_t>(outVec.size());
  out.write(reinterpret_cast<const char*>(&nn), sizeof(nn));
  if (!out) {
    throw errors::ValidationArgsError(std::string("failed write n to: ") + parsing::files.output);
  }
  if (!outVec.empty()) {
    out.write(
        reinterpret_cast<const char*>(outVec.data()),
        static_cast<std::streamsize>(outVec.size() * sizeof(uint32_t))
    );
    if (!out) {
      throw errors::ValidationArgsError(std::string("failed write data to: ") + parsing::files.output);
    }
  }
}

void printInfo(
    const prefsum::PrefSum& evaluator,
    const prefsum::GpuResult& gpuResult,
    std::optional<prefsum::CpuResult>& cpuResult
) {
  logg::log("gpu time:\t%f\n", utils::ns2ms(gpuResult._time.rawCalc));
  logg::log("parameters:\n");
  logg::log("|\tblock size = %zu\n", evaluator.gyperLibraryParameters.block.x);
  logg::log("|\tpartition = %zu\n", evaluator.gyperLibraryParameters.partition);
  if (cpuResult) {
    logg::log("cpu time:\t%f\n", utils::ns2ms(cpuResult->_time));
  }
}

int mainRun(int argc, char* argv[]) {
#if RELEASE == 1
  if (argc < 2) {
    logg::log("use --help to start");
    return 1;
  }

  try {
    prefsum::PrefSum evaluator;
    prefsum::CalculateParameters parampapams;

    processInitializing(evaluator, argc, argv);

    static std::vector<uint32_t> inputFileData;
    static std::vector<uint32_t> outputFileData;
    readInputFile(inputFileData);

    try {
      parampapams.elements.assign(inputFileData.size(), 0);
    } catch (...) {
      throw errors::MemoryError("vector");
    }

    evaluator.gyperLibraryParameters.block.x = 1024;
    evaluator.gyperLibraryParameters.partition = 32;

    prefsum::GpuResult gpuResult({}, {}, {});
    gpuResult = evaluator.gpuCalculate(parampapams);

    std::optional<prefsum::CpuResult> cpuResult = std::nullopt;
    if (evaluator.gyperLibraryParameters.enableCpuProfiling) {
      cpuResult = evaluator.cpuCalculate(parampapams);

      test::checkCorrectness(parampapams.elements, gpuResult, *cpuResult);
    }

    writeOutputFile(inputFileData);

    printInfo(evaluator, gpuResult, cpuResult);
  } catch (const std::exception& e) {
    logg::log(logg::LogLevel::ERROR, "%s\n", e.what());
    return 1;
  } catch (...) {
    throw;
  }

#else
  myBusinesLogic();
#endif

  return 0;
}
