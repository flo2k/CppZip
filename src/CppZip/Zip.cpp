/*
 * Zip.cpp
 *
 *  Created on: 13.11.2011
 *      Author: Florian Künzner
 */

#include "Zip.h"
#include "zip.h"
#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/conversion.hpp>
#include <fstream>
#include <vector>
#include <list>
#include <exception>

#include <time.h>

#define ZIP_WRAPPER_CHAR_ARRAY_BUFFER_SIZE 1000

Zip::Zip()
	: zipfile_handle(NULL), compressionLevel(Z_DEFAULT_COMPRESSION)
{

}

Zip::~Zip()
{
	close();
}

bool Zip::open(const std::string & fileName)
{
	if(isOpened()){ //if already opened, don't open a file
		return false;
	}

	boost::filesystem::path path(fileName);
	path = path.remove_filename();

	if(! createDirectoryIfNotExists(path.string())){
		return false;
	}

	zipfile_handle = zipOpen(fileName.c_str(), APPEND_STATUS_CREATE);

	return isOpened();
}



bool Zip::isOpened(void)
{
	return zipfile_handle != NULL;
}

bool Zip::addFile(const std::string & fileName, std::vector<unsigned char> content)
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

	//file attributes
	info->internal_fileAttributes = 0x777;
	info->external_fileAttributes = 0x777;

	return addFile_internal(info, content);
}

bool Zip::addFile_internal(
		std::shared_ptr<InnerZipFileInfo> info, std::vector<unsigned char> content)
{
	if(containsFile(info->fileName) || info->fileName.length() == 0){
		return false;
	}

	zip_fileinfo zipFileInfo;

	//time stuff
	zipFileInfo.dosDate = 0;
	zipFileInfo.tmz_date.tm_year = info->time_year;
	zipFileInfo.tmz_date.tm_mon  = info->time_month;
	zipFileInfo.tmz_date.tm_mday = info->time_day_of_month;
	zipFileInfo.tmz_date.tm_hour = info->time_hour;
	zipFileInfo.tmz_date.tm_min  = info->time_min;
	zipFileInfo.tmz_date.tm_sec  = info->time_sec;

	//file attributes
	zipFileInfo.internal_fa = info->internal_fileAttributes;
	zipFileInfo.external_fa = info->external_fileAttributes;

	//open file inside zip
	if(ZIP_OK != zipOpenNewFileInZip(
			zipfile_handle,
			info->fileName.c_str(),
			&zipFileInfo,
			NULL, 0,
			NULL, 0,
			info->comment.c_str(),
			Z_DEFLATED,
			compressionLevel)){
		return false;
	}

	//write content
	if(ZIP_OK != zipWriteInFileInZip(zipfile_handle, content.data(), content.size())){
		//try to close...
		zipCloseFileInZip(zipfile_handle);
		return false;
	}

	files[info->fileName] = info;

	//close file
	if(ZIP_OK != zipCloseFileInZip(zipfile_handle)){
		return false;
	}

	return true;
}

bool Zip::containsFile(std::string & fileName)
{
	return files.count(fileName);
}

bool Zip::addFile(const std::string & fileName, bool preservePath)
{
	std::shared_ptr<Zip::InnerZipFileInfo> info;
	std::vector<unsigned char> content;

	try{
		info = getFileInfo(fileName);
		content = getFileContent(fileName);
	} catch(std::exception & e){
		return false;
	}

	return addFile_internal(info, content);
}

std::shared_ptr<Zip::InnerZipFileInfo> Zip::getFileInfo(const std::string & fileName)
{
	std::shared_ptr<InnerZipFileInfo> info(new InnerZipFileInfo);

	info->fileName = fileName;
	info->dosDate = 0;

	//now!
	boost::posix_time::ptime posixTime = boost::posix_time::second_clock::universal_time();
	std::tm time = ::boost::posix_time::to_tm(posixTime);
	info->time_year = time.tm_year;
	info->time_month = time.tm_mon;
	info->time_day_of_month = time.tm_mday;
	info->time_hour = time.tm_hour;
	info->time_min = time.tm_min;
	info->time_sec = time.tm_sec;

	//file attributes
	info->internal_fileAttributes = 0;
	info->external_fileAttributes = 0;

	return info;
}

std::vector<unsigned char> Zip::getFileContent(const std::string & fileName)
{

	boost::filesystem::path path(fileName);
	boost::filesystem::ifstream ifs(path, std::ios::in | std::ios::binary);

//	std::istream_iterator<unsigned char> begin(ifs), end;
//	std::vector<unsigned char> content(begin, end);

	std::vector<unsigned char> content;
	char buffer[ZIP_WRAPPER_CHAR_ARRAY_BUFFER_SIZE];

	if(ifs.is_open()){
		while (ifs.good()) {
			ifs.read(buffer, ZIP_WRAPPER_CHAR_ARRAY_BUFFER_SIZE);
			int len = ifs.gcount();

			if(len > 0){
				content.insert(content.end(), buffer, buffer + len);
			}
		}

		ifs.close();
	} else {
		throw std::exception();
	}

	return content;
}

bool Zip::addFile(const std::string & fileName, const std::string & destFileName)
{
	if(destFileName.length() == 0){
		return false;
	}

	std::shared_ptr<Zip::InnerZipFileInfo> info;
	std::vector<unsigned char> content;

	try{
		info = getFileInfo(destFileName);
		content = getFileContent(fileName);
	} catch(std::exception & e){
		return false;
	}

	return addFile_internal(info, content);
}

bool Zip::addFolder(const std::string & fileName, bool recursive)
{
	return false;
}

bool Zip::addEmptyFolder(const std::string & folderName)
{
	return addFolder_internal(boost::algorithm::replace_all_copy(folderName, "\\", "/"));
}

bool Zip::addFolder_internal(const std::string & folderName)
{
	std::string folderToCreate = folderName;
	if(! boost::algorithm::ends_with(folderToCreate, "/"))
	{
		folderToCreate += "/";
	}

	if(containsFile(folderToCreate)){
		return true;
	}

	std::shared_ptr<InnerZipFileInfo> info(new InnerZipFileInfo);
	info->fileName = folderToCreate;
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

	//file attributes
	info->internal_fileAttributes = 0;
	info->external_fileAttributes = 0;

	return addFile_internal(info, std::vector<unsigned char>());
}

bool Zip::addFilter(std::string filter)
{
	return false;
}



bool Zip::zip(void)
{
	return false;
}



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
	files.clear();
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

bool Zip::createDirectoryIfNotExists(const std::string & path)
{
	std::string pathToCreate = path;
//	if(isDirectory(pathToCreate)){
//		boost::algorithm::erase_tail_copy(pathToCreate, 1);
//	}

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

