#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
int64_t bytesize(std::string input);
char* hextobytes(char* hex);
std::string pretty_bytes(int64_t bytes);
#endif
