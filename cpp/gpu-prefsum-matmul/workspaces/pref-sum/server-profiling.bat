@echo off

git pull || goto :end

set "SOURCES=main.cu"
for %%f in (src\*.cpp src\*.cu) do call set "SOURCES=%%SOURCES%% %%f"

nvcc -ccbin "C:\Program Files\Microsoft Visual Studio 2022\VC\Tools\MSVC\14.42.34433\bin\Hostx64\x64\cl.exe" -std=c++17 -O3 -lineinfo -arch=sm_80 -Xptxas="-v" -Iinclude %SOURCES% -o profiling_program.exe || goto :end


:end
