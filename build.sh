#!/bin/sh
clang src/main.c $(pkg-config --libs --cflags raylib) -o game && ./game
