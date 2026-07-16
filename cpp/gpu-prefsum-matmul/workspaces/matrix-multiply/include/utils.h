#ifndef UTILS
#define UTILS

#include "data-structures.h"
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

typedef struct {
  MNK mnk;
  MatrixType* a;
  MatrixType* b;
  MatrixType* cGraphic;
  MatrixType* cCentral;
} SourceMatrices;

typedef struct {
  const char* input;
  const char* output;
} Files;

extern Files files;
extern SourceMatrices sourceMatrices;

cl_int releaseContext(cl_int errorCode, const char* m1,const char* m2);

_Bool equals(const char* lhs, const char* rhs);

void logg(const char* message);

#endif
