#!/bin/bash

git add . ':!tables' ':!cuda-stubs'
git commit -m "$1"
git push
