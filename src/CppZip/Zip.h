/*!
 * \file Zip.h
 *
 * Zip.h -- IO on .zip files using minizip and zlib
 * Version 0.2, 22.09.2012
 * part of the CppZip project - (https://github.com/flo2k/CppZip)
 *
 * Created: 13.11.2011, Florian Künzner
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

#ifndef ZIP_H_
#define ZIP_H_

#include <string>
#include <boost/signals.hpp>
#include <vector>
#include <unordered_map>
#include <memory>

/*!
 * \brief Contains all classes to zip and unzip files.
 */
namespace cppzip {
//forward declaration
struct InnerZipFileInfo;
class Unzip;

/*!
 * \brief Zip allows creating zip files
 *
 * Zip is a c++ wrapper for zip.h (minizip). It provides a c++ interface for
 * working with zip files.
 *
 * \author Florian Künzner
 * \date 22.09.2012
 * \version 0.2
 */
class Zip {
public:
	Zip(void);
	virtual ~Zip();

	/*! Open flags */
	enum OpenFlags {
		CREATE_AND_OVERWRITE,   //!< Creates an new zip file, if exists overwrites.
		APPEND_TO_EXISTING_ZIP  /*!< Append new files to the existing zip file.
		                         *   Overwriting files and deleting files are not possible.
		                         */
	};

	/*!
	 * \brief Opens a zip file.
	 *
	 * Opens a new zip given in fileName. If open() is called and
	 * isOpened() == true, then open() returns false, because
	 * another zip file is already opened.
	 *
	 * open() creates the zip file in fileName. If the fileName contains
	 * a path and a file name (eg. "path/to/file.zip") then "path/to" will
	 * be created first. After that "file.zip" will be created.
	 *
	 * If the fileName already exists, open() overrides the existing file.
	 *
	 * \param fileName is the file (incl. path) to open.
	 * \param flag     is the flag that controls, how the zip class works.
	 *
	 * \return true if the zip file is opened, otherwise false.
	 */
	bool open(const std::string & fileName, OpenFlags flag = CREATE_AND_OVERWRITE);

	/*!
	 * \brief Gets the opened status.
	 *
	 * \return true if a zip file is opened, otherwise false.
	 */
	bool isOpened(void);

	/*!
	 * \brief Adds the content into a fileName inside the zip.
	 *
	 * If the fileName already exists, false is returned.
	 * If content.length() == 0 an empty file will be created inside the zip file.
	 *
	 * Examples for fileName:
	 *
	 * - "fileName"
	 * - "fileName.txt"
	 * - "folder/subFolder/fileName.txt"
	 * - "folder\\subFolder\\fileName.txt"
	 *
	 * \note At the moment there are problems with umlauts in fileName (ä, ö, ü, ..)
	 *
	 * This is a usage example for addFile():
	 * \code
	 *  //open zip file
	 *  Zip zip;
	 *  zip->open(zipFile);
	 *
	 *  //prepare content
	 *  std::string aString("this is a string");
	 *  std::vector<unsigned char> content;
	 *  content.insert(content.end(), aString.begin(), aString.end());
	 *
	 *  //add content
	 *  zip->addFile("filename.txt", content);
	 *
	 *  //close
	 *  zip->close();
	 * \endcode
	 *
	 * \param fileName is the file to add inside the zip file.
	 * \param content is the content to add to the zip file.
	 * \return true if the content could be added to the spezified fileName,
	 *         otherwise false.
	 */
	bool addFile(const std::string & fileName, std::vector<unsigned char> & content);

