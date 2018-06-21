#include "Zip.hpp"

#if defined(__GNUC__)
	// Ensure we get the 64-bit variants of the CRT's file I/O calls
	#ifndef _FILE_OFFSET_BITS
		#define _FILE_OFFSET_BITS 64
	#endif

	#ifndef _LARGEFILE64_SOURCE
		#define _LARGEFILE64_SOURCE 1
	#endif
#endif

#include "miniz.h"

#include <stdio.h>
#include <iostream>
#include <vector>

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint;

Zip::Zip()
{
	std::cout << "using miniz.c version: " << MZ_VERSION <<   std::endl;
}

Zip::~Zip()
{
}

int Zip::addFileToArchive(std::string filename)
{
	// TODO: see example2.c
}

int Zip::removeFileFromArchive(std::string filename)
{
	// TODO: see example2.c
}

int Zip::addDirectoryToArchive(std::string name)
{
	// TODO: see example2.c
}

int Zip::removeDirectoryFromArchive(std::string name, bool removeIfNotEmpty)
{
	// TODO: see example2.c
}

int Zip::compressArchive(std::string filename)
{
	return 0;
}

int Zip::decompressArchive(std::string filename)
{
	return 0;
}

int Zip::extractFileFromArchive(std::string filename, std::string destination)
{
	std::cout << "extracting " << filename << " from " << archivePath + archiveName << std::endl;

	mz_bool status;
	mz_zip_archive zip_archive;

	std::string temp_filename = archivePath + archiveName;
	const char *archiveFilename = temp_filename.c_str();

	memset(&zip_archive, 0, sizeof(zip_archive));

	status = mz_zip_reader_init_file(&zip_archive, archiveFilename, 0);
	if (!status)
	{
		printf("mz_zip_reader_init_file() failed!\n");
		return EXIT_FAILURE;
	}

	for (int i = 0; i < (int)mz_zip_reader_get_num_files(&zip_archive); i++)
	{
		mz_zip_archive_file_stat file_stat;
		if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat))
		{
			printf("mz_zip_reader_file_stat() failed!\n");
			mz_zip_reader_end(&zip_archive);
			return EXIT_FAILURE;
		}

		// TODO: find a better way to do this
		if (filename == file_stat.m_filename)
		{
			std::string temp_filename = destination + filename;

			if (!mz_zip_reader_extract_to_file(&zip_archive, i, temp_filename.c_str(), 0))
			{
				std::cerr << "failed to extract file" << std::endl;
				return EXIT_FAILURE;
			}
		}

	}

	mz_zip_reader_end(&zip_archive);

	printf("Success.\n");
	return EXIT_SUCCESS;
}
