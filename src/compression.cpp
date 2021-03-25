#include "compression.h"
#include "types.h"
#include <zlib/zlib.h>
#include <fstream>
#include <string>
#include <iostream>

bool clubCompress(char* pName)
{
	if (!pName)
	{
		std::printf("[ERROR] Filename is null.");
		return false;
	}

	// Open filestream (start at end of file)
	std::ifstream ifs(pName, std::ios::binary | std::ios::ate);

	// Make sure stream actually is open
	if (!ifs.is_open())
	{
		ifs.close();
		std::printf("[ERROR] File stream to %s (original file) could not be opened.\n\
			Are you sure the file exists and is not protected?", pName);
		return false;
	}

	// Get + validate filesize
	u32 fileSize = (u32)ifs.tellg();
	if (fileSize < 1)
	{
		std::printf("[ERROR] Filesize error");
		return false;
	}

	// File buffer
	char* pFile = new char[fileSize];
	// Read data into buffer
	ifs.seekg(0, std::ios::beg);
	ifs.read(pFile, fileSize);
	// Close stream
	ifs.close();

	// Create stream
	z_streamp pStrm = new z_stream();
	// Deflated data buffer
	u32 deflateEstimate = deflateBound(pStrm, fileSize);
	char* pNewFile = new char[deflateEstimate];
	// Setup z_stream
	pStrm->next_in = (Bytef*)pFile;
	pStrm->avail_in = fileSize;
	pStrm->next_out = (Bytef*)pNewFile;
	pStrm->avail_out = fileSize;

	// Compress (deflate)
	deflateInit2(pStrm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, WSC_WINDOWSIZE, WSC_MEMLEVEL, Z_DEFAULT_STRATEGY);
	if (deflate(pStrm, Z_FINISH) != Z_STREAM_END)
	{
		std::printf("[ERROR] Deflate did not reach the end of the stream.");
		return false;
	}
	deflateEnd(pStrm);
	// Original file buffer no longer needed
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
		std::printf("[ERROR] File stream to %s (new file) could not be opened.\n\
			Are you sure the file exists and is not protected?", newPath.c_str());
		return false;
	}
	// Write new file (size = total bytes written by z_stream)
	ofs.write(pNewFile, pStrm->total_out);

	// Write uncompressed filesize (WSC expects this at EOF)
	// Data should be in little-endian (Wii U)
	ofs.write(reinterpret_cast<char *>(&fileSize), sizeof(fileSize));

	// Close stream
	ofs.close();
	// Free memory
	delete[] pNewFile;

	return true;
}

bool clubDecompress(char* pName)
{
	if (!pName)
	{
		std::printf("[ERROR] Filename is null.");
		return false;
	}

	// Open filestream (start at end of file)
	std::ifstream ifs(pName, std::ios::binary | std::ios::ate);

	// Make sure stream actually is open
	if (!ifs.is_open())
	{
		ifs.close();
		std::printf("[ERROR] File stream to %s (original file) could not be opened.\n\
			Are you sure the file exists and is not protected?", pName);
		return false;
	}

	// Get + validate filesize
	u32 fileSize = (u32)ifs.tellg();
	if (fileSize < 1)
	{
		std::printf("[ERROR] Filesize error");
		return false;
	}

	// Get uncompressed filesize (last 4 bytes of file)
	ifs.seekg(-4, std::ios::cur);
	u32 inflateSize = 0;
	ifs.read(reinterpret_cast<char*>(&inflateSize), sizeof(inflateSize));
	// We can really only guess if the uncompressed size is missing
	// It should never be more than 200 MB
	if (inflateSize > MB * 200)
	{
		std::printf("[Warn] Uncompressed filesize is missing from the end of the file.\n\
			Are you sure you want to decompress anyway? (Y/N) ");

		char c;
		std::cin >> c;
		if (std::tolower(c) == 'n')
		{
			return false;
		}
	}

	// File buffer
	char* pFile = new char[fileSize];
	// Read data into buffer
	ifs.seekg(0, std::ios::beg);
	ifs.read(pFile, fileSize);
	// Close stream
	ifs.close();

	// Create stream
	z_streamp pStrm = new z_stream();
	// Inflated file buffer
	char* pNewFile = new char[inflateSize];
	// Setup z_stream
	pStrm->next_in = (Bytef*)pFile;
	pStrm->avail_in = fileSize;
	pStrm->next_out = (Bytef*)pNewFile;
	pStrm->avail_out = inflateSize;

	// Decompress (inflate)
	inflateInit2(pStrm, WSC_WINDOWSIZE);
	if (inflate(pStrm, Z_FINISH) != Z_STREAM_END)
	{
		std::printf("[ERROR] Inflate did not reach the end of the stream.");
		return false;
	}
	inflateEnd(pStrm);
	// Original file buffer no longer needed
	delete[] pFile;

	// New file path (attempt to remove ".z" extension)
	std::string newPath = pName;
	u32 idx = newPath.rfind(".z");
	if (idx == -1)
	{
		// I'm sorry
		idx = newPath.rfind(".Z");
	}
	// Just making sure the extension is at the end
	// Otherwise something like "myFile.zl.png"
	// would become "myFile"
	if (idx != -1 && newPath.length() - idx == sizeof(".z"))
	{
		// If the filename ends with the zlib extension we just remove it
		newPath = newPath.substr(0, idx);
	}

	// Open output stream
	std::ofstream ofs(newPath, std::ios::binary);
	// Validate stream
	if (!ofs.is_open())
	{
		ofs.close();
		std::printf("[ERROR] File stream to %s (new file) could not be opened.\n\
			Are you sure the file exists and is not protected?", newPath.c_str());
		return false;
	}
	// Write new file (size = total bytes written by z_stream)
	ofs.write(pNewFile, pStrm->total_out);

	// Close stream
	ofs.close();
	// Free memory
	delete[] pNewFile;

	return true;
}