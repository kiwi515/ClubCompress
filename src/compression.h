#pragma once
#include "types.h"
#include <cstdio>

#define WSC_WINDOWSIZE 12

// Files are compressed/decompressed in chunks of 512K
const u32 g_ChunkSize = 512000;

bool clubCompress(char* pName);
bool clubDecompress(char* pName);