/*
 * Zip.cpp
 *
 *  Created on: 13.11.2011
 *      Author: Florian Künzner
 */

#include "Zip.h"

#include "ZipStrategy.h"
#include "zlibStrategy/ZlibZipStrategy.h"

namespace cppzip {

Zip::Zip(Strategy::Strategies strategy)
    : strategy(strategy)
{
    switch (strategy) {
        case Strategy::Zlib:
            this->s = new ZlibZipStrategy();
            break;
        default:
            this->s = new ZlibZipStrategy();
            break;
    }
}

Zip::~Zip()
{
    this->s->close();
    delete this->s;
    this->s = nullptr;
}

bool Zip::open(const std::string& fileName,
               const OpenFlags::Flags& flag,
               const std::string& password)
{
    return this->s->open(fileName, flag, password);
}

bool Zip::close()
{
    return this->s->close();
}

bool Zip::isOpened()
{
    return this->s->isOpened();
}

bool Zip::addFile(const std::string& fileName, const std::vector<unsigned char>& content)
{
    return this->s->addFile(fileName, content);
}

bool Zip::addFile(const std::string& fileName, bool preservePath)
{
    return this->s->addFile(fileName, preservePath);
}

bool Zip::addFiles(const std::list<std::string>& fileNames, bool preservePath)
{
    return this->s->addFiles(fileNames, preservePath);
}

bool Zip::addFile(const std::string& fileName, const std::string& destFileName)
{
    return this->s->addFile(fileName, destFileName);
}

bool Zip::addFolder(const std::string& folderName, bool preservePath, bool recursive)
{
    return this->s->addFolder(folderName, preservePath, recursive);
}

bool Zip::addEmptyFolder(const std::string& folderName)
{
    return this->s->addEmptyFolder(folderName);
}

bool Zip::deleteFile(const std::string& fileName)
{
    return this->s->deleteFile(fileName);
}

bool Zip::deleteFiles(const std::list<std::string>& fileNames)
{
    return this->s->deleteFiles(fileNames);
}

bool Zip::deleteFolder(const std::string& folderName)
{
    return this->s->deleteFolder(folderName);
}

bool Zip::deleteFolders(const std::list<std::string>& folderNames)
{
    return this->s->deleteFolders(folderNames);
}

bool Zip::replaceFile(const std::string& fileName, std::vector<unsigned char>& content)
{
    return this->s->replaceFile(fileName, content);
}

bool Zip::replaceFile(const std::string& fileName, const std::string& destFileName)
{
    return this->s->replaceFile(fileName, destFileName);
}

bool Zip::setCompressionLevel(int compressionLevel)
{
    return this->s->setCompressionLevel(compressionLevel);
}

size_t Zip::getCompressionLevel()
{
    return this->s->getCompressionLevel();
}

} //cppzip
