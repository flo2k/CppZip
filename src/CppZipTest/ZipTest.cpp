/*
 * ZipTest.cpp
 *
 *  Created on: 15.11.2011
 *      Author: Florian Künzner
 */

#include "ZipTest.h"
#include <Zip.h>
#include <cppunit/extensions/HelperMacros.h>

#include <list>
#include <string>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/v3/operations.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <algorithm>

CPPUNIT_TEST_SUITE_REGISTRATION(ZipTest);

void ZipTest::setUp(void)
{
	zipFile = "test.zip";
	zipFileWithUmlaut = "täst.zip";
	anotherZipFile = "another_test.zip";
	notExistingZipFile = "not_existing_file.zip";
	writeProtectedZipFile = "write_protected_file.zip";
	tempFolder = "temp";
	picsFolder = "pics";
	fileInsideZipWithUmlaut = "Prüfplan.txt";
	fileInsideZip = "Pruefplan.txt";
	fileInsideZipThatDoesNotExist = "FileDoesNotExist";
	fileInsideZipJpg = "matrix.jpg";
	anotherFileName = "x.txt";
	readMeFileName = "data/ReadMe";
	notExistingFileName = "fileDoesNotExist";

	zip = std::shared_ptr<Zip>(new Zip());
	boost::filesystem::remove_all(tempFolder);
}

void ZipTest::tearDown(void)
{
}

void ZipTest::test_isOpenedWithNonOpenedZipFile(void)
{
	bool expected = false;
	bool actual = zip->isOpened();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_openANonExistingZipFile(void)
{
	bool expected = true;
	bool openOk = zip->open(tempFolder + "/" + zipFile);
	bool isOpened = zip->isOpened();

	CPPUNIT_ASSERT_EQUAL(expected, openOk);
	CPPUNIT_ASSERT_EQUAL(expected, isOpened);
}

void ZipTest::test_openAExistingZipFile(void)
{
	bool expected = true;
	boost::filesystem::create_directories(tempFolder);

	//workaround, because boost::filesystem::copy_file don't link
	//because there are some missing symbols...
	//tested on my ubuntu 11.10 64bit with boost 1.46.1
	//boost::filesystem::copy_file(zipFile, tempFolder + "/" + zipFile);
	std::string cmd = "touch " + tempFolder + "/" + zipFile;
	system(cmd.c_str());

	bool actual = zip->open(tempFolder + "/" + zipFile);

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_openAExistingWriteProtectedZipFile(void)
{
	bool expected = false;
	bool openOk = zip->open(writeProtectedZipFile);
	bool isOpened = zip->isOpened();

	CPPUNIT_ASSERT_EQUAL(expected, openOk);
	CPPUNIT_ASSERT_EQUAL(expected, isOpened);
}

void ZipTest::test_closeAZipFile(void)
{
	bool expected = true;
	zip->open(tempFolder + "/" + zipFile);
	bool actual = zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_closeWhenNoZipFileIsOpened(void)
{
	bool expected = true;
	bool actual = zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addFile(void)
{
	bool expected = true;
	zip->open(tempFolder + "/" + zipFile);
	bool actual = zip->addFile(readMeFileName, false);

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addFile_WhenFileNotExists(void)
{
	bool expected = false;
	zip->open(tempFolder + "/" + zipFile);
	bool actual = zip->addFile(notExistingFileName, false);

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addFile_WhenFileNameAlreadyExists(void)
{
	bool expected = false;
	zip->open(tempFolder + "/" + zipFile);
	zip->addFile(readMeFileName, false);
	bool actual = zip->addFile(readMeFileName, false);

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addFile_WithDestinationFile(void)
{
	bool expected = true;
	zip->open(tempFolder + "/" + zipFile);
	bool actual = zip->addFile(readMeFileName, anotherFileName);

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addFile_WithDestinationFile_WhenFileNotExists(void)
{
	bool expected = false;
	zip->open(tempFolder + "/" + zipFile);
	bool actual = zip->addFile(notExistingFileName, anotherFileName);

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addFile_WithDestinationFile_WithFileNameIsEmpty(void)
{
	bool expected = false;
	zip->open(tempFolder + "/" + zipFile);
	std::string fileEmpty = "";
	bool actual = zip->addFile(readMeFileName, fileEmpty);

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addFile_WithDestinationFile_WhenFileNameAlreadyExists(void)
{
	bool expected = false;
	zip->open(tempFolder + "/" + zipFile);
	zip->addFile(readMeFileName, false);
	bool actual = zip->addFile(readMeFileName, readMeFileName);

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addFile_Content(void)
{
	std::vector<unsigned char> content = {'a', 'z', '7'};
	bool expected = true;
	zip->open(tempFolder + "/" + zipFile);
	bool actual = zip->addFile("test.txt", content);

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addFile_Content_FromAString(void)
{
	std::string contentAsString("this is a string");
	std::vector<unsigned char> content;
	content.insert(content.end(), contentAsString.begin(), contentAsString.end());
	bool expected = true;
	zip->open(tempFolder + "/" + zipFile);
	bool actual = zip->addFile("test.txt", content);

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addFile_Content_WithEmptyContent(void)
{
	std::vector<unsigned char> content;
	bool expected = true;
	zip->open(tempFolder + "/" + zipFile);
	bool actual = zip->addFile("test.txt", content);

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addFile_Content_WithSubFoldersFileName(void)
{
	std::vector<unsigned char> content = {'a', 'z', '7'};
	bool expected = true;
	zip->open(tempFolder + "/" + zipFile);
	bool actual = zip->addFile("folder/subfolder/test.txt", content);

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addFile_Content_IfZipIsNotOpened(void)
{
	std::vector<unsigned char> content = {'a', 'z', '7'};
	bool expected = false;
	bool actual = zip->addFile("test.txt", content);

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addEmptyFolder(void)
{
	bool expected = true;
	zip->open(tempFolder + "/" + zipFile);
	bool actual = zip->addEmptyFolder("folder");

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addEmptyFolder_WithUmlaut(void)
{
	bool expected = true;
	zip->open(tempFolder + "/" + zipFile);
	bool actual = zip->addEmptyFolder("földer");

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addEmptyFolder_WhenFolderAlreadyExists(void)
{
	bool expected = true;
	zip->open(tempFolder + "/" + zipFile);
	bool actual = zip->addEmptyFolder("folder");
	CPPUNIT_ASSERT_EQUAL(expected, actual);

	actual = zip->addEmptyFolder("folder");
	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addEmptyFolder_WithSubFolders(void)
{
	bool expected = true;
	zip->open(tempFolder + "/" + zipFile);
	bool actual = zip->addEmptyFolder("folder/subfolder/subsubfolder");

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addEmptyFolder_WithSubFolders_WhenAFolderPartAlreadyExists(void)
{
	bool expected = true;
	zip->open(tempFolder + "/" + zipFile);

	bool actual = zip->addEmptyFolder("folder/subfolder1/subsubfolder1");
	CPPUNIT_ASSERT_EQUAL(expected, actual);

	actual = zip->addEmptyFolder("folder/subfolder1/subsubfolder2");
	CPPUNIT_ASSERT_EQUAL(expected, actual);

	actual = zip->addEmptyFolder("folder/subfolder1");
	CPPUNIT_ASSERT_EQUAL(expected, actual);

	actual = zip->addEmptyFolder("folder/subfolder2");
	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addEmptyFolder_WithSubFolders_WindowsStyle(void)
{
	bool expected = true;
	zip->open(tempFolder + "/" + zipFile);
	bool actual = zip->addEmptyFolder("folder\\subfolder\\subsubfolder");

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}
