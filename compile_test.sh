#!/bin/sh

mkdir bin
gcc -I"include" \
    -DUNITY_INCLUDE_DOUBLE -DUNITY_DOUBLE_PRECISION=1e-15 \
    -Wall -Wextra -Wpedantic -fsanitize=address \
    -o bin/test \
    src/test.c src/string_to_float.c src/unity.c \
    -lm

