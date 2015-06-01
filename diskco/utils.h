#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <stdint.h>
int64_t bytesize(std::string input);
char* hextobytes(const char* hex);
std::string pretty_bytes(int64_t bytes);
#endif
