@echo off
git pull || goto :end
@REM cmake --build --preset "release-server"
clang -fopenmp -std=c2x -DUSING_OCL -D_CRT_SECURE_NO_WARNINGS -D_USE_MATH_DEFINES -O3 -Wall -Iinclude src\*.c main.c -o __test.exe -lopencl ^
    -Dcsvprint_OUTPUT_PATH="\"C:/Users/st26_user_4/gpu-skkv/workspaces/matrix-multiply/tables/\"" || goto :end
@REM build\release-clang\my_tests.exe --input "running-files\1337_1337_1337_in.bin" --output "running-files\out.bin" --device-type dgpu --device-index 0
__test.exe --input "running-files\g-matrices.bin" --output "running-files\out.bin" --device-type all --device-index 0 --realization 1 || goto :end

:end


