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
		if (clubCompress(argv[2]))
		{
			std::printf("Compressed %s -> %s.z successfully.", argv[2], argv[2]);
		}
		else
		{
			std::printf("Error while compressing file %s", argv[2]);
			return 1;
		}
		break;

	case DECOMPRESS:
		if (clubDecompress(argv[2]))
		{
			std::printf("Decompressed %s -> %s.z successfully.", argv[2], argv[2]);
		}
		else
		{
			std::printf("Error while decompressing file %s", argv[2]);
			return 1;
		}
		break;

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
