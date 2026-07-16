#!/bin/bash

git add . ':!tables'
git commit -m "$1"
git push
