/*!
 * \file Zip.h
 *
 * Zip.h -- IO on .zip files using minizip and zlib
 * Version 1.0.0.2, 22.01.2017
 * part of the CppZip project - (https://github.com/flo2k/CppZip)
 *
 * Created: 13.11.2011, Florian Künzner
 *
 * Copyright (C) 2011      Florian Künzner (CppZip)
 * Copyright (C) 2012-2013 Florian Künzner and Andreas Bauer (CppZip) (https://github.com/flo2k/CppZip)
 * Copyright (C) 2014-2017 Florian Künzner (CppZip) (https://github.com/flo2k/CppZip)
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

#ifndef CPPZIP_ZIPSTRATEGY_H_
#define CPPZIP_ZIPSTRATEGY_H_

#include "CppZip_Global.h"
#include "ZipDefines.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <boost/signals2.hpp>

/*!
 * \brief Contains all classes to zip and unzip files.
 */
namespace cppzip {

/*!
 * \brief ZipStrategy is a abstract interface for zip strategies.
 *
 * \author Florian Künzner
 */
class ZipStrategy
{
public:
    ZipStrategy();
    virtual ~ZipStrategy();

    virtual bool open(const std::string& fileName,
                      const OpenFlags::Flags& flag = OpenFlags::CreateAndOverwrite,
                      const std::string& password = "") = 0;

    virtual bool close() = 0;
    virtual bool isOpened() = 0;

    virtual bool addFile(const std::string& fileName,
                         const std::vector<unsigned char>& content) = 0;
    virtual bool addFile(const std::string& fileName, bool preservePath = true) = 0;
    virtual bool addFiles(const std::list<std::string>& fileNames, bool preservePath = true) = 0;
    virtual bool addFile(const std::string& fileName, const std::string& destFileName) = 0;
    virtual bool addFolder(const std::string& folderName,
                           bool preservePath = true,
                           bool recursive = true) = 0;
    virtual bool addEmptyFolder(const std::string& folderName) = 0;

    virtual bool deleteFile(const std::string& fileName) = 0;
    virtual bool deleteFiles(const std::list<std::string>& fileNames) = 0;
    virtual bool deleteFolder(const std::string& folderName) = 0;
    virtual bool deleteFolders(const std::list<std::string>& folderNames) = 0;

    virtual bool replaceFile(const std::string& fileName, std::vector<unsigned char>& content) = 0;
    virtual bool replaceFile(const std::string& fileName, const std::string& destFileName) = 0;

    virtual bool setCompressionLevel(int compressionLevel) = 0;
    virtual size_t getCompressionLevel() = 0;
};

} //cppzip

#endif /* CPPZIP_ZIPSTRATEGY_H_ */