	/*!
	 * \brief Adds a file from fileName into the zip.
	 *
	 * Adds the content of fileName into a new file named fileName inside the zip.
	 *
	 * If the file in fileName is empty, an empty file will be created inside the zip file.
	 * If the file in fileName doesn't exist or can't read, false is returned.
     *
     * \note At the moment there are problems with umlauts in fileName (ä, ö, ü, ..)
     *
     * This is a usage example for addFile():
	 * \code
	 *  //open zip file
	 *  Zip zip;
	 *  zip->open(zipFile);
	 *
	 *  //adds a file
	 *  zip->addFile("filename.txt");
	 *
	 *  //close
	 *  zip->close();
	 * \endcode
     *
	 * \param fileName is the file to add (must exist on file system).
	 * \param preservePath preserves the path from fileName inside the zip.
	 *        - if preservePath == true and fileName == "path/to/file" -> fileName == "path/to/file".
	 *        - if preservePath == false and fileName == "path/to/file" -> fileName == "file".
	 * \return true if the content of fileName could be added to the specified destFileName,
	 *         otherwise false.
	 */
	bool addFile(const std::string & fileName, bool preservePath = true);

	/*!
	 * \brief Adds a list of files from the file system into the zip.
	 *
	 * Adds the content of the files into new files inside the zip.
	 *
	 * If a file in fileNames is empty, an empty file will be created inside the zip file.
	 * If a file in fileNames doesn't exist or can't read, false is returned, but it tries
	 * to add as much files as possible and don't break at a failure.
	 *
	 * \note At the moment there are problems with umlauts in fileName (ä, ö, ü, ..)
	 *
	 * \param fileNames is a list of fileNames to add (fileNames must exist on file system).
	 * \param preservePath preserves the path from fileNames inside the zip.
	 *        - if preservePath == true and a fileName == "path/to/file" -> a fileName == "path/to/file".
	 *        - if preservePath == false and a fileName == "path/to/file" -> a fileName == "file".
	 * \return true if the content of fileName could be added to the specified destFileName,
	 *         otherwise false.
	 */
	bool addFiles(const std::list<std::string> & fileNames, bool preservePath = true);

	/*!
	 * \brief Adds a file from fileName into the zip.
	 *
	 * Adds the content of fileName into a new file named in destFileName inside the zip.
	 *
	 * If the destFileName already exists, false is returned.
	 * If the destFileName.length() == 0, false is returned.
	 * If the file in fileName is empty, an empty file will be created inside the zip file.
	 * If the file in fileName doesn't exist or can't read, false is returned.
	 *
	 * \note At the moment there are problems with umlauts in fileName (ä, ö, ü, ..)
	 *
	 * \param fileName is the file to add (must exist on file system).
	 * \param destFileName is the destination file name inside the zip.
	 * \return true if the content of fileName could be added to the specified destFileName,
	 *         otherwise false.
	 */
	bool addFile(const std::string & fileName, const std::string & destFileName);

	/*!
	 * \brief Adds a folder from the file system to the zip.
	 *
	 * Adds the folder and the content of the folder to the zip.
	 *
	 * If the folder already exists addFolder() == false, and nothing will be added.
	 *
	 * \note At the moment there are problems with umlauts in fileName (ä, ö, ü, ..)
	 *
	 * \param folderName is the folder to add inside the zip file.
	 * \param preservePath preserves the path from folderName inside the zip.
	 *        - if preservePath == true and folderName == "path/to/folder" -> folderName == "path/to/folder".
	 *        - if preservePath == false and folderName == "path/to/folder" -> folderName == "folder".
	 *          All files inside "path/to/folder/file" results in "folder/file".
	 * \param recursive  is a flag, that controls the add behavior:
	 *                   - if recursive == true: all the content of the folder and the
	 *                     content of the subfolders are added.
	 *                   - if recursive == false: only the files of the folder will be added.
	 * \return true if folder and the content of the folder could be added, otherwise false.
	 */
	bool addFolder(const std::string & folderName, bool preservePath = true, bool recursive = true);

	/*!
	 * \brief Adds a new empty folder inside the zip.
	 *
	 * This folder don't have to exist on the file system.
	 *
	 * If the folder already exists addEmptyFolder() == true.
	 *
	 * Examples for folderName:
	 *
	 * - "folder"
	 * - "folder/"
	 * - "folder/subFolder/subSubFolder"
	 * - "folder\\subFolder\\subSubFolder"
	 *
	 * \note At the moment there are problems with umlauts in fileName (ä, ö, ü, ..)
	 *
	 * \param folderName is the folder to add inside the zip file.
	 * \return true if folder could be added, otherwise false.
	 */
	bool addEmptyFolder(const std::string & folderName);

