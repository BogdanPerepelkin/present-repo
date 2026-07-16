#include "data-structures.h"
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

cl_uint libUtProcGetPlatforms(UtLibContext* ctx);

cl_int libUtProcGetPlatformInfo(UtLibContext* ctx);

cl_int libUtProcGetDeviceIDs(UtLibContext* ctx);

cl_int libUtHelperAllocatingGet(UtLibContext* ctx, cl_device_id id, cl_device_info info, UtString* arr, const char* message);

cl_int libUtProcGetDeviceInfo(UtLibContext* ctx);

cl_int libUtReadPlatformDeviceInfo(UtLibContext* ctx);

cl_int libUtcreateClContext(UtLibContext* ctx);

cl_int libUtcreateProgram(UtLibContext* ctx);

void libUtmakeOptionsDefines(char* buf, size_t size);

void libUtmakeOptionsIncludes(char* buf, size_t size);

cl_int libUtProcBuild(UtLibContext* ctx);

cl_int libUtcreateQueue(UtLibContext* ctx);

cl_int libUtcreateBuffers(UtLibContext* ctx);

cl_int libUtenqueue2Dev(UtLibContext* ctx, cl_event* evFirstBuf, cl_event* evSecondBuf);

cl_int libUtcreateKernel(UtLibContext* ctx);

size_t libUtgetNaiveGlobalDim(size_t size);

cl_int libUtsetDimensions(UtLibContext* ctx);

cl_int libUtenqueueRunKernel(UtLibContext* ctx, cl_event* event);

cl_int libUtenqueue2Host(UtLibContext* ctx, cl_event* event);

ProfTimes libUtgetProfilingInfo(cl_event* event);

cl_int
libUtgetAdvancedProfilingInfo(UtLibContext* ctx, cl_event* event, cl_ulong amountOperations, cl_ulong* time, cl_float* flops);

cl_int libUtcreateAllDevs();

int libUtfillPriorityMap();

int libUtcompareAllDevices(const void* inLhs, const void* inRhs);

cl_int libUtReleaseContext(UtLibContext* ctx, cl_int errCode, const char* message);

void libUtErrLog(const char* message);

void libUtLogg(const char* message);
