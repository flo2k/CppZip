/*!
 * \file Unzip.h
 *
 * Unzip.h -- IO on .zip files using minizip and zlib
 * Version 0.2, 22.09.2012
 * part of the CppZip project - (https://github.com/flo2k/CppZip)
 *
 * Created: 05.11.2011, Florian Künzner
 *
 * Copyright (C) 2011 Florian Künzner (CppZip)
 * Copyright (C) 2012 Florian Künzner and Andreas Bauer (CppZip) (https://github.com/flo2k/CppZip)
 *
 * ---------------------------------------------------------------------------
 *
 * Condition of use and distribution are the same than minizip and zlib :
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * ---------------------------------------------------------------------------
 *
 */

#ifndef UNZIP_H_
#define UNZIP_H_

#include <string>
#include <list>
#include <unordered_map>
#include <vector>
#include <memory>
#include <boost/signal.hpp>

namespace cppzip {
//forward declaration
struct InnerZipFileInfo;

/*!
 * \brief Unzip allows reading files inside a zip file
 *
 * Unzip is a c++ wrapper for unzip.h (minizip). It provides a c++ interface for
 * working with existing zip files.
 *
 * \author Florian Künzner
 * \date 22.09.2012
 * \version 0.2
 */
class Unzip {
public:
	Unzip(void);
	~Unzip();

	/*!
	 * \brief Opens a zip given in zipFile file.
	 *
	 * If open() is called and isOpened() == true,
	 * then open() returns false, because another zip
	 * file is already opened.
	 *
	 * \param zipFile is the file (incl. path) to open
	 * \param password the password which protects the files within the zip files
	 *
	 * \return true if the zip file is opened, otherwise false.
	 */
	bool open(const std::string & zipFile, const std::string & password = "");

	/*!
	 * \brief Closes a zip file.
	 *
	 * If it couldn't close it returns false. If no zip file was
	 * opened it returns true.
	 *
	 * \return true if the close() was successful, otherwise false.
	 */
	bool close(void);

	/*!
	 * \brief Gets the opened status.
	 *
	 * Returns true if a zip file is opened. When a new object of Unzip is
	 * created and open() is never called, then it returns always false;
	 *
	 * \return true if a zip file is opened
	 */
	bool isOpened(void);

	/*!
	 * \brief Returns the number of files (files, folders) inside the zip file.
	 *
	 * If no zip file is opened getNumElements() == 0.
	 *
	 * \note A folder is also a file.
	 *
	 * \return number of elements in the zip file
	 */
	int getNumFiles(void);

	/*!
	 * \brief Returns if a file is inside the zip file.
	 *
	 * If no zip file is opened containsFile() == false
	 *
	 * \return true if the file is inside the zip
	 */
	bool containsFile(const std::string & fileName);

	/*!
	 * \brief Checks if path is a file.
	 *
	 * \param path to check
	 * \return true if path is a file, otherwise false.
	 */
	bool isFile(const std::string & path);

	/*!
	 * \brief Checks if path is a folder.
	 *
	 * \note Inside a zip file, a folder ends with "/"
	 *
	 * \param path to check
	 * \return true if path is a folder, otherwise false.
	 */
	bool isFolder(const std::string & path);

	/*!
	 * \brief Returns the containing file names (files, folders) in the zip file.
	 *
	 * If zip file is not opened or zip file is empty, an empty list
	 * will be returned.
	 *
	 * \note A folder is also a file and is in the list of file names.
	 *
	 * \return list of file names
	 */
	std::list<std::string> getFileNames(void);

	/*!
	 * \brief Get the file content.
	 *
	 * If file is a folder or doesn't exist an empty list will be returned.
	 *
	 * This is a usage example of getFileNames():
	 * \code
	 *  //open zip file
	 *  Unzip zip;
	 *  zip.open("file.zip");
	 *
	 *  //retrieve content
	 *  std::vector<unsigned char> content = zip->getFileContent("filename.txt");
	 *
	 *  //convert to a std::string and print it to console
	 *  std::string contantAsString(content.begin(), content.end());
	 *  std::cout << "content: " << contantAsString << std::endl;
	 *
	 *  //close zip
	 *  zip.close();
	 * \endcode
	 *
	 * \return the content of a file as vector of unsigned char
	 */
	std::vector<unsigned char> getFileContent(const std::string & fileName);

