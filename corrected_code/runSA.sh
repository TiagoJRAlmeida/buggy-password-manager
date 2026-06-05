#!/bin/bash

rm -rf static_analysis/*
scan-build --show-description \
  -enable-checker security \
  -enable-checker security.insecureAPI.gets \
  -enable-checker security.insecureAPI.strcpy \
  -enable-checker security.insecureAPI.rand \
  -o static_analysis \
  make clean all \
    CC=clang \
    "CFLAGS=-Wall -std=c11 -D_XOPEN_SOURCE=500 -D_GNU_SOURCE -g" \
    "LDFLAGS=-lbsd"
