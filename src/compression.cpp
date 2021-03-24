#include "compression.h"
#include "types.h"
#include <zlib/zlib.h>
#include <fstream>
#include <string>

bool clubCompress(char* pName)
{
	// Open filestream (start at end of file)
	std::ifstream ifs(pName, std::ios::binary | std::ios::ate);

	// Make sure stream actually is open
	if (!ifs.is_open())
	{
		ifs.close();
		return false;
	}

	// Get + validate filesize
	u32 fileSize = (u32)ifs.tellg();
	if (fileSize < 1)
	{
		return false;
	}

	// File buffer
	char* pFile = new char[fileSize];
	// Read data into buffer
	ifs.seekg(0, std::ios::beg);
	ifs.read(pFile, fileSize);
	// Close stream
	ifs.close();

	// Deflated data buffer
	char* pNewFile = new char[fileSize];
	// Setup z_stream
	z_stream zs;
	zs.next_in = (Bytef*)pFile;
	zs.avail_in = fileSize;
	zs.next_out = (Bytef*)pNewFile;
	zs.avail_out = fileSize;

	// Compress (deflate)
	deflateInit(&zs, Z_BEST_COMPRESSION);
	deflate(&zs, Z_FINISH);
	deflateEnd(&zs);
	delete[] pFile;

	// New file path (previous name + ".z" extension)
	std::string newPath = pName;
	newPath += ".z";

	// Open output stream
	std::ofstream ofs(newPath, std::ios::binary);
	// Validate stream
	if (!ofs.is_open())
	{
		ofs.close();
		return false;
	}
	// Write new file (size = total bytes written by z_stream)
	ofs.write(pNewFile, zs.total_out);
	delete[] pNewFile;
	// Close stream
	ofs.close();

	return true;
}

bool clubDecompress(char* pName)
{
	return false;
}