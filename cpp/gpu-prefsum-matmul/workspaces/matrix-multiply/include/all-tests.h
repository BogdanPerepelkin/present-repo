#ifndef ALL_TESTS
#define ALL_TESTS

// initial test for correctness
void test_small();

void test_big();

void test_stress_v1();

void test_stress_v2();

void test_tiles();

void test_artificial_factor_iterations();

// ______ cpu

// transpose correctness and performance
void test_cpu_transpose_correctness_performance();

// ______ gpu

// gpu/cpu correctness
void test_gpu_cpu_correctness();

// v_2_short
void test_gpu_v1_vs_v2_short();

// optimal tile_size
void test_gpu_optimal_tile_size();

#endif