	/*!
	 * \brief     Deletes a file inside the zip.
	 *
	 * \attention The delete operation may be slow on big zip files.
	 *
	 * \param     fileName is the file that should be removed from the zip file.
	 *
	 * \return    true if the file is deleted, otherwise false.
	 */
	bool deleteFile(const std::string & fileName);

	/*!
	 * \brief     Deletes all files specified in fileNames inside the zip.
	 *
	 * \attention The delete operation may be slow on big zip files.
	 *
	 * \param     fileName is the file that should be removed from the zip file.
	 *
	 * \return    true if the files are deleted, otherwise false.
	 */
	bool deleteFiles(const std::list<std::string> & fileNames);


	/*!
	 * \brief     Deletes a folder incl. file inside the zip.
	 *
	 * \attention The delete operation may be slow on big zip files.
	 *
	 * \param     folderName is the folder that should be removed from the zip file.
	 *
	 * \return    true if the folder is deleted, otherwise false.
	 */
	bool deleteFolder(const std::string & folderName);

	/*!
	 * \brief     Deletes a folders incl. file inside the zip.
	 *
	 * \attention The delete operation may be slow on big zip files.
	 *
	 * \param     folderNames are the folder that should be removed from the zip file.
	 *
	 * \return    true if the folders are deleted, otherwise false.
	 */
	bool deleteFolders(const std::list<std::string> & folderNames);

	/*!
	 * \brief Replaces the existing file inside the zip file with given the content.
	 *
	 * If the fileName doesn't exist inside the zip, the file will be added.
	 *
	 * \attention The replace operation may be slow on big zip files.
	 *
	 * \note At the moment there are problems with umlauts in fileName (ä, ö, ü, ..)
	 *
	 * \param  fileName is the file that should be replaced inside the zip file.
	 * \param  content  is the new content of fileName.
	 *
	 * \return true if the file could be replaced or added, otherwise false.
	 */
	bool replaceFile(const std::string & fileName, std::vector<unsigned char> & content);

	/*!
	 * \brief Replaces the existing file inside the zip file with the content of the file.
	 *
	 * If the fileName doesn't exist inside the zip, the file will be added.
	 *
	 * \attention The replace operation may be slow on big zip files.
	 *
	 * \note At the moment there are problems with umlauts in fileName (ä, ö, ü, ..)
	 *
	 * \param fileName     is the file to add (must exist on file system).
	 * \param destFileName is the destination file name inside the zip that should be replaced.
	 *
	 * \return true if the file could be replaced or added, otherwise false.
	 */
	bool replaceFile(const std::string & fileName, const std::string & destFileName);

	//bool addFilter(std::string filter); //??

	/*!
	 * \brief Sets the compression level.
	 *
	 * Allowed values are from 0 to 9.
	 *  - 0 = no compression, only saving in zip
	 *  - 9 = maximum compression
	 *  - default = 6
	 *
	 * \param compressionLevel is the level of compression for all files
	 * \return true if the compressionLevel could set, otherwise false
	 *
	 * \see getCompressionLevel()
	 */
	bool setCompressionLevel(int compressionLevel);

	/*!
	 * \brief Gets the compression level.
	 *
	 * \return the compression level.
	 * \see setCompressionLevel()
	 */
	size_t getCompressionLevel(void);

	/*!
	 * \brief Closes the zip file.
	 *
	 * When close() is called, the zip headers are written to the end of
	 * the zip file.
	 *
	 * \return true if the zip file is successfully closed, otherwise false.
	 */
	bool close(void);

public:
	/*!
	 * This signal is emitted, before a file will be extracted. The parameter
	 * std::string& describes the path (incl. file name) of the files
	 * destination inside the zip. The value can be changed to a new one.
	 *
	 * std::string & is the destination inside the zip file
	 */
	boost::signal<void (std::string &)> beforeFileZipping;

