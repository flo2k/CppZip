/*!
 * UnzipTest.h -- Tests Unzip.h from CppZip
 * Version 0.2, 22.09.2012
 * part of the CppZip project - (https://github.com/flo2k/CppZip)
 *
 * Created: 05.11.2011, Florian Künzner
 *
 * Copyright (C) 2011 Florian Künzner (CppZip)
 * Copyright (C) 2012 Florian Künzner and Andreas Bauer (CppZip) (https://github.com/flo2k/CppZip)
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

#ifndef CPPZIP_UNZIPTEST_H_
#define CPPZIP_UNZIPTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <memory>
#include <string>

namespace cppzip {

//forward declaration
class Unzip;

class UnzipTest : public CppUnit::TestFixture {

	CPPUNIT_TEST_SUITE(UnzipTest);
		CPPUNIT_TEST(test_isOpenedWithNonOpenedZipFile);

		CPPUNIT_TEST(test_openExistingZipFile);
		CPPUNIT_TEST(test_openZipFileWhileAnotherIsAlreadyOpened);
		CPPUNIT_TEST(test_openANonExistingZipFile);

		CPPUNIT_TEST(test_closeAZipFile);
		CPPUNIT_TEST(test_closeWhenNoZipFileIsOpened);

		CPPUNIT_TEST(test_getNumFiles);
		CPPUNIT_TEST(test_getNumFilesWithNoZipFileIsOpened);
		CPPUNIT_TEST(test_getNumFilesAfterCloseZipFiled);

		CPPUNIT_TEST(test_containsFile);
		CPPUNIT_TEST(test_containsFileWithANonExistingFile);
		CPPUNIT_TEST(test_containsFileWithNoZipFileIsOpened);
		CPPUNIT_TEST(test_containsFileAfterCloseZipFiled);

		CPPUNIT_TEST(test_getFileNames);
		CPPUNIT_TEST(test_getFileNamesWithNoZipFileIsOpened);
		CPPUNIT_TEST(test_getFileNamesAfterCloseZipFiled);

		CPPUNIT_TEST(test_getFileContent);
		CPPUNIT_TEST(test_getFileContentWithNoZipFileIsOpened);
		CPPUNIT_TEST(test_getFileContentAfterCloseZipFiled);
		CPPUNIT_TEST(test_getFileContentFromANonExistingFile);
		CPPUNIT_TEST(test_getFileContentFromPasswordProtectedZipFile_linux32bit);
		CPPUNIT_TEST(test_getFileContentFromPasswordProtectedZipFile_linux64bit);
		CPPUNIT_TEST(test_getFileContentFromPasswordProtectedZipFile_win32bit);
		CPPUNIT_TEST(test_getFileContentFromPasswordProtectedZipFile_win64bit_7zip);
		CPPUNIT_TEST(test_getFileContentFromPasswordProtectedZipFile_win64bit_winrar);

		CPPUNIT_TEST(test_extractFile);
		CPPUNIT_TEST(test_extractFile_AJpg);
		CPPUNIT_TEST(test_extractFile_WithOverwriteAExistingFile);
		CPPUNIT_TEST(test_extractFile_WithNotOverwriteAExistingFile);

		CPPUNIT_TEST(test_extractAllFilesTo);
		CPPUNIT_TEST(test_extractAllFilesToWithUmlaut);
		CPPUNIT_TEST(test_extractAllFiles_WithOverwriteAExistingFile);
		CPPUNIT_TEST(test_extractAllFiles_WithNotOverwriteAExistingFile);

		CPPUNIT_TEST(test_signalFileExtracted);
		CPPUNIT_TEST(test_signalBeforeFileExtracted);
		CPPUNIT_TEST(test_signalBeforeFileExtractedWithChangingFileName);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

public:
	void test_isOpenedWithNonOpenedZipFile(void);

	void test_openExistingZipFile(void);
	void test_openZipFileWhileAnotherIsAlreadyOpened(void);
	void test_openANonExistingZipFile(void);

	void test_closeAZipFile(void);
	void test_closeWhenNoZipFileIsOpened(void);

	void test_getNumFiles(void);
	void test_getNumFilesWithNoZipFileIsOpened(void);
	void test_getNumFilesAfterCloseZipFiled(void);

	void test_containsFile(void);
	void test_containsFileWithANonExistingFile(void);
	void test_containsFileWithNoZipFileIsOpened(void);
	void test_containsFileAfterCloseZipFiled(void);

	void test_getFileNames(void);
	void test_getFileNamesWithNoZipFileIsOpened(void);
	void test_getFileNamesAfterCloseZipFiled(void);

	void test_getFileContent(void);
	void test_getFileContentWithNoZipFileIsOpened(void);
	void test_getFileContentAfterCloseZipFiled(void);
	void test_getFileContentFromANonExistingFile(void);
	void test_getFileContentFromPasswordProtectedZipFile_linux32bit(void);
	void test_getFileContentFromPasswordProtectedZipFile_linux64bit(void);
	void test_getFileContentFromPasswordProtectedZipFile_win32bit(void);
	void test_getFileContentFromPasswordProtectedZipFile_win64bit_7zip(void);
	void test_getFileContentFromPasswordProtectedZipFile_win64bit_winrar(void);

	void test_extractFile(void); //TODO: test exceptions: file not writeable, and so on...
	void test_extractFile_AJpg(void);
	void test_extractFile_WithOverwriteAExistingFile(void);
	void test_extractFile_WithNotOverwriteAExistingFile(void);

	void test_extractAllFilesTo(void);
	void test_extractAllFilesToWithUmlaut(void);
	void test_extractAllFiles_WithOverwriteAExistingFile(void);
	void test_extractAllFiles_WithNotOverwriteAExistingFile(void);

	void test_signalFileExtracted(void);
	void test_signalBeforeFileExtracted(void);
	void test_signalBeforeFileExtractedWithChangingFileName(void);

private:
	void beforeFileExtractedSignalHandler_ChangeFileName(std::string & fileName);

private:
	std::shared_ptr<Unzip> zip;

	std::string zipFile;
	std::string zipFileWithUmlaut;
	std::string anotherZipFile;
	std::string notExistingZipFile;
	std::string passwordProtectedZipFile_linux32bit;
	std::string passwordProtectedZipFile_linux64bit;
	std::string passwordProtectedZipFile_win32bit;
	std::string passwordProtectedZipFile_win64bit_7zip;
	std::string passwordProtectedZipFile_win64bit_winrar;
	std::string tempFolder;
	std::string picsFolder;
	std::string fileInsideZip;
	std::string fileInsideZip_ReadMe;
	std::string fileInsideZipWithUmlaut;
	std::string fileInsideZipThatDoesNotExist;
	std::string fileInsideZipJpg;
	std::string anotherFileName;
};

} //cppzip

#endif /* CPPZIP_UNZIPTEST_H_ */
