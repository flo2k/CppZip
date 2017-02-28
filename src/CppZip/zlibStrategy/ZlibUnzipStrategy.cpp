/*
 * Unzip.cpp
 *
 *  Created on: 05.11.2011
 *      Author: Florian Künzner
 */

#include "ZlibUnzipStrategy.h"

#include "ZipCommon.h"
#include "ZipPrivate.h"

#include "minizip/unzip.h"

#include <algorithm>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>

#define CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE 1000

namespace cppzip {

ZlibUnzipStrategy::ZlibUnzipStrategy()
    : p(new UnzipPrivate())
{

}

ZlibUnzipStrategy::~ZlibUnzipStrategy()
{
    close();
    delete p;
    p = nullptr;
}

bool ZlibUnzipStrategy::open(const std::string& zipFile, const std::string& password)
{
    if(isOpened()){ //if already opened, don't open a file
        return false;
    }

    this->p->zipfile_handle = unzOpen(zipFile.c_str());
    this->p->password = password;

    if(isOpened()){
        getGlobalInfo();
        retrieveAllFileInfos();
    }

    return isOpened();
}

bool ZlibUnzipStrategy::close()
{
    if(! isOpened()){
        return true;
    }

    if(UNZ_OK == unzClose(this->p->zipfile_handle)){
        clear();
        return true;
    } else{
        return false;
    }
}

void ZlibUnzipStrategy::clear()
{
    this->p->numFiles = 0;
    this->p->zipfile_handle = nullptr;
    this->p->fileInfos.clear();
}

bool ZlibUnzipStrategy::isOpened()
{
    return this->p->zipfile_handle != nullptr;
}

int ZlibUnzipStrategy::getNumFiles()
{
    return this->p->numFiles;
}

void ZlibUnzipStrategy::getGlobalInfo()
{
    unz_global_info info;
    unzGetGlobalInfo(this->p->zipfile_handle, &info);

    this->p->numFiles = info.number_entry;
}

std::list<std::string> ZlibUnzipStrategy::getFileNames()
{
    std::list<std::string> fileNames;

    std::unordered_map<std::string, std::shared_ptr<InnerZipFileInfo> >::const_iterator iter;
    for(iter = this->p->fileInfos.cbegin(); iter != this->p->fileInfos.cend(); ++iter){
        const std::string & fileName = iter->first;
        fileNames.push_back(fileName);
    }

    return fileNames;
}

std::vector<unsigned char> ZlibUnzipStrategy::getFileContent(const std::string& fileName)
{
    std::vector<unsigned char> fileContent;

    if(this->p->fileInfos.count(fileName) == 0){
        return fileContent;
    }

    //locate file
    if(! goToFile(fileName)){
        return fileContent;
    }

    //open file
    if(UNZ_OK != unzOpenCurrentFile3(this->p->zipfile_handle,
                                     nullptr, nullptr, 0,
                                     formatPassword(this->p->password)))
    {
        return fileContent;
    }

    //read content
    unsigned char buffer[CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE];

    unsigned int len = 0;
    while((len = unzReadCurrentFile(
               this->p->zipfile_handle,
               buffer,
               CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE))
          )
    {
        for(unsigned int i = 0; i < len; ++i){
            fileContent.push_back(buffer[i]);
        }
    }

    //close file
    if(UNZ_OK != unzCloseCurrentFile(this->p->zipfile_handle)){
        return fileContent;
    }

    return fileContent;
}

bool ZlibUnzipStrategy::goToFile(const std::string& fileName)
{
    if(containsFile(fileName) == false){
        return false;
    }

    if(unzLocateFile(this->p->zipfile_handle, fileName.c_str(), 0) == UNZ_OK){
        return true;
    } else {
        return false;
    }
}

bool ZlibUnzipStrategy::containsFile(const std::string& fileName)
{
    if(this->p->fileInfos.count(fileName) == 1){
        return true;
    } else {
        return false;
    }
}

bool ZlibUnzipStrategy::isFile(const std::string& path)
{
    return isFolder(path) == false;
}

bool ZlibUnzipStrategy::isFolder(const std::string& path)
{
    return boost::algorithm::ends_with(path, "/");
}

bool ZlibUnzipStrategy::extractFileTo(
        const std::string& fileName,
        const std::string& path,
        const bool& overwriteExistingFile)
{
    return extractFileTo_Internal(fileName, path, overwriteExistingFile);
}

bool ZlibUnzipStrategy::extractFileTo_Internal(
        const std::string& fileName,
        const std::string& path,
        const bool& overwriteExistingFile)
{
    if(! containsFile(fileName)){
        return false;
    }

    if(!overwriteExistingFile && doesFileExistOnFileSystem(path)){
        return false;
    }

    bool extraction_ok = true;

    try{
        std::string destinationPath = path;
        //beforeFileExtraction(destinationPath);

        boost::filesystem::path p(destinationPath);
        if(createFolderIfNotExists(p.parent_path().string()) == false){
            extraction_ok = false;
        }

        //locate filefileContent
        if(! goToFile(fileName)){
            return false;
        }

        //open file
        if(UNZ_OK != unzOpenCurrentFile3(this->p->zipfile_handle,
                                         NULL, NULL, 0,
                                         formatPassword(this->p->password)))
        {
            return false;
        }

        //destination
        boost::filesystem::ofstream ofs(p, std::ios::out | std::ios::binary);

        //copy the content
        unsigned char buffer[CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE];

        unsigned int len = 0;
        while((len = unzReadCurrentFile(
                   this->p->zipfile_handle,
                   buffer,
                   CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE))
              )
        {
            ofs.write((const char *)buffer, len);
        }

        //close file
        if(UNZ_OK != unzCloseCurrentFile(this->p->zipfile_handle)){
            return false;
        }

        ofs.flush();

        if(ofs.fail()){
            extraction_ok = false;
        }
        ofs.close();

        //fileExtracted(destinationPath, max, current);
    } catch(...){
        return false;
    }

    return extraction_ok;
}

bool ZlibUnzipStrategy::doesFileExistOnFileSystem(const std::string& fileName) {

    return boost::filesystem::exists(fileName);
}

bool ZlibUnzipStrategy::extractAllFilesTo(const std::string& path, const bool& overwriteExistingFile)
{
    std::string dest_path = path;
    bool extraction_ok = true;

    if(! isFolder(dest_path)){
        dest_path += "/";
    }

    std::unordered_map<std::string, std::shared_ptr<InnerZipFileInfo> >::const_iterator iter;
    for(iter = this->p->fileInfos.cbegin(); iter != this->p->fileInfos.cend(); ++iter){
        const std::string & fileName = iter->first;

        if(isFile(fileName)){
            bool ok = extractFileTo_Internal(fileName,
                                             dest_path + fileName,
                                             overwriteExistingFile);
            if(!ok){
                extraction_ok = false;
            }
        } else {
            if(createFolderIfNotExists(dest_path + fileName) == false){
                extraction_ok = false;
            }
        }
    }

    return extraction_ok;
}

bool ZlibUnzipStrategy::createFolderIfNotExists(const std::string& path)
{
    std::string pathToCreate = path;
    if(isFolder(pathToCreate)){
        boost::algorithm::erase_tail(pathToCreate, 1);
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

void ZlibUnzipStrategy::retrieveAllFileInfos()
{
    do{
        unz_file_pos pos;
        if(UNZ_OK != unzGetFilePos(this->p->zipfile_handle, &pos)){
            continue;
        }

        unz_file_info info;
        char currentFileName[CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE];
        char currentExtraField[CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE];
        char currentComment[CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE];

        unzGetCurrentFileInfo(this->p->zipfile_handle, &info,
                              currentFileName, CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE,
                              currentExtraField, CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE,
                              currentComment, CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE);

        std::shared_ptr<InnerZipFileInfo> innerFileInfo(new InnerZipFileInfo());
        innerFileInfo->fileName = currentFileName;
        innerFileInfo->extraField = currentExtraField;
        innerFileInfo->comment = currentComment;
        innerFileInfo->time_sec = info.tmu_date.tm_sec;
        innerFileInfo->time_min = info.tmu_date.tm_min;
        innerFileInfo->time_hour = info.tmu_date.tm_hour;
        innerFileInfo->time_day_of_month = info.tmu_date.tm_mday;
        innerFileInfo->time_month = info.tmu_date.tm_mon;
        innerFileInfo->time_year = info.tmu_date.tm_year;
        innerFileInfo->dosDate = info.dosDate;
        innerFileInfo->crc = info.crc;
        innerFileInfo->compressed_size = info.compressed_size;
        innerFileInfo->uncompressed_size = info.uncompressed_size;
        innerFileInfo->internal_fileAttributes = info.internal_fa;
        innerFileInfo->external_fileAttributes = info.external_fa;

        this->p->fileInfos.insert(std::make_pair(innerFileInfo->fileName, innerFileInfo));


    } while(UNZ_OK == unzGoToNextFile(this->p->zipfile_handle));
}

std::shared_ptr<InnerZipFileInfo> ZlibUnzipStrategy::getFileInfoFromLocalFileInfos(
        const std::string& fileName)
{
    return this->p->fileInfos[fileName];
}

} //cppzip