	/*!
	 * This signal is emitted, after a file is extracted. The parameter
	 * std::string describes the path (incl. file name) of the files
	 * destination. The value can be changed to a new one.
	 *
	 * std::string & is the destination
	 * unsigned int is the maximum number of files to extract
	 * unsigned int is the current file that is extracted (begins with 1)
	 */
	boost::signal<void (const std::string, unsigned int, unsigned int)> fileZipped;

private:
	/*!
	 * Retrieves the file infos of an existing zip file.
	 *
	 * If the file doesn't exist or an error occurs, an empty map will be returned;
	 *
	 * \return the zip file infos.
	 */
	std::unordered_map<std::string, std::shared_ptr<InnerZipFileInfo> >
		retrieveFileInfos(const std::string & fileName);

	/*!
	 * Clears the internal members.
	 */
	void clear(void);

	// Helpers for adding file/folder
	/*!
	 * Creates a folder on the file system with all subfolders if not exists
	 *
	 * \param path to create
	 * \return true if path exists or created, otherwise false.
	 */
	bool createFolderIfNotExists(const std::string & path);

	std::shared_ptr<InnerZipFileInfo> getFileInfoForANewFile(const std::string & fileName);
	std::shared_ptr<InnerZipFileInfo> getFileInfoForAExistingFile(const std::string & fileName);
	std::shared_ptr<InnerZipFileInfo> getFileInfoFromLocalFileInfos(const std::string & fileName);

	/*!
	 * Gets the platform depended file attributes from an exiting file on the file system.
	 *
	 * \return The file attributes.
	 */
	unsigned long getExternalFileAttributesFromExistingFile(const std::string &fileName);

	bool addFile_internal(
			std::shared_ptr<InnerZipFileInfo> info,
			std::vector<unsigned char> & content);

	bool addFile_internal(
				std::shared_ptr<InnerZipFileInfo> info,
				const std::string & fileName);

	bool containsFile(const std::string & fileName);
	bool containsAnyFile(const std::list<std::string> & fileNames);

	bool addFolder(
			const std::string & realFolderName, const std::string & relativeFolderName,
			bool preservePath = true, bool recursive = true);
	bool addFolderChilds(
				const std::string & realFolderName, const std::string & folderNameToAdd,
				bool preservePath = true, bool recursive = true);
	bool addFolder_internal(std::shared_ptr<InnerZipFileInfo> info);

	// Helpers for deleting file/folder
	/*!
	 * Moves the current zip to an temp zip.
	 *
	 * \return the name of the file name if successfully moved, otherwise an empty string.
	 */
	std::string moveTheCurrentZipToAnTempZip(void);

	/*!
	 * Copies all files and folders into a new zip, except the fileName.
	 *
	 * \return true if all is ok, otherwise false.
	 */
//	bool copyAllFilesAndFoldersIntoANewZipFileExceptTheFileName(const std::string & tempZipFile,
//																const std::string & fileName,
//																bool isFileNameAFolder);

	bool copyAllFilesAndFoldersIntoANewZipFileExceptTheFileNames(const std::string & tempZipFile,
																 const std::list<std::string> & fileNames,
																 bool areFileNamesFolders);

	bool copyFile(Unzip & unzip, const std::string & fileName);

	/*!
	 * Cleans up the temporary files and tries to restore the original zip file.
	 *
	 * \return true if the cleanup was successful, otherwise false.
	 */
	bool cleanUpAfterCopying(bool ok, const std::string & tempZipFile);

	/*!
	 * Restores the old open status, with the zip file was opened.
	 */
	void restoreTheOldOpenStatus(OpenFlags oldOpenState);

private:
	typedef void * voidp;
	typedef voidp zipFile;
	std::string zipFileName;
	std::unordered_map<std::string, std::shared_ptr<InnerZipFileInfo> > fileInfos;

	zipFile zipfile_handle;
	OpenFlags openFlag;
	int compressionLevel;
};

} //cppzip

#endif /* ZIP_H_ */
