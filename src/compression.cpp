#include "compression.h"
#include "types.h"
#include <zlib/zlib.h>
#include <fstream>
#include <string>

int clubCompress(char* pName)
{
	// Should never happen but hey
	if (!pName)
	{
		std::printf("[ERROR] Filename is null.");
		return Z_ERRNO;
	}

	// Open filestream (start at end of file)
	std::ifstream ifs(pName, std::ios::binary | std::ios::ate);

	// Make sure stream actually is open
	if (!ifs.is_open())
	{
		ifs.close();
		std::printf("[ERROR] File stream to %s (original file) could not be opened.\n\
			Are you sure the file exists and is not protected?", pName);
		return Z_STREAM_ERROR;
	}

	// Get + validate filesize
	u32 fileSize = (u32)ifs.tellg();
	if (fileSize < 1)
	{
		std::printf("[ERROR] Filesize error");
		return Z_DATA_ERROR;
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
		return Z_BUF_ERROR;
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
		return Z_STREAM_ERROR;
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

	return Z_OK;
}

int clubDecompress(char* pName)
{
	// Should never happen but hey
	if (!pName)
	{
		std::printf("[ERROR] Filename is null.");
		return Z_ERRNO;
	}



	return Z_OK;
}

//const char* zGetError(int err)
//{
//	return zError[err];
//}