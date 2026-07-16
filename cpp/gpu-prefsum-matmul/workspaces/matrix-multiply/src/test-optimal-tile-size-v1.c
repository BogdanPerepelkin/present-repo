#include "all-tests.h"
#include "csv-printer.h"
#include "data-structures.h"
#include "multiply-matrices.h"
#include "test-utils.h"

#include <stdio.h>

void test_gpu_optimal_tile_size() {
  const char* DIRECTORY_NAME = "tile_size/";
  const size_t NORMA = 32;
  RunInfo runInfo = {true, 1};
  for (size_t curSize = 2000; curSize < 6000 + 1; curSize += 1000) {
    char fileName[BUFF];
    snprintf(fileName, sizeof(fileName), "%s%zu", DIRECTORY_NAME, curSize);
    csvSetPath(fileName);
    csvClearFile();

    for (size_t tileSize = 6; tileSize < 16 + 1; tileSize++) {
      float time = 0;
      for (size_t iterator = 0; iterator < 32; iterator++) {
        TargetResult result = utRunTest(curSize, curSize, curSize, fileName, runInfo);
        time += ns2ms(libGetEndSta(result.gpu.calculating)) / NORMA;
      }

      csvAppendFloat(tileSize);
      csvAppendFloat(time);
      csvNewLine();
    }
  }
}
