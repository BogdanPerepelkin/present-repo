#!/bin/bash

cp -f include/* /Users/bogdan/ct-dance/gpu/workspaces/ct-gpu-26-prefsum-cuda-BogdanPerepelkin/include/
cp -f src/* /Users/bogdan/ct-dance/gpu/workspaces/ct-gpu-26-prefsum-cuda-BogdanPerepelkin/src/
cp -f main.cu /Users/bogdan/ct-dance/gpu/workspaces/ct-gpu-26-prefsum-cuda-BogdanPerepelkin/main.cu

cd ../ct-gpu-26-prefsum-cuda-BogdanPerepelkin
git add .
git commit -m "$1"
git push
