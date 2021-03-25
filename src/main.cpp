#include <iostream>
#include <cstdio>
#include "zlib/zlib.h"
#include "types.h"
#include "compression.h"

Operation_t getOperation(u16, char**);
void showUsage();

int main(u16 argc, char** argv)
{
	Operation_t task = getOperation(argc, argv);
	
	switch (task)
	{
	case INVALID:
		showUsage();
		return 1;

	case COMPRESS:
		if (clubCompress(argv[2]) == Z_OK)
		{
			std::printf("Compressed file %s -> %s.z successfully.\n", argv[2], argv[2]);
			break;
		}
		return 1;

	case DECOMPRESS:
		if (clubDecompress(argv[2]) == Z_OK)
		{
			std::printf("Decompressed file %s -> %s.z successfully.\n", argv[2], argv[2]);
			break;
		}
		return 1;

	default:
		break;
	}

	return 0;
}

Operation_t getOperation(u16 argc, char** argv)
{
	// Argument count
	if (argc != 3)
	{
		return INVALID;
	}

	// Argument strings
	if (!argv[1] || !argv[2])
	{
		return INVALID;
	}

	Operation_t task = INVALID;
	// Compress
	task = (_stricmp(argv[1], "-C") == 0) ? COMPRESS : task;
	// Decompress
	task = (_stricmp(argv[1], "-D") == 0) ? DECOMPRESS : task;

	// Always invalid if no filename specified
	return (argv[2] != NULL) ? task : INVALID;
}

void showUsage()
{
	std::cout << "Valid usage: " << std::endl
		<< "ClubCompress.exe -D MyCompressedFile.z" << std::endl
		<< "ClubCompress.exe -C MyUncompressedFile.z" << std::endl;
}