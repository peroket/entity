#!/bin/bash

# give analyzers, if empty it will be skipped
CPPCHECK=../build/cppcheck/cppcheck








# if we know the location of cppcheck
if [ -n $CPPCHECK ]
then
  echo 'cppchek:'
  ./$CPPCHECK -f --enable=all --platform=unix64 --library=qt --includes-file=includes.txt --config-excludes-file=excludes.txt ../sources/ ../unitTests/
fi



