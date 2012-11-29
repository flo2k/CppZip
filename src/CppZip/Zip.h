/*!
 * Zip.h -- IO on .zip files using minizip and zlib
 * Version 0.2, 22.09.2012
 * part of the CppZip project - (https://github.com/flo2k/CppZip)
 *
 * Created: 13.11.2011, Florian Künzner
 *
 * Copyright (C) 2011-2012 Florian Künzner (CppZip) (https://github.com/flo2k/CppZip)
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
#include <map>
#include <memory>

namespace cppzip {

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
		APPEND_TO_EXISTING_ZIP, /*!< Append new files to the existing zip file.
		                         *   Overwriting files and deleting files are not possible.
		                         */
		OVERWRITE_AND_DELETE    /*!< Allows to overwrite and delete files from an existing zip file.
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
	 * \note At the Moment there are Problems with Umlauts in fileName (ä, ö, ü, ..)
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
	bool addFile(const std::string & fileName, std::vector<unsigned char> content);

	/*!
	 * \brief Adds a file from fileName into the zip.
	 *
	 * Adds the content of fileName into a new file named fileName inside the zip.
	 *
	 * If the file in fileName is empty, an empty file will be created inside the zip file.
	 * If the file in fileName doesn't exist or can't read, false is returned.
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
	 * \param preservePath preserves the from fileName inside the zip.
	 * \return true if the content of fileName could be added to the specified destFileName,
	 *         otherwise false.
	 */
	bool addFile(const std::string & fileName, bool preservePath = true);

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
	 * \param fileName is the file to add (must exist on file system).
	 * \param destFileName is the destination file name inside the zip.
	 * \return true if the content of fileName could be added to the specified destFileName,
	 *         otherwise false.
	 */
	bool addFile(const std::string & fileName, const std::string & destFileName);
	bool addFolder(const std::string & folderName, bool recursive);

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
	 * \note At the Moment there are Problems with Umlauts in fileName (ä, ö, ü, ..)
	 *
	 * \param folderName is the folder to add inside the zip file.
	 * \return true if folder could be added, otherwise false.
	 */
	bool addEmptyFolder(const std::string & folderName);

	bool addFilter(std::string filter); //??

	/*!
	 * Sets the compression level. Allowed values are from 0 to 9.
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
	 * Gets the compression level.
	 *
	 * \return the compression level.
	 * \see setCompressionLevel()
	 */
	size_t getCompressionLevel(void);

	/*!
	 * Closes the zip file.
	 *
	 * \details When close() is called, the zip headers are written to the end of
	 *         the zip file.
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
	 * Clears the internal members.
	 */
	void clear(void);

	/*!
	 * Creates a directory on the file system with all subdirs if not exists
	 *
	 * \param path to create
	 * \return true if path exists or created, otherwise false.
	 */
	bool createDirectoryIfNotExists(const std::string & path);

	std::vector<unsigned char> getFileContent(const std::string & fileName);

	/*!
	 * InnerZipFileInfo contains all Infos to save a file into zip.
	 * The informations are very similar to zip_fileinfo from zip.h
	 */
	typedef struct {
		std::string fileName;
		std::string extraField;
		std::string comment;
		unsigned int time_sec;            /*! seconds after the minute - [0,59] */
		unsigned int time_min;            /*! minutes after the hour - [0,59] */
		unsigned int time_hour;           /*! hours since midnight - [0,23] */
		unsigned int time_day_of_month;   /*! day of the month - [1,31] */
		unsigned int time_month;          /*! months since January - [0,11] */
		unsigned int time_year;           /*! years - [1980..2044] */
		unsigned long dosDate;            /*! if dos_date == 0, tmu_date is used */
		unsigned long internal_fileAttributes;
		unsigned long external_fileAttributes;
	} InnerZipFileInfo;

	std::shared_ptr<InnerZipFileInfo> getFileInfo(const std::string & fileName);

	/*!
	 * Gets the platform depended file attributes from an exiting file on the file system.
	 *
	 * \return The file attributes.
	 */
	unsigned long getExternalFileAttributesFromExistingFile(const std::string &fileName);

	bool addFile_internal(
			std::shared_ptr<InnerZipFileInfo> info,
			std::vector<unsigned char> content);
	bool containsFile(std::string & fileName);

	bool addFolder_internal(const std::string & folderName);

private:
	typedef void * voidp;
	typedef voidp zipFile;

	std::map<std::string, std::shared_ptr<InnerZipFileInfo> > files;

	zipFile zipfile_handle;
	int compressionLevel;
};

} //cppzip

#endif /* ZIP_H_ */
