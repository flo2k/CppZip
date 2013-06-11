/*
 * Zip.cpp
 *
 *  Created on: 13.11.2011
 *      Author: Florian Künzner
 */

#include "Zip.h"
#include "ZipCommon.h"
#include "minizip/zip.h"
#include "minizip/unzip.h"
#include "Unzip.h"
#include "ZipPrivate.h"

#include <algorithm>
#include <fstream>
#include <vector>
#include <list>
#include <time.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/conversion.hpp>

#ifndef WIN32
	#include <sys/types.h>
	#include <sys/stat.h>
#else
	#include <windows.h>
#endif

namespace cppzip {

#define CPPZIP_ZIP_CHAR_ARRAY_BUFFER_SIZE 65536

Zip::Zip()
	: zipfile_handle(NULL), openFlag(CreateAndOverwrite),
	  compressionLevel(Z_DEFAULT_COMPRESSION)
{

}

Zip::~Zip()
{
	close();
}

bool Zip::open(const std::string & fileName,
			   const OpenFlags & flag,
			   const std::string & password)
{
	if(isOpened()){ //if already opened, don't open a file
		return false;
	}

	this->openFlag = flag;
	this->password = password;

	boost::filesystem::path path(fileName);
	path = path.remove_filename();

	if(! createFolderIfNotExists(path.string())){
		return false;
	}

	switch (this->openFlag) {
		case Zip::OpenExisting:{
			this->fileInfos = retrieveFileInfos(fileName);
			this->zipfile_handle = zipOpen(fileName.c_str(), APPEND_STATUS_ADDINZIP);
			break;
		}
		default:{
			this->zipfile_handle = zipOpen(fileName.c_str(), APPEND_STATUS_CREATE);
			break;
		}
	}

	if(isOpened()){
		this->zipFileName = fileName;
	}

	return isOpened();
}

bool Zip::isOpened(void)
{
	return zipfile_handle != NULL;
}

std::unordered_map<std::string, std::shared_ptr<InnerZipFileInfo> >
	Zip::retrieveFileInfos(const std::string & fileName)
{
	std::unordered_map<std::string, std::shared_ptr<InnerZipFileInfo> > fileInfos_;

	Unzip unzip;
	bool ok = unzip.open(fileName);
	if(!ok){
		return fileInfos_;
	}

	fileInfos_ = unzip.fileInfos;

	unzip.close();

	return fileInfos_;
}

bool Zip::addFile(const std::string & fileName, const std::vector<unsigned char> & content)
{
	std::shared_ptr<InnerZipFileInfo> info = getFileInfoForANewFile(fileName);

	return addFile_internal(info, content);
}

bool Zip::addFile_internal(
		std::shared_ptr<InnerZipFileInfo> info, const std::vector<unsigned char> & content)
{
	if(containsFile(info->fileName) || info->fileName.length() == 0){
		return false;
	}

	zip_fileinfo zipFileInfo = convertInnerZipFileInfo_to_zipFileInfo(info);

	//open file inside zip
	if(ZIP_OK != zipOpenNewFileInZip4_64 (
					zipfile_handle,
					info->fileName.c_str(),
					&zipFileInfo,
			        NULL,
			        0,
			        NULL,
			        0,
			        info->comment.c_str(),
			        Z_DEFLATED,
			        compressionLevel,
			        0,
			        -MAX_WBITS,
			        DEF_MEM_LEVEL,
			        Z_DEFAULT_STRATEGY,
			        formatPassword(this->password),
			        0,
			        VERSIONMADEBY,
			        0,
			        0)){
		return false;
	}

	//write content
	if(ZIP_OK != zipWriteInFileInZip(zipfile_handle, content.data(), content.size())){
		//try to close...
		zipCloseFileInZip(zipfile_handle);
		return false;
	}

	fileInfos[info->fileName] = info;

	//close file
	if(ZIP_OK != zipCloseFileInZip(zipfile_handle)){
		return false;
	}

	return true;
}

bool Zip::containsFile(const std::string & fileName)
{
	int count = fileInfos.count(fileName);
	return count == 1;
}

bool Zip::addFile(const std::string & fileName, bool preservePath)
{
	if(! preservePath){
		boost::filesystem::path fileToAdd(fileName);
		std::string destinationFileName = fileToAdd.filename().string();
		return addFile(fileName, destinationFileName);
	}

	std::shared_ptr<InnerZipFileInfo> info;

	try{
		info = getFileInfoForAExistingFile(fileName);
	} catch(std::exception & e){
		return false;
	}

	return addFile_internal(info, fileName);
}

bool Zip::addFile_internal(std::shared_ptr<InnerZipFileInfo> info, const std::string& fileName)
{
	if(containsFile(info->fileName) || info->fileName.length() == 0){
		return false;
	}

	//open the file on filesystem
	boost::filesystem::ifstream ifs(fileName, std::ios::in | std::ios::binary);

	if(! ifs.is_open()){
		return false;
	}

	zip_fileinfo zipFileInfo = convertInnerZipFileInfo_to_zipFileInfo(info);

	//open file inside zip
	if(ZIP_OK != zipOpenNewFileInZip4_64 (
			zipfile_handle,
			info->fileName.c_str(),
			&zipFileInfo,
	        NULL,
	        0,
	        NULL,
	        0,
	        info->comment.c_str(),
	        Z_DEFLATED,
	        compressionLevel,
	        0,
	        -MAX_WBITS,
	        DEF_MEM_LEVEL,
	        Z_DEFAULT_STRATEGY,
	        formatPassword(this->password),
	        0,
	        VERSIONMADEBY,
	        0,
	        0)){
		return false;
	}

	//copy the file contents
	char buffer[CPPZIP_ZIP_CHAR_ARRAY_BUFFER_SIZE];
	while (ifs.good()) {
		ifs.read(buffer, CPPZIP_ZIP_CHAR_ARRAY_BUFFER_SIZE);
		unsigned int len = static_cast<unsigned int>(ifs.gcount());

		if(len > 0){
			bool ok = ZIP_OK == zipWriteInFileInZip(zipfile_handle, buffer, len);
			if(!ok){
				//try to close...
				zipCloseFileInZip(zipfile_handle);
				return false;
			}
		}
	}

	ifs.close();

	fileInfos[info->fileName] = info;

	//close file
	if(ZIP_OK != zipCloseFileInZip(zipfile_handle)){
		return false;
	}

	return true;
}

std::shared_ptr<InnerZipFileInfo> Zip::getFileInfoForANewFile(const std::string & fileName)
{
	std::shared_ptr<InnerZipFileInfo> info(new InnerZipFileInfo);

	info->fileName = fileName;
	info->dosDate = 0;

	//time: now!
	boost::posix_time::ptime posixTime = boost::posix_time::second_clock::universal_time();
	std::tm time = ::boost::posix_time::to_tm(posixTime);
	info->time_year = time.tm_year;
	info->time_month = time.tm_mon;
	info->time_day_of_month = time.tm_mday;
	info->time_hour = time.tm_hour;
	info->time_min = time.tm_min;
	info->time_sec = time.tm_sec;

	info->crc = 0;
	info->compressed_size = 0;
	info->uncompressed_size = 0;

	//file attributes
	info->internal_fileAttributes = 0;
	info->external_fileAttributes = 0; //the external file attributes depends on the platform
                                       //and is on linux and windows different!
	return info;
}

std::shared_ptr<InnerZipFileInfo> Zip::getFileInfoForAExistingFile(const std::string & fileName)
{
	std::shared_ptr<InnerZipFileInfo> info(new InnerZipFileInfo);

	info->fileName = fileName;
	info->dosDate = 0;

	//time: now!
	boost::posix_time::ptime posixTime = boost::posix_time::second_clock::universal_time();
	std::tm time = ::boost::posix_time::to_tm(posixTime);
	info->time_year = time.tm_year;
	info->time_month = time.tm_mon;
	info->time_day_of_month = time.tm_mday;
	info->time_hour = time.tm_hour;
	info->time_min = time.tm_min;
	info->time_sec = time.tm_sec;

	info->crc = 0;
	info->compressed_size = 0;
	info->uncompressed_size = 0;

	//file attributes
	info->internal_fileAttributes = 0;
	info->external_fileAttributes = this->getExternalFileAttributesFromExistingFile(fileName); //the external file attributes depends on the platform
                                                                                                //and is on linux and windows different!
	return info;
}

std::shared_ptr<InnerZipFileInfo> Zip::getFileInfoFromLocalFileInfos(const std::string & fileName)
{
	return fileInfos[fileName];
}

unsigned long Zip::getExternalFileAttributesFromExistingFile(
		const std::string& fileName)
{
	boost::filesystem::path path(fileName);
	unsigned long externalAttributes = 0;

	#ifdef WIN32
		externalAttributes = GetFileAttributes(path.string().c_str()); //windows.h function
	#else //on linux
	    struct stat pathStat;
	    if(stat(path.c_str(), &pathStat) == 0) //linux function
	    {
	    	externalAttributes = pathStat.st_mode;
	    }
	#endif

	return externalAttributes;
}

bool Zip::addFiles(const std::list<std::string> & fileNames, bool preservePath)
{
	bool ok = true;

	std::list<std::string>::const_iterator fileNameIter;
	for(fileNameIter = fileNames.cbegin(); fileNameIter != fileNames.cend(); ++fileNameIter){
		const std::string & fileName = *fileNameIter;
		if(! addFile(fileName, preservePath)){
			ok = false;
		}
	}

	return ok;
}

bool Zip::addFile(const std::string & fileName, const std::string & destFileName)
{
	if(destFileName.length() == 0){
		return false;
	}

	std::shared_ptr<InnerZipFileInfo> info;

	try{
		info = getFileInfoForAExistingFile(fileName);
		info->fileName = destFileName;
	} catch(std::exception & e){
		return false;
	}

	return addFile_internal(info, fileName);
}

bool Zip::addFolder(const std::string & folderName, bool preservePath, bool recursive)
{
	std::string relativeFolderName = boost::filesystem::path(folderName).filename().string();
	return addFolder(folderName, relativeFolderName, preservePath, recursive);
}

bool Zip::addFolder(
			const std::string & realFolderName, const std::string & relativeFolderName,
			bool preservePath, bool recursive)
{
	std::string folderNameToAdd;

	if(preservePath){
		folderNameToAdd = boost::algorithm::replace_all_copy(realFolderName, "\\", "/");
	} else {
		folderNameToAdd = relativeFolderName;
	}

	if(! boost::algorithm::ends_with(folderNameToAdd, "/")){
		folderNameToAdd += "/";
	}

	if(containsFile(folderNameToAdd)){
		return false;
	}

	std::shared_ptr<InnerZipFileInfo> info = getFileInfoForAExistingFile(realFolderName);
	info->fileName = folderNameToAdd;
	if(! addFolder_internal(info)){
		return false;
	}

	return addFolderChilds(realFolderName, folderNameToAdd, preservePath, recursive);
}

bool Zip::addFolderChilds(
				const std::string & realFolderName, const std::string & folderNameToAdd,
				bool preservePath, bool recursive)
{
	boost::filesystem::directory_iterator iter(realFolderName);
	boost::filesystem::directory_iterator end;

	for(; iter != end; ++iter){
		boost::filesystem::path dirEntry = *iter;
		std::string destName;

		if(preservePath){
			destName = dirEntry.string();
		} else {
			destName= folderNameToAdd + dirEntry.filename().string();
		}

		boost::algorithm::replace_all(destName, "\\", "/");

		if(boost::filesystem::is_regular_file(dirEntry)){
			if(! addFile(dirEntry.string(), destName)){
				return false;
			}

		} else if(boost::filesystem::is_directory(dirEntry) && recursive){

			if(! addFolder(dirEntry.string(), destName, preservePath, recursive)){
				return false;
			}
		}
	}

	return true;
}

bool Zip::addEmptyFolder(const std::string & folderName)
{
	std::string folderNameToAdd = boost::algorithm::replace_all_copy(folderName, "\\", "/");
	if(! boost::algorithm::ends_with(folderNameToAdd, "/")){
		folderNameToAdd += "/";
	}

	std::shared_ptr<InnerZipFileInfo> info = getFileInfoForANewFile(folderNameToAdd);

	return addFolder_internal(info);
}

bool Zip::addFolder_internal(std::shared_ptr<InnerZipFileInfo> info)
{
	if(containsFile(info->fileName)){
		return true;
	}

	std::vector<unsigned char> emptyData;
	return addFile_internal(info, emptyData);
}

bool Zip::deleteFile(const std::string & fileName)
{
	std::list<std::string> fileNames;
	fileNames.push_back(fileName);

	return deleteFiles(fileNames);
}

bool Zip::deleteFiles(const std::list<std::string> & fileNames)
{
	if(! isOpened()){
		return false;
	}

	//remember the openFlag
	enum OpenFlags oldOpenFlag = openFlag;

	//check if a file or a folder with a name in fileNames exists
	if(! containsAnyFile(fileNames)){
		return true;
	}

	//close the current zip
	if(! close()){
		return false;
	}

	//move the current zip to an tempzip
	std::string tempZipFile = moveTheCurrentZipToAnTempZip();
	if(tempZipFile.empty()){
		return false;
	}

	//Copy all files and folders into a new zip file, except the fileNames
	bool ok = copyAllFilesAndFoldersIntoANewZipFileExceptTheFileNames(tempZipFile, fileNames, false);
	cleanUpAfterCopying(ok, tempZipFile);

	//restore the old open status if necessary
	restoreTheOldOpenStatus(oldOpenFlag);

	return ok;
}

bool Zip::containsAnyFile(const std::list<std::string> & fileNames)
{
	std::list<std::string>::const_iterator fileNameIter;
	for(fileNameIter = fileNames.cbegin(); fileNameIter != fileNames.cend(); ++fileNameIter){
		const std::string & fileName = *fileNameIter;

		if(containsFile(fileName)){
			return true;
		}
	}

	return false;
}

std::string Zip::moveTheCurrentZipToAnTempZip(void)
{
	boost::filesystem::path p(zipFileName);
	std::string folderName = p.parent_path().string();
	std::string fileName = p.filename().string();

	std::string tempZipFileName(folderName + "/" + ".~" + fileName);

	try {
		boost::filesystem::rename(zipFileName, tempZipFileName);
	} catch (boost::filesystem::filesystem_error & e) {
		std::string emptyString;
		return emptyString;
	}

	return tempZipFileName;
}

bool Zip::copyAllFilesAndFoldersIntoANewZipFileExceptTheFileNames(
		const std::string & tempZipFile,
		const std::list<std::string> & fileNames,
		bool areFileNamesFolders)
{
	Unzip unzip;
	if(! unzip.open(tempZipFile)){
		return false;
	}

	open(zipFileName, CreateAndOverwrite);

	std::list<std::string> zipFileNames = unzip.getFileNames();
	std::list<std::string>::const_iterator zipFileIter;
	for(zipFileIter = zipFileNames.cbegin(); zipFileIter != zipFileNames.cend(); ++zipFileIter){
		const std::string & zipFileName = *zipFileIter;

		if(areFileNamesFolders){
			//copy all files except the folder and the files in the folder
			bool equals = false;
			std::list<std::string>::const_iterator fileNameIter;
			for(fileNameIter = fileNames.cbegin(); fileNameIter != fileNames.cend(); ++fileNameIter){
				const std::string & fileName = *fileNameIter;

				if(boost::algorithm::starts_with(zipFileName, fileName)){
					equals = true;
					break;
				}
			}
			if(equals){
				continue;
			}
		} else{
			bool equals = false;
			std::list<std::string>::const_iterator fileNameIter;
			for(fileNameIter = fileNames.cbegin(); fileNameIter != fileNames.cend(); ++fileNameIter){
				const std::string & fileName = *fileNameIter;

				if(zipFileName == fileName){
					equals = true;
					break;
				}
			}
			if(equals){
				continue;
			}
		}

		if(unzip.isFile(zipFileName)){
			if(! copyFile(unzip, zipFileName)){
				return false;
			}
		} else {
			if(! addEmptyFolder(zipFileName)){
				return false;
			}
		}
	}

	return unzip.close();
}

bool Zip::copyFile(Unzip & unzip, const std::string & fileName)
{
	bool ok;

	//locate file
	ok = unzip.goToFile(fileName);

	std::shared_ptr<InnerZipFileInfo> info = unzip.getFileInfoFromLocalFileInfos(fileName);
	zip_fileinfo zipFileInfo = convertInnerZipFileInfo_to_zipFileInfo(info);

	int raw = 1;
	int method;
	int level;

	//open the files
	ok = UNZ_OK == unzOpenCurrentFile3(unzip.zipfile_handle, &method, &level, raw, NULL);
	ok = ZIP_OK == zipOpenNewFileInZip4_64 (
			zipfile_handle,
			info->fileName.c_str(),
			&zipFileInfo,
	        NULL,
	        0,
	        NULL,
	        0,
	        info->comment.c_str(),
	        Z_DEFLATED,
	        compressionLevel,
	        0,
	        -MAX_WBITS,
	        DEF_MEM_LEVEL,
	        Z_DEFAULT_STRATEGY,
	        formatPassword(this->password),
	        0,
	        VERSIONMADEBY,
	        0,
	        0);

	//read and write the content
	unsigned char buffer[CPPZIP_ZIP_CHAR_ARRAY_BUFFER_SIZE];

	unsigned int len = 0;
	while((len = unzReadCurrentFile(
			unzip.zipfile_handle,
			buffer,
			CPPZIP_ZIP_CHAR_ARRAY_BUFFER_SIZE))
	){
		ok = ZIP_OK == zipWriteInFileInZip(zipfile_handle, buffer, len);
	}

	//close the files
	ok = UNZ_OK == unzCloseCurrentFile(unzip.zipfile_handle);
	ok = ZIP_OK == zipCloseFileInZipRaw(zipfile_handle, info->uncompressed_size, info->crc);

	return ok;
}

bool Zip::cleanUpAfterCopying(bool ok, const std::string & tempZipFile)
{
	if(! ok){
		close();

		//remove the corrupt copy
		try{
			boost::filesystem::remove(zipFileName);
		} catch(boost::filesystem::filesystem_error & e){
			//nothing todo..
		}

		try{
			boost::filesystem::rename(tempZipFile, zipFileName);
		} catch(boost::filesystem::filesystem_error & e){
			//nothing todo..
		}
	} else {
		//cleanup
		try{
			boost::filesystem::remove(tempZipFile);
		} catch(boost::filesystem::filesystem_error & e){
			//nothing todo...
		}
	}

	return true; //TODO: check the return type!!
}

void Zip::restoreTheOldOpenStatus(Zip::OpenFlags oldOpenState)
{
	if(oldOpenState == OpenExisting){
		close();
		open(zipFileName, oldOpenState);
	}
}

bool Zip::deleteFolder(const std::string& folderName)
{
	std::list<std::string> folderNames;
	folderNames.push_back(folderName);

	return deleteFolders(folderNames);
}

bool Zip::deleteFolders(const std::list<std::string> & folderNames)
{
	std::list<std::string> folderNamesToDelete;

	std::list<std::string>::const_iterator folderNameIter;
	for(folderNameIter = folderNames.cbegin(); folderNameIter != folderNames.cend(); ++folderNameIter){
		std::string folderName = *folderNameIter;

		if(! boost::algorithm::ends_with(folderName, "/")){
			folderName += "/";
		}
		folderNamesToDelete.push_back(folderName);
	}

	if(! isOpened()){
		return false;
	}

	//remember the openFlag
	enum OpenFlags oldOpenFlag = openFlag;

	//check if a file or a folder with the name of fileName exists
	if(! containsAnyFile(folderNamesToDelete)){
		return true;
	}

	//close the current zip
	close();

	//move the current zip to an tempzip
	std::string tempZipFile = moveTheCurrentZipToAnTempZip();
	if(tempZipFile.empty()){
		return false;
	}

	//Copy all files and folders into a new zip file, except the fileName
	bool ok = copyAllFilesAndFoldersIntoANewZipFileExceptTheFileNames(tempZipFile, folderNamesToDelete, true);
	cleanUpAfterCopying(ok, tempZipFile);

	//restore the old open status if necessary
	restoreTheOldOpenStatus(oldOpenFlag);

	return ok;
}

bool Zip::replaceFile(const std::string & fileName, std::vector<unsigned char> & content)
{
	bool ok = false;

	ok = deleteFile(fileName);
	if(!ok){
		return false;
	}

	ok = addFile(fileName, content);

	return ok;
}

bool Zip::replaceFile(const std::string & fileName, const std::string & destFileName)
{
	bool ok = false;

	ok = deleteFile(destFileName);
	if(!ok){
		return false;
	}

	ok = addFile(fileName, destFileName);

	return ok;

}

//bool Zip::addFilter(std::string filter)
//{
//	return false;
//}

bool Zip::close(void)
{
	if(! isOpened()){
		return true;
	}

	if(ZIP_OK == zipClose(zipfile_handle, NULL)){
		clear();
		return true;
	} else{
		return false;
	}
}

void Zip::clear(void)
{
	zipfile_handle = NULL;
	fileInfos.clear();
}

bool Zip::setCompressionLevel(int compressionLevel)
{
	if(compressionLevel < 0 || compressionLevel > 9){
		return false;
	}

	this->compressionLevel = compressionLevel;

	return true;
}

size_t Zip::getCompressionLevel(void)
{
	return compressionLevel;
}

bool Zip::createFolderIfNotExists(const std::string & path)
{
	std::string pathToCreate = path;

	if(pathToCreate.length() == 0){
		return true;
	}

	boost::filesystem::path p(pathToCreate);
	bool ok = true;
	if(! boost::filesystem::exists(p)){
		if(boost::filesystem::create_directories(p) == false){
			ok = false;
		}
	}

	return ok;
}

} //cppzip
