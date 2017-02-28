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

#ifndef CPPZIP_ZLIBZIPSTRATEGY_H_
#define CPPZIP_ZLIBZIPSTRATEGY_H_

#include "../CppZip_Global.h"
#include "../ZipDefines.h"
#include "../ZipStrategy.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <boost/signals2.hpp>

/*!
 * \brief Contains all classes to zip and unzip files.
 */
namespace cppzip {
//forward declaration
struct InnerZipFileInfo;
class Unzip;
class ZipPrivate;
class ZlibUnzipStrategy;

/*!
 * \brief Zip allows creating zip files
 *
 * Zip is a c++ wrapper for zip.h (minizip). It provides a c++ interface for
 * working with zip files.
 */
class ZlibZipStrategy : public ZipStrategy
{
public:
    ZlibZipStrategy();
    virtual ~ZlibZipStrategy();

    virtual bool open(const std::string& fileName,
                      const OpenFlags::Flags& flag = OpenFlags::CreateAndOverwrite,
                      const std::string& password = "") override;

    virtual bool close() override;
    virtual bool isOpened() override;

    virtual bool addFile(const std::string& fileName, const std::vector<unsigned char>& content) override;
    virtual bool addFile(const std::string& fileName, bool preservePath = true) override;
    virtual bool addFiles(const std::list<std::string>& fileNames, bool preservePath = true) override;
    virtual bool addFile(const std::string& fileName, const std::string& destFileName) override;
    virtual bool addFolder(const std::string& folderName, bool preservePath = true, bool recursive = true) override;
    virtual bool addEmptyFolder(const std::string& folderName) override;

    virtual bool deleteFile(const std::string& fileName) override;
    virtual bool deleteFiles(const std::list<std::string>& fileNames) override;
    virtual bool deleteFolder(const std::string& folderName) override;
    virtual bool deleteFolders(const std::list<std::string>& folderNames) override;

    virtual bool replaceFile(const std::string& fileName, std::vector<unsigned char>& content) override;
    virtual bool replaceFile(const std::string& fileName, const std::string& destFileName) override;

    virtual bool setCompressionLevel(int compressionLevel) override;
    virtual size_t getCompressionLevel() override;

public:
    /*!
     * \brief   This signal is emitted, before a file will be zipped.
     *
     * \details The parameter destination is the path (incl. file name) of the files
     *          destination inside the zip. The value can be changed to a new one.
     *
     * \param   destination is the destination inside the zip file.
     */
    //boost::signals2::signal<void (std::string& destination)> beforeFileZipping;

    /*!
     * \brief   This signal is emitted, after a file is zipped.
     *
     * \details The parameter the path (incl. file name) of the files
     *          destination. The value can be changed to a new one.
     *
     * \param   destination is the destination inside the zip file.
     * \param   maxFiles    is the maximum number of files to extract.
     * \param   currentFile is is the current file that is extracted (counting begins with 1).
     */
    //boost::signals2::signal<void (const std::string destination,
    //		            const unsigned int& maxFiles,
    //		            const unsigned int& currentFile)> fileZipped;

private:
    /*!
     * Retrieves the file infos of an existing zip file.
     *
     * If the file doesn't exist or an error occurs, an empty map will be returned;
     *
     * \return the zip file infos.
     */
    std::unordered_map<std::string, std::shared_ptr<InnerZipFileInfo>>
    retrieveFileInfos(const std::string& fileName);

    /*!
     * Clears the internal members.
     */
    void clear();

    // Helpers for adding file/folder
    /*!
     * Creates a folder on the file system with all subfolders if not exists
     *
     * \param path to create
     * \return true if path exists or created, otherwise false.
     */
    bool createFolderIfNotExists(const std::string& path);

    std::shared_ptr<InnerZipFileInfo> getFileInfoForANewFile(const std::string& fileName);
    std::shared_ptr<InnerZipFileInfo> getFileInfoForAExistingFile(const std::string& fileName);
    std::shared_ptr<InnerZipFileInfo> getFileInfoFromLocalFileInfos(const std::string& fileName);

    /*!
     * Gets the platform depended file attributes from an exiting file on the file system.
     *
     * \return The file attributes.
     */
    unsigned long getExternalFileAttributesFromExistingFile(const std::string& fileName);

    bool addFile_internal(
            std::shared_ptr<InnerZipFileInfo> info,
            const std::vector<unsigned char>& content);

    bool addFile_internal(
            std::shared_ptr<InnerZipFileInfo> info,
            const std::string& fileName);

    bool containsFile(const std::string& fileName);
    bool containsAnyFile(const std::list<std::string>& fileNames);

    bool addFolder(
            const std::string& realFolderName, const std::string& relativeFolderName,
            bool preservePath = true, bool recursive = true);
    bool addFolderChilds(
            const std::string& realFolderName, const std::string& folderNameToAdd,
            bool preservePath = true, bool recursive = true);
    bool addFolder_internal(std::shared_ptr<InnerZipFileInfo> info);

    // Helpers for deleting file/folder
    /*!
     * Moves the current zip to an temp zip.
     *
     * \return the name of the file name if successfully moved, otherwise an empty string.
     */
    std::string moveTheCurrentZipToAnTempZip();

    /*!
     * Copies all files and folders into a new zip, except the files in fileNames.
     *
     * \return true if all is ok, otherwise false.
     */
    bool copyAllFilesAndFoldersIntoANewZipFileExceptTheFileNames(const std::string & tempZipFile,
                                                                 const std::list<std::string>& fileNames,
                                                                 bool areFileNamesFolders);

    bool copyFile(ZlibUnzipStrategy& unzip, const std::string& fileName);

    /*!
     * Cleans up the temporary files and tries to restore the original zip file.
     *
     * \return true if the cleanup was successful, otherwise false.
     */
    bool cleanUpAfterCopying(bool ok, const std::string& tempZipFile);

    /*!
     * Restores the old open status, with the zip file was opened.
     */
    void restoreTheOldOpenStatus(OpenFlags::Flags oldOpenState);

private:
    ZipPrivate* p;
};

} //cppzip

#endif /* CPPZIP_ZLIBZIPSTRATEGY_H_ */
