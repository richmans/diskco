#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <stdio.h>

int64_t bytesize(std::string input);
std::string pretty_bytes(int64_t bytes);
#endif