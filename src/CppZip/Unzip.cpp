/*
 * Unzip.cpp
 *
 *  Created on: 05.11.2011
 *      Author: Florian Künzner
 */

#include "Unzip.h"

#include "UnzipStrategy.h"
#include "zlibStrategy/ZlibUnzipStrategy.h"

namespace cppzip {

Unzip::Unzip(Strategy::Strategies strategy)
    : strategy(strategy)
{
    switch (strategy) {
        case Strategy::Zlib:
            this->s = new ZlibUnzipStrategy();
            break;
        default:
            this->s = new ZlibUnzipStrategy();
            break;
    }
}

Unzip::~Unzip()
{
    this->s->close();
    delete this->s;
    this->s = nullptr;
}

bool Unzip::open(const std::string& zipFile, const std::string& password)
{
    return this->s->open(zipFile, password);
}

bool Unzip::close()
{
    return this->s->close();
}

bool Unzip::isOpened()
{
    return this->s->isOpened();
}

int Unzip::getNumFiles()
{
    return this->s->getNumFiles();
}

std::list<std::string> Unzip::getFileNames()
{
    return this->s->getFileNames();
}

std::vector<unsigned char> Unzip::getFileContent(const std::string& fileName)
{
    return this->s->getFileContent(fileName);
}

bool Unzip::containsFile(const std::string& fileName)
{
    return this->s->containsFile(fileName);
}

bool Unzip::isFile(const std::string& path)
{
    return this->s->isFile(path);
}

bool Unzip::isFolder(const std::string& path)
{
    return this->s->isFolder(path);
}

bool Unzip::extractFileTo(
        const std::string& fileName,
        const std::string& path,
        const bool& overwriteExistingFile)
{
    return this->s->extractFileTo(fileName, path, overwriteExistingFile);
}

bool Unzip::extractAllFilesTo(const std::string& path, const bool& overwriteExistingFile)
{
    return this->s->extractAllFilesTo(path, overwriteExistingFile);
}

} //cppzip
