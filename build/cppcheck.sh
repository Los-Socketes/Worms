#!/bin/sh

cppcheck --inline-suppr --enable=all --suppress=missingIncludeSystem --suppress=missingInclude --suppress=unusedFunction --suppress=unmatchedSuppression --language=c++ --std=c++20 --error-exitcode=1  --suppress=noExplicitConstructor --suppressions-list=build/suppressions.txt src/*/*
