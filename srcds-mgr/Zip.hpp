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
	int compressArchive(std::string filename);

	// Decompresses the archive. Blocking unless run in another thread.
	int decompressArchive(std::string filename);

	// Extracts a specific file from the archive.
	int extractFileFromArchive(std::string filename, std::string destination);
};

#endif // !ZIP_HPP
