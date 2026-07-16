#include "utils.h"

#include <OpenCL/opencl.h>
#include <stdio.h>

extern bool enableCPUProfiling;

size_t idx(size_t i, size_t j, size_t n);

cl_int cpuCalc(
    size_t m,
    size_t n,
    size_t k,
    const MatrixType* a,
    const MatrixType* b,
    MatrixType* c,
    cl_ulong* time,
    double* flops
);

void printABC(
    const MatrixType* a,
    const MatrixType* b,
    const MatrixType* c,
    const MatrixType* cpuC,
    size_t aSize,
    size_t bSize,
    size_t cSize
);

void printPerformance(cl_ulong time, double flops, size_t amountOperations);