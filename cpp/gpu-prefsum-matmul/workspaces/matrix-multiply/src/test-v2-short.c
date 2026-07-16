#include "all-tests.h"
#include "csv-printer.h"
#include "data-structures.h"
#include "multiply-matrices.h"
#include "test-utils.h"

void test_gpu_v1_vs_v2_short() {
  const char* DIRECTORY_NAME = "v1-vs-v2-short/";
  RunInfo runInfo = {true, 1};
  libParam.ENABLE_CPU_PROFILING = false;

  for (size_t curSize = 2000; curSize < 10000 + 1; curSize += 1000) {
    char name[BUFF];
    snprintf(name, sizeof(name), "%s%zu", DIRECTORY_NAME, curSize);
    csvSetPath(name);
    csvClearFile();
    csvAppendLine("size,time_v1,time_v2");

    size_t size = curSize;

    libParam.SHORT_TILE_SIZE = 16;
    libParam.CALCULATING_MODE = V_1;
    TargetResult res1 = utRunTest(size, size, size, "v1", runInfo);

    for (size_t tileSize = 4; tileSize < 64; tileSize++) {
      libParam.SHORT_TILE_SIZE = libParam.SHORT_THREAD_RANGE = tileSize;

      char tilesName[BUFF];
      snprintf(tilesName, sizeof(tilesName), "tiles %zu", tileSize);
      char threadsName[BUFF];
      snprintf(threadsName, sizeof(threadsName), "threads %zu", tileSize);

      libParam.CALCULATING_MODE = V_2_SHORT;
      TargetResult res2 = utRunTest(size, size, size, threadsName, runInfo);

      csvAppendFloat(tileSize);
      csvAppendFloat(ns2ms(libGetEndSta(res1.gpu.calculating)));
      csvAppendFloat(ns2ms(libGetEndSta(res2.gpu.calculating)));
      csvNewLine();
    }
  }
}
