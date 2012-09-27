/*
 * Unzip.cpp
 *
 *  Created on: 05.11.2011
 *      Author: Florian Künzner
 */

#include "Unzip.h"
#include <unzip.h>
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>

#define CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE 1000

namespace cppzip {

Unzip::Unzip(void)
: zipfile_handle(NULL), numFiles(0)
{
	// TODO Auto-generated constructor stub

}

Unzip::~Unzip()
{
	close();
}

bool Unzip::open(const std::string & zipFile)
{
	if(isOpened()){ //if already opened, don't open a file
		return false;
	}

	zipfile_handle = unzOpen(zipFile.c_str());

	if(isOpened()){
		getGlobalInfo();
		retrieveAllFileInfos();
	}

	return isOpened();
}

bool Unzip::close(void)
{
	if(! isOpened()){
		return true;
	}

	if(UNZ_OK == unzClose(zipfile_handle)){
		clear();
		return true;
	} else{
		return false;
	}
}

void Unzip::clear(void)
{
	numFiles = 0;
	zipfile_handle = NULL;
	fileInfos.clear();
}

bool Unzip::isOpened(void)
{
	return zipfile_handle != NULL;
}

int Unzip::getNumFiles(void)
{
	return numFiles;
}

void Unzip::getGlobalInfo(void)
{
	unz_global_info info;
	unzGetGlobalInfo(zipfile_handle, &info);

	numFiles = info.number_entry;
}

std::list<std::string> Unzip::getFileNames(void)
{
	std::list<std::string> fileNames;

	for(auto iter = fileInfos.begin(); iter != fileInfos.end(); ++iter){
		std::string fileName = iter->first;
		fileNames.push_back(fileName);
	}

	return fileNames;
}

std::vector<unsigned char> Unzip::getFileContent(const std::string & fileName)
{
	std::vector<unsigned char> fileContent;

	if(fileInfos.count(fileName) == 0){
		return fileContent;
	}

	//locate file
	if(! goToFile(fileName)){
		return fileContent;
	}

	//open file
	if(UNZ_OK != unzOpenCurrentFile(zipfile_handle)){
		return fileContent;
	}

	//read content
	unsigned char buffer[CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE];
//	for(int i = 0; i < UNZIP_WRAPPER_CHAR_ARRAY_BUFFER_SIZE; ++i){
//		buffer[i] = 9;
//	}
	unsigned int len = 0;
	while((len = unzReadCurrentFile(
			zipfile_handle,
			buffer,
			CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE))
	){
		for(unsigned int i = 0; i < len; ++i){
			fileContent.push_back(buffer[i]);
		}
	}

	//close file
	if(UNZ_OK != unzCloseCurrentFile(zipfile_handle)){
		return fileContent;
	}

	return fileContent;
}

bool Unzip::goToFile(const std::string & fileName)
{

	if(containsFile(fileName) == false){
		return false;
	}

	if(unzLocateFile(zipfile_handle, fileName.c_str(), 0) == UNZ_OK){
		return true;
	} else {
		return false;
	}
}

bool Unzip::containsFile(const std::string & fileName)
{
	if(fileInfos.count(fileName) == 1){
		return true;
	} else {
		return false;
	}
}

bool Unzip::extractFileTo(const std::string & fileName, const std::string & path)
{
	return extractFileTo_Internal(fileName, path, 1, 1);
}

bool Unzip::extractFileTo_Internal(
		const std::string & fileName,
		const std::string & path,
		int max,
		int current)
{
	if(! containsFile(fileName)){
		return false;
	}

	bool extraction_ok = true;

	try{
		std::string destinationPath = path;
		beforeFileExtraction(destinationPath);

		boost::filesystem::path p(destinationPath);
		if(createDirectoryIfNotExists(p.parent_path().string()) == false){
			extraction_ok = false;
		}

		std::vector<unsigned char> data = getFileContent(fileName);

		boost::filesystem::ofstream ofs(p, std::ios::out | std::ios::binary);
		std::copy(data.begin(), data.end(), std::ostream_iterator<unsigned char>(ofs));

		ofs.flush();

		if(ofs.fail()){
			extraction_ok = false;
		}
		ofs.close();

		fileExtracted(destinationPath, max, current);
	} catch(...){
		return false;
	}

	return extraction_ok;
}

bool Unzip::extractAllTo(const std::string & path)
{
	std::string dest_path = path;
	bool extraction_ok = true;

	if(! isDirectory(dest_path)){
		dest_path += "/";
	}

	int current = 1;
	for(auto iter = fileInfos.begin(); iter != fileInfos.end(); ++iter){
		std::string fileName = iter->first;

		if(isFile(fileName)){
			bool ok = extractFileTo_Internal(fileName, dest_path + fileName, fileInfos.size(), current++);
			if(!ok){
				extraction_ok = false;
			}
		} else {
			if(createDirectoryIfNotExists(dest_path + fileName) == false){
				extraction_ok = false;
			}
		}
	}

	return extraction_ok;
}

bool Unzip::isFile(const std::string & path)
{
	return isDirectory(path) == false;
}

bool Unzip::isDirectory(const std::string & path)
{
	return boost::algorithm::ends_with(path, "/");
}

bool Unzip::createDirectoryIfNotExists(const std::string & path)
{
	std::string pathToCreate = path;
	if(isDirectory(pathToCreate)){
		boost::algorithm::erase_tail_copy(pathToCreate, 1);
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


void Unzip::retrieveAllFileInfos(void)
{
	do{
		unz_file_pos pos;
		if(UNZ_OK != unzGetFilePos(zipfile_handle, &pos)){
			continue;
		}

		unz_file_info info;
		char currentFileName[CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE];
		char currentExtraField[CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE];
		char currentComment[CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE];

		unzGetCurrentFileInfo(zipfile_handle, &info,
				currentFileName, CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE,
				currentExtraField, CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE,
				currentComment, CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE);

		InnerZipFileInfo innerFileInfo;
		innerFileInfo.fileName = currentFileName;
		innerFileInfo.extraField = currentExtraField;
		innerFileInfo.comment = currentComment;
		innerFileInfo.pos_in_zip_directory = pos.pos_in_zip_directory;
		innerFileInfo.num_of_file = pos.num_of_file;

		fileInfos.insert(std::make_pair(innerFileInfo.fileName, innerFileInfo));


	} while(UNZ_OK == unzGoToNextFile(zipfile_handle));
}

} //cppzip