	/*!
	 * \brief Extracts the file in fileName to the path.
	 *
	 * path = "/path/to/file"
	 *
	 * If path doesn't exist, extractFileTo trys to create it
	 *
	 * This is a usage example of extractFileTo():
	 * \code
	 *  //open zip file
	 *  Unzip zip;
	 *  zip.open("file.zip");
	 *
	 *  //extract file
	 *  zip->extractFileTo("fileInsideZip.txt", "pathOnFileSystem.txt");
	 *
	 *  //close zip
	 *  zip.close();
	 * \endcode
	 *
	 * \param fileName file that should be extracted
	 * \param path is the destination
	 *
	 * \return true if extraction was successful, otherwise false.
	 */
	bool extractFileTo(const std::string & fileName, const std::string & path);

	/*!
	 * \brief Extracts the contents of zip file to the given path.
	 *
	 * \note If the path doesn't exists, extractAllTo() tries to
	 * create the paths and all subdirs.
	 *
	 * \param path is a folder
	 * \return true if all is extracted, otherwise false
	 */
	bool extractAllTo(const std::string & path);

public:
	/*!
	 * This signal is emitted, before a file will be extracted. The parameter
	 * std::string& describes the path (incl. file name) of the files
	 * destination. The value can be changed to a new one.
	 *
	 * std::string & is the destination
	 */
	boost::signal<void (std::string &)> beforeFileExtraction;

	/*!
	 * This signal is emitted, after a file is extracted. The parameter
	 * std::string describes the path (incl. file name) of the files
	 * destination. The value can be changed to a new one.
	 *
	 * std::string & is the destination
	 * unsigned int is the maximum number of files to extract
	 * unsigned int is the current file that is extracted (begins with 1)
	 */
	boost::signal<void (const std::string, unsigned int, unsigned int)> fileExtracted;

private:
	/*!
	 * Clears the internal members.
	 */
	void clear(void);

	/*!
	 * Get's the global info of the zip file
	 * - sets the numFiles
	 */
	void getGlobalInfo(void);

	/*!
	 * Makes the file given in fileName to the current file.
	 *
	 * \param fileName to make to current file
	 * \return true if operation was ok, otherwise false.
	 */
	bool goToFile(const std::string & fileName);

	/*!
	 * Reads all elements in the zip file
	 */
	void retrieveAllFileInfos(void);

	/*!
	 * Creates a folder with all subdirs if not exists
	 *
	 * \param path to create
	 * \return true if path exists or created, otherwise false.
	 */
	bool createFolderIfNotExists(const std::string & path);

	/*!
	 * Extracts the file to the given path. Bevor a file is extracted
	 * the signal beforeFileExtraction is emitted and the user has the
	 * posibility to change the path.
	 *
	 * After the extraction is successfully done the fileExtracted signal
	 * is emitted. When extraction fails, the signal will NOT be emitted.
	 *
	 * \param fileName file that should be extracted
	 * \param path is the destination
	 * \param max describes how much files to extract
	 * \param current the current file that is extracted (begins with 1)
	 *
	 * \return true if extraction was successful, otherwise false.
	 */
	bool extractFileTo_Internal(
			const std::string & fileName,
			const std::string & path,
			int max,
			int current);

	std::shared_ptr<InnerZipFileInfo> getFileInfoFromLocalFileInfos(const std::string & fileName);

	/*
	 * Formats the password for the use with the minizip api
	 *
	 * \param password the password that shall be formatted
	 */
	const char* formatPassword(const std::string & password);

private:
	typedef void * voidp;
	typedef voidp unzFile;

	unzFile zipfile_handle;
	int numFiles;

	typedef std::pair<std::string, InnerZipFileInfo> FileInfoPair;
	std::unordered_map<std::string, std::shared_ptr<InnerZipFileInfo> > fileInfos;

	friend class Zip;
	std::string password;
};

} //cppzip

#endif /* UNZIP_H_ */
