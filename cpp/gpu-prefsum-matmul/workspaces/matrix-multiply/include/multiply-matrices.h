#ifndef MUlTIPLY_MATRICES
#define MUlTIPLY_MATRICES

#include "data-structures.h"

extern const char* LIB_CLCODE_PATHS[];
extern UtLibContext ctx;
extern GyperParametersLibrary libParam;
extern LibInitializeParameters libIni;

// CPU
cl_int libMultiplyMatricesCpu(MNK mnk, const MatrixType* restrict a, const MatrixType* restrict b, MatrixType* c);

void libGetCpuProfiling(ProfilingCPU* result);

// GPU
cl_int libInitializeContext();

cl_int libDestroyContext();

cl_int
libMultiplyMatricesGpu(MNK mnk, const MatrixType* restrict a, const MatrixType* restrict b, MatrixType* restrict c);

cl_uint libArtificialMultiplyMatricesGpu(
    MNK mnk,
    const float* restrict a,
    const float* restrict b,
    float* restrict c,
    cl_uint artificialFactor
);

const char* libGetChoosenDeviceName();

const char* libGetChoosenPlatformName();

cl_uint libGetMaxFrequency();

size_t libGetMaxWorkGroupSize();

cl_ulong libGetLocalMemSize();

void libGetGPUProfiling(ProfilingGPU* result);

cl_ulong libGetSubQue(ProfTimes profTimes);

cl_ulong libGetStaSub(ProfTimes profTimes);

cl_ulong libGetEndSta(ProfTimes profTimes);

cl_ulong libGetRealyFullTime(ProfTimes profTimes);

cl_float libGetFlops(ProfilingGPU profTimes);

void libPrintPlatsNum(UtLibContext* ctx);

void libPrintPlatsInfo(UtLibContext* ctx);

void libPrintDevIDs(UtLibContext* ctx);

void libPrintDevInfo(UtLibContext* ctx);

void libPrintChoosenDevice(UtLibContext* ctx);

#endif
