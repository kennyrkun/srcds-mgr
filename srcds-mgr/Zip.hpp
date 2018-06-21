#ifndef ZIP_HPP
#define ZIP_HPP

#include <string>

// A wrapper for miniz ZIP library.
// Purpose: Use miniz more easily and with object orientation.
class Zip
{
public:
	Zip();
	~Zip();

	// The name of the archive we're dealing with.
	std::string archiveName;
	// The path to the archive.
	std::string archivePath;

	// Add a single file to the archive
	// filename must include path.
	int addFileToArchive(std::string filename);

	// Remove a single file from the archive.
	// filename must include path.
	int removeFileFromArchive(std::string filename);
	
	// Add a single directory to the archive.
	// name must include path if archive is not top level.
	int addDirectoryToArchive(std::string name);
	// Removes a single directory from the archive by name.
	// name must include path if archive is not top level.
	int removeDirectoryFromArchive(std::string name, bool removeIfNotEmpty = false);

	// Compresses the archive. Blocking unless run in another thread.
	int compressArchive();

	// Compresses the archive one step at a time.
	// Developer must do this for the entire size of the file, probably in a for loop.
	int compressArchiveSteps();
	// Returns the compression step number.
	// Useful for progress bars and such.
	int getCompressionProgress();

	// Decompresses the archive. Blocking unless run in another thread.
	int decompressArchive();

	// Decompress the archive in steps.
	// Developer must do this for the entire size of the file, probably in a for loop.
	int decompressArchiveSteps();
	// Returns the decompression step.
	// Useful for progress bars and such.
	int getDecompressionSteps();

	int extractFileFromArchive(std::string filename);
};

#endif // !ZIP_HPP
