/*
 * ZipPrivate.h -- IO on .zip files using minizip and zlib
 * Version 1.0.0.2, 22.01.2017
 * part of the CppZip project - (https://github.com/flo2k/CppZip)
 *
 * Created: 12.12.2012, Florian Künzner
 *
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

#ifndef CPPZIP_ZIPPRIVATE_H_
#define CPPZIP_ZIPPRIVATE_H_

#include <memory>
#include <unordered_map>

#include "../ZipDefines.h"
#include "ZipCommon.h"
#include "minizip/zip.h"

namespace cppzip{

/*!
     * \brief  Converts a InnerZipFileInfo to a zip_fileinfo.
     *
     * \param  innerZipFileInfo is the element to convert.
     *
     * \return the zip_fileinfo.
     */
zip_fileinfo convertInnerZipFileInfo_to_zipFileInfo(
        std::shared_ptr<InnerZipFileInfo> innerZipFileInfo);

/*!
     * \brief  Converts a zip_fileinfo to a zip_fileinfo.
     *
     * \param  zipInfo is the element to convert.
     *
     * \return the InnerZipFileInfo.
     */
std::shared_ptr<InnerZipFileInfo> convertInnerZipFileInfo_to_zipFileInfo(
        zip_fileinfo zipInfo);

/*!
     * \brief Formats the password for the use with the minizip api.
     *
     * \param password is the password that shall be formatted.
     */
const char* formatPassword(const std::string& password);

class UnzipPrivate
{
public:
    UnzipPrivate()
        : zipfile_handle(nullptr)
        , numFiles(0)
    {}

    typedef void * voidp;
    typedef voidp unzFile;

    unzFile zipfile_handle;
    int numFiles;

    typedef std::pair<std::string, InnerZipFileInfo> FileInfoPair;
    std::unordered_map<std::string, std::shared_ptr<InnerZipFileInfo> > fileInfos;

    std::string password;
};

class ZipPrivate
{
public:
    ZipPrivate()
        : zipfile_handle(nullptr)
        , openFlag(OpenFlags::CreateAndOverwrite)
        , compressionLevel(Z_DEFAULT_COMPRESSION)
    {}

    typedef void * voidp;
    typedef voidp zipFile;
    std::string zipFileName;
    std::unordered_map<std::string, std::shared_ptr<InnerZipFileInfo> > fileInfos;

    zipFile zipfile_handle;
    OpenFlags::Flags openFlag;
    int compressionLevel;
    std::string password;
};

} //cppzip


#endif /* CPPZIP_ZIPPRIVATE_H_ */
