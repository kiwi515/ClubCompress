#pragma once
#include "types.h"
#include <cstdio>

#define WSC_WINDOWSIZE 12
#define WSC_MEMLEVEL 5

//const char* zError[] = {
//	"Z_OK", "Z_STREAM_END", "Z_NEED_DICT",
//	"Z_ERRNO", "Z_STREAM_ERROR", "Z_DATA_ERROR",
//	"Z_MEM_ERROR", "Z_BUF_ERROR", "Z_VERSION_ERROR"
//};

//const char* zGetError(int err);
int clubCompress(char* pName);
int clubDecompress(char* pName);