/*!
 * \file Unzip.h
 *
 * Unzip.h -- IO on .zip files using minizip and zlib
 * Version 1.0.0.2, 22.01.2017
 * part of the CppZip project - (https://github.com/flo2k/CppZip)
 *
 * Created: 28.02.2017, Florian Künzner
 *
 * Copyright (C) 2017 Florian Künzner (CppZip) (https://github.com/flo2k/CppZip)
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

#ifndef CPPZIP_UNZIPSTRATEGY_H_
#define CPPZIP_UNZIPSTRATEGY_H_

#include "CppZip_Global.h"
#include "ZipDefines.h"

#include <string>
#include <list>
#include <vector>

namespace cppzip {

/*!
 * \brief UnzipStrategy is a abstract interface for unzip strategies.
 *
 * \author Florian Künzner
 */
class UnzipStrategy {
public:
    UnzipStrategy();
    virtual ~UnzipStrategy();

    virtual bool open(const std::string& zipFile, const std::string& password = "") = 0;
    virtual bool close() = 0;
    virtual bool isOpened() = 0;

    virtual int getNumFiles() = 0;
    virtual bool containsFile(const std::string& fileName) = 0;
    virtual bool isFile(const std::string& path) = 0;
    virtual bool isFolder(const std::string& path) = 0;

    virtual std::list<std::string> getFileNames() = 0;

    virtual std::vector<unsigned char> getFileContent(const std::string& fileName) = 0;

    virtual bool extractFileTo(const std::string& fileName,
                               const std::string& path,
                               const bool& overwriteExistingFile = true) = 0;
    virtual bool extractAllFilesTo(const std::string& path,
                                   const bool& overwriteExistingFile = true) = 0;
};

} //cppzip

#endif /* CPPZIP_UNZIPSTRATEGY_H_ */
