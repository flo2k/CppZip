/*
 * UnzipTest.cpp
 *
 *  Created on: 05.11.2011
 *      Author: Florian Künzner
 */

#include "UnzipTest.h"
#include <Unzip.h>
#include <cppunit/extensions/HelperMacros.h>

#include <list>
#include <string>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <algorithm>

namespace cppzip {

CPPUNIT_TEST_SUITE_REGISTRATION(UnzipTest);

void UnzipTest::setUp(void){
	zipFile = "test.zip";
	zipFileWithUmlaut = "täst.zip";
	anotherZipFile = "another_test.zip";
	notExistingZipFile = "not_existing_file.zip";
	passwordProtectedZipFile_linux32bit = "passwordProtected_linux_32bit.zip";
	passwordProtectedZipFile_linux64bit = "passwordProtected_linux_64bit.zip";
	passwordProtectedZipFile_win32bit = "passwordProtected_win_32bit.zip";
	passwordProtectedZipFile_win64bit_7zip = "passwordProtected_win_64bit_7zip.zip";
	passwordProtectedZipFile_win64bit_winrar = "passwordProtected_win_64bit_winrar.zip";
	tempFolder = "temp";
	picsFolder = "pics";
	fileInsideZip = "Pruefplan.txt";
	fileInsideZip_ReadMe = "readme.txt";
	fileInsideZipWithUmlaut = "Prüfplan.txt";
	fileInsideZipThatDoesNotExist = "FileDoesNotExist";
	fileInsideZipJpg = "matrix.jpg";
	anotherFileName = "x.txt";

	zip = std::shared_ptr<Unzip>(new Unzip());
	boost::filesystem::remove_all(tempFolder);
}

void UnzipTest::tearDown(void){

}

void UnzipTest::test_isOpenedWithNonOpenedZipFile(void){
	bool expected = false;
	bool actual = zip->isOpened();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void UnzipTest::test_openExistingZipFile(void)
{
	bool expected = true;
	bool openOk = zip->open(zipFile);
	bool isOpened = zip->isOpened();

	CPPUNIT_ASSERT_EQUAL(expected, openOk);
	CPPUNIT_ASSERT_EQUAL(expected, isOpened);
}

void UnzipTest::test_openZipFileWhileAnotherIsAlreadyOpened(void)
{
	bool actual = zip->open(zipFile);
	CPPUNIT_ASSERT_EQUAL(true, actual);

	actual = zip->open(anotherZipFile);
	CPPUNIT_ASSERT_EQUAL(false, actual);

	actual = zip->isOpened();
	CPPUNIT_ASSERT_EQUAL(true, actual);
}

void UnzipTest::test_openANonExistingZipFile(void)
{
	bool expected = false;
	bool actual = zip->open(notExistingZipFile);

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void UnzipTest::test_closeAZipFile(void)
{
	bool expected = true;
	zip->open(zipFile);
	bool actual = zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void UnzipTest::test_closeWhenNoZipFileIsOpened(void)
{
	bool expected = true;
	bool actual = zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void UnzipTest::test_getNumFiles(void)
{
	int expected = 6;
	zip->open(zipFile);
	int actual = zip->getNumFiles();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void UnzipTest::test_getNumFilesWithNoZipFileIsOpened(void)
{
	int expected = 0;
	int actual = zip->getNumFiles();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void UnzipTest::test_getNumFilesAfterCloseZipFiled(void)
{
	int expected = 0;
	zip->open(zipFile);
	zip->close();
	int actual = zip->getNumFiles();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void UnzipTest::test_containsFile(void)
{
	bool expected = true;
	zip->open(zipFile);
	bool actual = zip->containsFile(fileInsideZipWithUmlaut);

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void UnzipTest::test_containsFileWithANonExistingFile(void)
{
	bool expected = false;
	zip->open(zipFile);
	bool actual = zip->containsFile(fileInsideZipThatDoesNotExist);

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void UnzipTest::test_containsFileWithNoZipFileIsOpened(void)
{
	bool expected = false;
	bool actual = zip->containsFile(fileInsideZipWithUmlaut);

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void UnzipTest::test_containsFileAfterCloseZipFiled(void)
{
	bool expected = false;
	zip->open(zipFile);
	zip->close();
	bool actual = zip->containsFile(fileInsideZipWithUmlaut);

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void UnzipTest::test_getFileNames(void)
{
	std::vector<std::string> expectedFileNames = {
			"Prüfplan.txt",
			"Pruefplan.txt",
			"info/readme.txt",
			"info/",
			"pics/",
			"pics/matrix.jpg"
	};

	zip->open(zipFile);
	std::list<std::string> actualfileNames = zip->getFileNames();

//	BOOST_FOREACH(std::string fileName, fileNames){
//		std::cout << fileName << std::endl;
//	}

	CPPUNIT_ASSERT_EQUAL(expectedFileNames.size(), actualfileNames.size());

	//assert the file names..
	for(size_t i = 0; i < actualfileNames.size(); ++i){
		std::string fileName = expectedFileNames[i];
		int found = std::count(actualfileNames.begin(), actualfileNames.end(), fileName);
		CPPUNIT_ASSERT_EQUAL(1, found);
	}
}

void UnzipTest::test_getFileNamesWithNoZipFileIsOpened(void)
{
	std::vector<std::string> expected;
	std::list<std::string> actual = zip->getFileNames();

	CPPUNIT_ASSERT_EQUAL(expected.size(), actual.size());
}

void UnzipTest::test_getFileNamesAfterCloseZipFiled(void)
{
	std::vector<std::string> expected;
	zip->open(zipFile);
	zip->close();
	std::list<std::string> actual = zip->getFileNames();

	CPPUNIT_ASSERT_EQUAL(expected.size(), actual.size());
}

void UnzipTest::test_getFileContent(void)
{
	int expected = 2;
	zip->open(zipFile);
	std::vector<unsigned char> content = zip->getFileContent(fileInsideZip);
	int actual = content.size();

//	std::string x(content.begin(), content.end());
//	std::cout << "Prüfplan: " << x << std::endl;

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void UnzipTest::test_getFileContentWithNoZipFileIsOpened(void)
{
	int expected = 0;
	std::vector<unsigned char> content = zip->getFileContent(fileInsideZip);
	int actual = content.size();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void UnzipTest::test_getFileContentAfterCloseZipFiled(void)
{
	int expected = 0;
	zip->open(zipFile);
	zip->close();
	std::vector<unsigned char> content = zip->getFileContent(fileInsideZip);
	int actual = content.size();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void UnzipTest::test_getFileContentFromANonExistingFile(void)
{
	int expected = 0;
	zip->open(zipFile);
	std::vector<unsigned char> content = zip->getFileContent(fileInsideZipThatDoesNotExist);
	int actual = content.size();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void UnzipTest::test_getFileContentFromPasswordProtectedZipFile_linux32bit(void)
{
	bool ok = zip->open(passwordProtectedZipFile_linux32bit, "secret");
	CPPUNIT_ASSERT_EQUAL_MESSAGE("open", true, ok);
	std::vector<unsigned char> content = zip->getFileContent(fileInsideZip);

	std::string expected = "this is a string";
	std::string actual(content.begin(), content.end());

	std::cout << "expected: " << expected << std::endl;
	std::cout << "actual: " << actual << std::endl;

	CPPUNIT_ASSERT_EQUAL_MESSAGE("file content", expected, actual);
}

void UnzipTest::test_getFileContentFromPasswordProtectedZipFile_linux64bit(void)
{
	bool ok = zip->open(passwordProtectedZipFile_linux64bit, "secret");
	CPPUNIT_ASSERT_EQUAL_MESSAGE("open", true, ok);
	std::vector<unsigned char> content = zip->getFileContent(fileInsideZip);

	std::string expected = "this is a string";
	std::string actual(content.begin(), content.end());

	std::cout << "expected: " << expected << std::endl;
	std::cout << "actual: " << actual << std::endl;

	CPPUNIT_ASSERT_EQUAL_MESSAGE("file content", expected, actual);
}

void UnzipTest::test_getFileContentFromPasswordProtectedZipFile_win32bit(void)
{
	bool ok = zip->open(passwordProtectedZipFile_win32bit, "secret");
	CPPUNIT_ASSERT_EQUAL_MESSAGE("open", true, ok);
	std::vector<unsigned char> content = zip->getFileContent(fileInsideZip);

	std::string expected = "this is a string";
	std::string actual(content.begin(), content.end());

	std::cout << "expected: " << expected << std::endl;
	std::cout << "actual: " << actual << std::endl;

	CPPUNIT_ASSERT_EQUAL_MESSAGE("file content", expected, actual);
}

void UnzipTest::test_getFileContentFromPasswordProtectedZipFile_win64bit_7zip(void)
{
	bool ok = zip->open(passwordProtectedZipFile_win64bit_7zip, "secret");
	CPPUNIT_ASSERT_EQUAL_MESSAGE("open", true, ok);
	std::vector<unsigned char> content = zip->getFileContent(fileInsideZip);

	std::string expected = "this is a string";
	std::string actual(content.begin(), content.end());

	std::cout << "expected: " << expected << std::endl;
	std::cout << "actual: " << actual << std::endl;

	CPPUNIT_ASSERT_EQUAL_MESSAGE("file content", expected, actual);
}

void UnzipTest::test_getFileContentFromPasswordProtectedZipFile_win64bit_winrar(void)
{
	bool ok = zip->open(passwordProtectedZipFile_win64bit_winrar, "secret");
	CPPUNIT_ASSERT_EQUAL_MESSAGE("open", true, ok);
	std::vector<unsigned char> content = zip->getFileContent(fileInsideZip);

	std::string expected = "this is a string";
	std::string actual(content.begin(), content.end());

	std::cout << "expected: " << expected << std::endl;
	std::cout << "actual: " << actual << std::endl;

	CPPUNIT_ASSERT_EQUAL_MESSAGE("file content", expected, actual);
}

void UnzipTest::test_extractFile(void)
{
	bool expected = true;
	zip->open(zipFile);
	bool actual = zip->extractFileTo(
			fileInsideZipWithUmlaut,
			tempFolder + "/" + fileInsideZipWithUmlaut);

	CPPUNIT_ASSERT_EQUAL(expected, actual);

	size_t expectedFileSizeInBytes = 40;
	size_t actualFileSizeInBytes =
			boost::filesystem::file_size(tempFolder + "/" + fileInsideZipWithUmlaut);

	CPPUNIT_ASSERT_EQUAL(expectedFileSizeInBytes, actualFileSizeInBytes);
}

void UnzipTest::test_extractFile_AJpg(void)
{
	bool expected = true;
	zip->open(zipFile);
	std::string fileName = tempFolder + "/" + fileInsideZipJpg;
	bool actual = zip->extractFileTo(picsFolder + "/" + fileInsideZipJpg, fileName);

	CPPUNIT_ASSERT_EQUAL(expected, actual);

	size_t expectedFileSizeInBytes = 220635;
	size_t actualFileSizeInBytes = boost::filesystem::file_size(fileName);

	CPPUNIT_ASSERT_EQUAL(expectedFileSizeInBytes, actualFileSizeInBytes);
}

void UnzipTest::test_extractFile_WithOverwriteAExistingFile(void)
{
	zip->open(zipFile);

	bool actual = zip->extractFileTo(
			fileInsideZip,
			tempFolder + "/" + fileInsideZip);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("extract not existing", true, actual);

	actual = zip->extractFileTo(
			fileInsideZipWithUmlaut,
			tempFolder + "/" + fileInsideZip,
			true);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("extract and overwrite existing", true, actual);
}

void UnzipTest::test_extractFile_WithNotOverwriteAExistingFile(void)
{
	zip->open(zipFile);

	bool actual = zip->extractFileTo(
			fileInsideZip,
			tempFolder + "/" + fileInsideZip);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("extract not existing", true, actual);

	actual = zip->extractFileTo(
			fileInsideZipWithUmlaut,
			tempFolder + "/" + fileInsideZip,
			false);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("extract and overwrite existing", false, actual);
}

void UnzipTest::test_extractAllFilesTo(void)
{
	bool expected = true;
	zip->open(zipFile);
	std::string path = tempFolder;

	bool actual = zip->extractAllFilesTo(path);
	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void UnzipTest::test_extractAllFilesToWithUmlaut(void)
{
	bool expected = true;
	zip->open(zipFile);
	std::string path = tempFolder;

	bool actual = zip->extractAllFilesTo(path);
	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void UnzipTest::test_extractAllFiles_WithOverwriteAExistingFile(void)
{
	zip->open(zipFile);
	std::string path = tempFolder;

	bool actual = zip->extractAllFilesTo(path);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("extract not existing", true, actual);

	actual = zip->extractAllFilesTo(path, true);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("extract and overwrite existing", true, actual);
}

void UnzipTest::test_extractAllFiles_WithNotOverwriteAExistingFile(void)
{
	zip->open(zipFile);
	std::string path = tempFolder;

	bool actual = zip->extractAllFilesTo(path);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("extract not existing", true, actual);

	actual = zip->extractAllFilesTo(path, false);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("extract and overwrite existing", false, actual);
}

bool actualFileExtracted = false;
std::string actualFileName = "";
int actualMax = -1;
int actualCurrent = -1;
void fileExtractedSignalHandler(const std::string fileName, unsigned int max, unsigned int current){
	actualFileExtracted = true;
	actualFileName = fileName;
	actualMax = max;
	actualCurrent = current;
}

void UnzipTest::test_signalFileExtracted(void)
{
	actualFileExtracted = false;
	actualFileName = "";
	actualMax = -1;
	actualCurrent = -1;

	zip->open(zipFile);
	zip->fileExtracted.connect(boost::bind(fileExtractedSignalHandler, _1, _2, _3));
	zip->extractFileTo(fileInsideZip, tempFolder + "/" + fileInsideZip);

	CPPUNIT_ASSERT_EQUAL(true, actualFileExtracted);
	CPPUNIT_ASSERT_EQUAL(tempFolder + "/" + fileInsideZip, actualFileName);
	CPPUNIT_ASSERT_EQUAL(1, actualMax);
	CPPUNIT_ASSERT_EQUAL(1, actualCurrent);
}

void beforeFileExtractedSignalHandler(const std::string & fileName){
	actualFileName = fileName;
}

void UnzipTest::test_signalBeforeFileExtracted(void)
{
	actualFileName = "";

	zip->open(zipFile);
	zip->beforeFileExtraction.connect(boost::bind(beforeFileExtractedSignalHandler, _1));
	zip->extractFileTo(fileInsideZip, tempFolder + "/" + fileInsideZip);

	CPPUNIT_ASSERT_EQUAL(tempFolder + "/" + fileInsideZip, actualFileName);
}

void UnzipTest::beforeFileExtractedSignalHandler_ChangeFileName(std::string & fileName){
	fileName = tempFolder + "/" + anotherFileName;
}

void UnzipTest::test_signalBeforeFileExtractedWithChangingFileName(void)
{
	bool expected = true;

	zip->open(zipFile);
	zip->beforeFileExtraction.connect(boost::bind(
			&UnzipTest::beforeFileExtractedSignalHandler_ChangeFileName, this, _1));
	zip->extractFileTo(fileInsideZip, tempFolder + "/" + fileInsideZip);

	bool actual = boost::filesystem::exists(
			boost::filesystem::path(tempFolder + "/" + anotherFileName));
	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

} //cppzip
