#include "utils.h"

#include "data-structures.h"

#include <stdio.h>
#include <string.h>

Files files = {0};
SourceMatrices sourceMatrices = {0};

cl_int releaseContext(cl_int errorCode, const char* m1, const char* m2) {
  char buf[LONG_BUFF];
  snprintf(buf, sizeof(buf), "[my_err] %s %s\n", m1, m2);
  if (errorCode != CL_SUCCESS) {
    logg(buf);
  }

  if (sourceMatrices.a != NULL) {
    free(sourceMatrices.a);
  }
  if (sourceMatrices.b != NULL) {
    free(sourceMatrices.b);
  }
  if (sourceMatrices.cCentral != NULL) {
    free(sourceMatrices.cCentral);
  }
  if (sourceMatrices.cGraphic != NULL) {
    free(sourceMatrices.cGraphic);
  }

  return errorCode;
}

_Bool equals(const char* lhs, const char* rhs) {
  return strcmp(lhs, rhs) == 0;
}

void logg(const char* message) {
  printf("%s", message);
}
