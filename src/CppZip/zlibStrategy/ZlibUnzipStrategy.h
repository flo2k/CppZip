/*!
 * \file Unzip.h
 *
 * Unzip.h -- IO on .zip files using minizip and zlib
 * Version 1.0.0.2, 22.01.2017
 * part of the CppZip project - (https://github.com/flo2k/CppZip)
 *
 * Created: 05.11.2011, Florian Künzner
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

#ifndef CPPZIP_ZLIBUNZIPSTRATEGY_H_
#define CPPZIP_ZLIBUNZIPSTRATEGY_H_

#include "../CppZip_Global.h"
#include "../ZipDefines.h"
#include "../UnzipStrategy.h"

#include <string>
#include <list>
#include <unordered_map>
#include <vector>
#include <memory>
#include <boost/signals2.hpp>

namespace cppzip {
//forward declaration
struct InnerZipFileInfo;
class UnzipPrivate;

/*!
 * \brief Unzip allows reading files inside a zip file
 *
 * Unzip is a c++ wrapper for unzip.h (minizip). It provides a c++ interface for
 * working with existing zip files.
 *
 * \author Florian Künzner
 */
class ZlibUnzipStrategy : public UnzipStrategy {
public:
    ZlibUnzipStrategy();
    virtual ~ZlibUnzipStrategy();

    virtual bool open(const std::string& zipFile, const std::string& password = "") override;
    virtual bool close() override;
    virtual bool isOpened() override;

    virtual int getNumFiles() override;
    virtual bool containsFile(const std::string& fileName) override;
    virtual bool isFile(const std::string& path) override;
    virtual bool isFolder(const std::string& path) override;

    virtual std::list<std::string> getFileNames() override;

    virtual std::vector<unsigned char> getFileContent(const std::string& fileName) override;

    virtual bool extractFileTo(const std::string& fileName,
                               const std::string& path,
                               const bool& overwriteExistingFile = true) override;
    virtual bool extractAllFilesTo(const std::string& path,
                                   const bool & overwriteExistingFile = true) override;

private:
    /*!
     * Clears the internal members.
     */
    void clear();

    /*!
     * Get's the global info of the zip file
     * - sets the numFiles
     */
    void getGlobalInfo();

    /*!
     * Makes the file given in fileName to the current file.
     *
     * \param fileName to make to current file
     * \return true if operation was ok, otherwise false.
     */
    bool goToFile(const std::string& fileName);

    /*!
     * Reads all elements in the zip file
     */
    void retrieveAllFileInfos();

    /*!
     * Creates a folder with all subdirs if not exists
     *
     * \param path to create
     * \return true if path exists or created, otherwise false.
     */
    bool createFolderIfNotExists(const std::string& path);

    /*!
     * Extracts the file to the given path. Bevor a file is extracted
     * the signal beforeFileExtraction is emitted and the user has the
     * posibility to change the path.
     *
     * After the extraction is successfully done the fileExtracted signal
     * is emitted. When extraction fails, the signal will NOT be emitted.
     *
     * \param fileName file that should be extracted
     * \param path is the destination
     * \param max describes how much files to extract
     * \param current the current file that is extracted (begins with 1)
     *
     * \return true if extraction was successful, otherwise false.
     */
    bool extractFileTo_Internal(
            const std::string& fileName,
            const std::string& path,
            const bool & overwriteExistingFile);

    std::shared_ptr<InnerZipFileInfo> getFileInfoFromLocalFileInfos(const std::string& fileName);

    bool doesFileExistOnFileSystem(const std::string& fileName);

private:
    UnzipPrivate* p;

    friend class ZlibZipStrategy;
};

} //cppzip

#endif /* CPPZIP_ZLIBUNZIPSTRATEGY_H_ */
