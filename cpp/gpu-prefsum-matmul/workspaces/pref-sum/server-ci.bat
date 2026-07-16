
@call "C:\Program Files\Microsoft Visual Studio 2022\VC\Auxiliary\Build\vcvars64.bat" >nul
@"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v12.8\bin\nvcc.exe" -o __test.exe -O3 --std c++20 -gencode arch=compute_89,code=sm_89 -no-compress %*
