#pragma once
#include "types.h"
#include <cstdio>

#define WSC_WINDOWSIZE 12
#define WSC_MEMLEVEL 5

#define KB 1024
#define MB KB * 1024
#define GB MB * 1024

bool clubCompress(char* pName);
bool clubDecompress(char* pName);