cmake_minimum_required(VERSION 3.20)
project(proj LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Добавляем все исходники в исполняемый файл

add_library(lib STATIC src/gpu-calc.c src/cpu-calcs.c)
target_include_directories(lib PUBLIC include)
find_package(OpenCL REQUIRED)
target_link_libraries(lib PUBLIC OpenCL::OpenCL)

# Тесты
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS main.c src/*.c )
add_executable(my_tests ${TEST_SOURCES})
target_include_directories(my_tests PRIVATE include )
target_link_libraries(my_tests PRIVATE lib)


enable_testing()
add_test(NAME my_tests COMMAND my_tests)

# OpenMP (если доступно, но не обязательно)
if(APPLE)
  set(OpenMP_C_FLAGS "-fopenmp")
  set(OpenMP_C_LIB_NAMES "omp" "libomp")
  set(OpenMP_CXX_FLAGS "-fopenmp")
  set(OpenMP_CXX_LIB_NAMES "omp" "libomp")
  set(OpenMP_C_LIB_PATH "/opt/homebrew/opt/libomp/lib")
  set(OpenMP_CXX_LIB_PATH "/opt/homebrew/opt/libomp/lib")
  set(OpenMP_omp_LIBRARY "/opt/homebrew/opt/libomp/lib/libomp.dylib")
  set(OpenMP_libomp_LIBRARY "/opt/homebrew/opt/libomp/lib/libomp.dylib")
elseif(WIN32 AND CMAKE_C_COMPILER_ID MATCHES "Clang")
  set(OpenMP_C_FLAGS "-fopenmp")
  set(OpenMP_C_LIB_NAMES "omp" "libomp")
  set(OpenMP_C_LIB_PATH "C:/Program Files/LLVM/lib")
  set(OpenMP_CXX_FLAGS "-fopenmp")
  set(OpenMP_CXX_LIB_NAMES "omp" "libomp")
  set(OpenMP_CXX_LIB_PATH "C:/Program Files/LLVM/lib")
  set(OpenMP_omp_LIBRARY "C:/Program Files/LLVM/lib/libomp.lib")
  set(OpenMP_libomp_LIBRARY "C:/Program Files/LLVM/lib/libomp.lib")
endif()
find_package(OpenMP)
if(OpenMP_C_FOUND)
  target_link_libraries(lib PUBLIC OpenMP::OpenMP_C)
else()
  message(STATUS "OpenMP not found; building without OpenMP support")
endif()