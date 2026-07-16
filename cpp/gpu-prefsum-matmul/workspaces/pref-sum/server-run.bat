@echo off

rem Fetch latest changes from git
git pull || goto :end

rem Compile CUDA project with nvcc.
rem Run this from a shell where nvcc is in PATH (Visual Studio Developer Command Prompt or CUDA shell).

@REM nvcc -std=c++17 -O3 -Iinclude main.cpp src\main-run.cpp src\errors.cpp src\logger.cpp src\pref-sum.cu -o my_tests.exe || goto :end

rem Build list of sources: main.cpp + all .cpp and .cu from src
set "SOURCES=main.cu"
for %%f in (src\*.cpp src\*.cu) do call set "SOURCES=%%SOURCES%% %%f"

nvcc -ccbin "C:\Program Files\Microsoft Visual Studio 2022\VC\Tools\MSVC\14.42.34433\bin\Hostx64\x64\cl.exe" -std=c++17 -O3 -Iinclude %SOURCES% -o my_tests.exe || goto :end

rem Run the built executable with CUDA parameters
my_tests.exe --input "running-files\g-matrices.bin" --output "running-files\out.bin" --device-type all --device-index 0 --realization 1 || goto :end

:end

