#!/bin/bash
set -e

clang \
  -std=c11 \
  -O2 \
  output.c \
  -o output

