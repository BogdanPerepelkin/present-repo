#!/bin/bash

cp -f include/* /Users/bogdan/ct-dance/gpu/workspaces/ct-gpu-26-matmul-opencl-BogdanPerepelkin/include/
cp -f src/* /Users/bogdan/ct-dance/gpu/workspaces/ct-gpu-26-matmul-opencl-BogdanPerepelkin/src
cp -f main.c /Users/bogdan/ct-dance/gpu/workspaces/ct-gpu-26-matmul-opencl-BogdanPerepelkin/main.c

cd ../ct-gpu-26-matmul-opencl-BogdanPerepelkin
git add .
git commit -m "$1"
git push
