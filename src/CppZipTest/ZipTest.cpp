/*
 * ZipTest.cpp
 *
 *  Created on: 15.11.2011
 *      Author: Florian Künzner
 */

#include "ZipTest.h"
#include <Zip.h>
#include <Unzip.h>
#include <cppunit/extensions/HelperMacros.h>

#include <list>
#include <string>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/algorithm/string.hpp>
#include <algorithm>

namespace cppzip {

CPPUNIT_TEST_SUITE_REGISTRATION(ZipTest);

void ZipTest::setUp(void) {
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
	zipFileFor_deleteAndReplace = "zipFileFor_deleteAndReplaceTests.zip";
	folderNameInsideZip = "rootFolder";

	zip = std::shared_ptr<Zip> (new Zip());
	boost::filesystem::remove_all(tempFolder);
}

void ZipTest::tearDown(void) {
	zip->close();
	boost::filesystem::remove_all(tempFolder);
}

void ZipTest::test_isOpenedWithNonOpenedZipFile(void) {
	bool expected = false;

	bool actual = zip->isOpened();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_openANonExistingZipFile(void) {
	bool expected = true;

	bool openOk = zip->open(tempFolder + "/" + zipFile);
	bool isOpened = zip->isOpened();

	CPPUNIT_ASSERT_EQUAL(expected, openOk);
	CPPUNIT_ASSERT_EQUAL(expected, isOpened);
}

void ZipTest::test_openAExistingZipFile(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	createFolder(tempFolder);
	copyFile(zipFile, zipFileName);

	bool actual = zip->open(zipFileName);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 0, numFilesInZip(zipFileName));
}

void ZipTest::test_openAExistingWriteProtectedZipFile(void) {
	bool expected = false;

	//make the file write protected
	boost::filesystem::perms savedFilePerms = boost::filesystem::status(writeProtectedZipFile).permissions();
	boost::filesystem::permissions(writeProtectedZipFile, boost::filesystem::owner_read);

	bool openOk = zip->open(writeProtectedZipFile);
	bool isOpened = zip->isOpened();

	//make the file writeable again
	boost::filesystem::permissions(writeProtectedZipFile, savedFilePerms);

	CPPUNIT_ASSERT_EQUAL(expected, openOk);
	CPPUNIT_ASSERT_EQUAL(expected, isOpened);
}

void ZipTest::test_openAppendToZip(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	createFolder(tempFolder);
	copyFile(zipFile, zipFileName);

	zip->open(zipFileName, Zip::OpenExisting);
	std::string theString("Lorem Ipsum...");
	std::vector<unsigned char> content;
	content.insert(content.end(), theString.begin(), theString.end());
	bool actual = zip->addFile("file1.txt", content);
	zip->close();

	CPPUNIT_ASSERT_EQUAL_MESSAGE("add", expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, "file1.txt"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 7, numFilesInZip(zipFileName));
}

void ZipTest::test_openAppendToZipWithFileAlreadyExisting(void) {
	bool expected = false;
	std::string zipFileName = tempFolder + "/" + zipFile;

	createFolder(tempFolder);
	copyFile(zipFile, zipFileName);

	//first open
	zip->open(zipFileName, Zip::OpenExisting);
	std::string theString("Lorem Ipsum...");
	std::vector<unsigned char> content;
	content.insert(content.end(), theString.begin(), theString.end());
	zip->addFile("file1.txt", content);
	zip->close();

	//second open
	zip->open(zipFileName, Zip::OpenExisting);
	theString = "Lorem Ipsum Number 2...";
	content.insert(content.end(), theString.begin(), theString.end());
	bool actual = zip->addFile("file1.txt", content);
	zip->close();

	CPPUNIT_ASSERT_EQUAL_MESSAGE("add", expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, "file1.txt"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 7, numFilesInZip(zipFileName));
}

void ZipTest::test_closeAZipFile(void) {
	bool expected = true;

	zip->open(tempFolder + "/" + zipFile);
	bool actual = zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_closeWhenNoZipFileIsOpened(void) {
	bool expected = true;

	bool actual = zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addFile(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	bool actual = zip->addFile(readMeFileName);
	zip->close();

	CPPUNIT_ASSERT_EQUAL_MESSAGE("add", expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 1, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, readMeFileName));
}

void ZipTest::test_addFile_WhenFileNotExists(void) {
	bool expected = false;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	bool actual = zip->addFile(notExistingFileName);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 0, numFilesInZip(zipFileName));
}

void ZipTest::test_addFile_WhenFileNameAlreadyExists(void) {
	bool expected = false;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	zip->addFile(readMeFileName);
	bool actual = zip->addFile(readMeFileName);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 1, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, readMeFileName));
}

void ZipTest::test_addFile_WithNotPreservePath(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	bool actual = zip->addFile(readMeFileName, false);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 1, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, boost::filesystem::path(readMeFileName).filename().string()));
}

void ZipTest::test_addFile_WithDestinationFile(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	bool actual = zip->addFile(readMeFileName, anotherFileName);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 1, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, anotherFileName));
}

void ZipTest::test_addFile_WithDestinationFile_WhenFileNotExists(void) {
	bool expected = false;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	bool actual = zip->addFile(notExistingFileName, anotherFileName);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 0, numFilesInZip(zipFileName));
}

void ZipTest::test_addFile_WithDestinationFile_WithFileNameIsEmpty(void) {
	bool expected = false;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	std::string emptyFileName = "";
	bool actual = zip->addFile(readMeFileName, emptyFileName);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 0, numFilesInZip(zipFileName));
}

void ZipTest::test_addFile_WithDestinationFile_WhenFileNameAlreadyExists(void) {
	bool expected = false;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	zip->addFile(readMeFileName, anotherFileName);
	bool actual = zip->addFile(readMeFileName, anotherFileName);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 1, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, anotherFileName));
}

void ZipTest::test_addFile_Content(void) {
	std::vector<unsigned char> content;
	content.push_back('a');
	content.push_back('z');
	content.push_back('7');

	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	bool actual = zip->addFile(fileInsideZip, content);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 1, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, fileInsideZip));
}

void ZipTest::test_addFile_Content_FromAString(void) {
	std::string contentAsString("this is a string");
	std::vector<unsigned char> content;
	content.insert(content.end(), contentAsString.begin(), contentAsString.end());
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	bool actual = zip->addFile(fileInsideZip, content);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 1, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, fileInsideZip));
}

void ZipTest::test_addFile_Content_WithEmptyContent(void) {
	std::vector<unsigned char> content;
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	bool actual = zip->addFile(fileInsideZip, content);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 1, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, fileInsideZip));
}

void ZipTest::test_addFile_Content_WithSubFoldersFileName(void) {
	std::vector<unsigned char> content;
	content.push_back('a');
	content.push_back('z');
	content.push_back('7');

	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	bool actual = zip->addFile("folder/subfolder/test.txt", content);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 1, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, "folder/subfolder/test.txt"));
}

void ZipTest::test_addFile_Content_IfZipIsNotOpened(void) {
	std::vector<unsigned char> content;
	content.push_back('a');
	content.push_back('z');
	content.push_back('7');

	bool expected = false;
	bool actual = zip->addFile("test.txt", content);

	CPPUNIT_ASSERT_EQUAL(expected, actual);
}

void ZipTest::test_addFiles(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	std::string dataDir = "data/test/";
	std::list<std::string> fileNames;
	fileNames.push_back(dataDir + fileInsideZip);
	fileNames.push_back(dataDir + fileInsideZipWithUmlaut);

	bool actual = zip->addFiles(fileNames);
	zip->close();

	CPPUNIT_ASSERT_EQUAL_MESSAGE("add", expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 2, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, dataDir + fileInsideZip));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, dataDir + fileInsideZipWithUmlaut));
}

void ZipTest::test_addFiles_WithNotPreservePath(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	std::string dataDir = "data/test/";
	std::list<std::string> fileNames;
	fileNames.push_back(dataDir + fileInsideZip);
	fileNames.push_back(dataDir + fileInsideZipWithUmlaut);

	bool actual = zip->addFiles(fileNames, false);
	zip->close();

	CPPUNIT_ASSERT_EQUAL_MESSAGE("add", expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 2, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, fileInsideZip));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, fileInsideZipWithUmlaut));
}

void ZipTest::test_addFiles_WhenOneFileNotExists(void) {
	bool expected = false;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	std::string dataDir = "data/test/";
	std::list<std::string> fileNames;
	fileNames.push_back(notExistingFileName);
	fileNames.push_back(dataDir + fileInsideZip);

	bool actual = zip->addFiles(fileNames);
	zip->close();

	CPPUNIT_ASSERT_EQUAL_MESSAGE("add", expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 1, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, dataDir + fileInsideZip));
}

void ZipTest::test_addEmptyFolder(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	bool actual = zip->addEmptyFolder("folder");
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 1, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFolder(zipFileName, "folder"));
}

void ZipTest::test_addEmptyFolder_WithUmlaut(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	bool actual = zip->addEmptyFolder("földer");
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 1, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFolder(zipFileName, "földer"));
}

void ZipTest::test_addEmptyFolder_WhenFolderAlreadyExists(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	bool actual = zip->addEmptyFolder("folder");
	CPPUNIT_ASSERT_EQUAL(expected, actual);

	actual = zip->addEmptyFolder("folder");
	CPPUNIT_ASSERT_EQUAL(expected, actual);

	zip->close();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 1, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFolder(zipFileName, "folder"));
}

void ZipTest::test_addEmptyFolder_WithSubFolders(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	bool actual = zip->addEmptyFolder("folder/subfolder/subsubfolder");
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 1, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFolder(zipFileName, "folder/subfolder/subsubfolder"));
}

void ZipTest::test_addEmptyFolder_WithSubFolders_WhenAFolderPartAlreadyExists(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);

	bool actual = zip->addEmptyFolder("folder/subfolder1/subsubfolder1");
	CPPUNIT_ASSERT_EQUAL(expected, actual);

	actual = zip->addEmptyFolder("folder/subfolder1/subsubfolder2");
	CPPUNIT_ASSERT_EQUAL(expected, actual);

	actual = zip->addEmptyFolder("folder/subfolder1");
	CPPUNIT_ASSERT_EQUAL(expected, actual);

	actual = zip->addEmptyFolder("folder/subfolder2");
	CPPUNIT_ASSERT_EQUAL(expected, actual);

	zip->close();

	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 4, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFolder(zipFileName, "folder/subfolder1/subsubfolder1"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFolder(zipFileName, "folder/subfolder1/subsubfolder2"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFolder(zipFileName, "folder/subfolder1"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFolder(zipFileName, "folder/subfolder2"));
}

void ZipTest::test_addEmptyFolder_WithSubFolders_WindowsStyle(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	bool actual = zip->addEmptyFolder("folder\\subfolder\\subsubfolder");
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 1, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFolder(zipFileName, "folder/subfolder/subsubfolder"));
}

void ZipTest::test_addFolder_recursive(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	bool actual = zip->addFolder("data/test");
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 7, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFolder(zipFileName, "data/test"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, "data/test/Pruefplan.txt"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, "data/test/Prüfplan.txt"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFolder(zipFileName, "data/test/info"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, "data/test/info/readme.txt"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFolder(zipFileName, "data/test/pics"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, "data/test/pics/matrix.jpg"));
}

void ZipTest::test_addFolder_notRecursive(void){
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	bool actual = zip->addFolder("data/test", true, false);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 3, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFolder(zipFileName, "data/test"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, "data/test/Pruefplan.txt"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, "data/test/Prüfplan.txt"));
}

void ZipTest::test_addFolder_notPreservesPath(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	bool actual = zip->addFolder("data/test", false);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 7, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFolder(zipFileName, "test"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, "test/Pruefplan.txt"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, "test/Prüfplan.txt"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFolder(zipFileName, "test/info"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, "test/info/readme.txt"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFolder(zipFileName, "test/pics"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, "test/pics/matrix.jpg"));
}

void ZipTest::test_addFolder_notPreservesPath_and_notRecursive(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFile;

	zip->open(zipFileName);
	bool actual = zip->addFolder("data/test", false, false);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 3, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFolder(zipFileName, "test"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, "test/Pruefplan.txt"));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, "test/Prüfplan.txt"));
}

void ZipTest::test_deleteFile(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFileFor_deleteAndReplace;

	createFolder(tempFolder);
	copyFile(zipFileFor_deleteAndReplace, zipFileName);

	zip->open(zipFileName, Zip::OpenExisting);
	std::string fileToDelete = folderNameInsideZip + "/file1.txt";
	bool actual  = zip->deleteFile(fileToDelete);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 7, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", false, containsFile(zipFileName, fileToDelete));
}

void ZipTest::test_deleteFiles(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFileFor_deleteAndReplace;

	createFolder(tempFolder);
	copyFile(zipFileFor_deleteAndReplace, zipFileName);

	zip->open(zipFileName, Zip::OpenExisting);
	std::string fileToDelete1 = folderNameInsideZip + "/file1.txt";
	std::string fileToDelete2 = folderNameInsideZip + "/file2.txt";
	std::list<std::string> filesToDelete;
	filesToDelete.push_back(fileToDelete1);
	filesToDelete.push_back(fileToDelete2);

	bool actual  = zip->deleteFiles(filesToDelete);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 6, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", false, containsFile(zipFileName, fileToDelete1));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", false, containsFile(zipFileName, fileToDelete2));
}

void ZipTest::test_deleteFolder(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFileFor_deleteAndReplace;

	createFolder(tempFolder);
	copyFile(zipFileFor_deleteAndReplace, zipFileName);

	zip->open(zipFileName, Zip::OpenExisting);
	std::string folderToDelete = folderNameInsideZip + "/folder1";
	bool actual = zip->deleteFolder(folderToDelete);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 5, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", false, containsFolder(zipFileName, folderToDelete));
}

void ZipTest::test_deleteFolders(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFileFor_deleteAndReplace;

	createFolder(tempFolder);
	copyFile(zipFileFor_deleteAndReplace, zipFileName);

	zip->open(zipFileName, Zip::OpenExisting);
	std::string folderToDelete1 = folderNameInsideZip + "/folder1";
	std::string folderToDelete2 = folderNameInsideZip + "/folder2";
	std::list<std::string> foldersToDelete;
	foldersToDelete.push_back(folderToDelete1);
	foldersToDelete.push_back(folderToDelete2);

	bool actual = zip->deleteFolders(foldersToDelete);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 2, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", false, containsFolder(zipFileName, folderToDelete1));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", false, containsFolder(zipFileName, folderToDelete2));
}

void ZipTest::test_deleteFile_WhenFileNotExists(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFileFor_deleteAndReplace;

	createFolder(tempFolder);
	copyFile(zipFileFor_deleteAndReplace, zipFileName);

	zip->open(zipFileName, Zip::OpenExisting);
	bool actual = zip->deleteFile(notExistingFileName);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 8, numFilesInZip(zipFileName));
}

void ZipTest::test_deleteFile_WhenTemparyFileCouldntCreated(void) {
	bool expected = false;
	std::string zipFileName = tempFolder + "/" + zipFileFor_deleteAndReplace;
	std::string tempZipFileName = tempFolder + "/" + ".~" + zipFileFor_deleteAndReplace;
	
	createFolder(tempFolder);
	copyFile(zipFileFor_deleteAndReplace, zipFileName);
	
#ifdef WIN32
	createFile(tempZipFileName, "");
	// make tempZipFileName that zip want's to create read-only
	boost::filesystem::perms savedTempfolderPerms = boost::filesystem::status(tempZipFileName).permissions();
	boost::filesystem::permissions(tempZipFileName, boost::filesystem::owner_read);
#else
	// make folder which contains the zip read-only
	boost::filesystem::perms savedTempfolderPerms = boost::filesystem::status(tempFolder).permissions();
	boost::filesystem::permissions(tempFolder, boost::filesystem::owner_read | boost::filesystem::owner_exe);
#endif

	zip->open(zipFileName, Zip::OpenExisting);
	std::string fileToDelete = folderNameInsideZip + "/file1.txt";
	bool actual = zip->deleteFile(fileToDelete);
	zip->close();

#ifdef WIN32
	// make the file writeable again so it can be deleted
	boost::filesystem::permissions(tempZipFileName, savedTempfolderPerms);
#else
	// make the folder writeable again so it can be deleted
	boost::filesystem::permissions(tempFolder, savedTempfolderPerms);
#endif

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 8, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, fileToDelete));
}

void ZipTest::test_replaceFile(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFileFor_deleteAndReplace;

	createFolder(tempFolder);
	copyFile(zipFileFor_deleteAndReplace, zipFileName);

	bool actual = false;

	zip->open(zipFileName, Zip::OpenExisting);
	std::string fileToReplace = folderNameInsideZip + "/file2.txt";
	actual = zip->replaceFile(readMeFileName, fileToReplace);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 8, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, fileToReplace));
}

void ZipTest::test_replaceFile_WhenFileNotExistsInZip(void) {
	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFileFor_deleteAndReplace;

	createFolder(tempFolder);
	copyFile(zipFileFor_deleteAndReplace, zipFileName);

	zip->open(zipFileName, Zip::OpenExisting);
	std::string fileToReplace = folderNameInsideZip + "/file2.txt";
	bool actual = zip->replaceFile(readMeFileName, notExistingFileName);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 9, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, notExistingFileName));
}

void ZipTest::test_replaceFile_WhenFileNotExistsOnFileSystem(void){
	bool expected = false;
	std::string zipFileName = tempFolder + "/" + zipFileFor_deleteAndReplace;

	createFolder(tempFolder);
	copyFile(zipFileFor_deleteAndReplace, zipFileName);

	zip->open(zipFileName, Zip::OpenExisting);
	std::string fileToReplace = folderNameInsideZip + "/file2.txt";
	bool actual = zip->replaceFile(notExistingFileName, fileToReplace);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 7, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", false, containsFile(zipFileName, fileToReplace));
}

void ZipTest::test_replaceFile_Content(void) {
	std::vector<unsigned char> content;
	content.push_back('a');
	content.push_back('z');
	content.push_back('7');

	bool expected = true;
	std::string zipFileName = tempFolder + "/" + zipFileFor_deleteAndReplace;

	createFolder(tempFolder);
	copyFile(zipFileFor_deleteAndReplace, zipFileName);

	zip->open(zipFileName, Zip::OpenExisting);
	std::string fileToReplace = folderNameInsideZip + "/file2.txt";
	bool actual = zip->replaceFile(fileToReplace, content);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 8, numFilesInZip(zipFileName));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(zipFileName, fileToReplace));
}

void ZipTest::test_addFile_WithPasswordProtection(void) {
	bool expected = true;
	zip->open(tempFolder + "/" + zipFile, Zip::CreateAndOverwrite, "secret");
	bool actual = zip->addFile("data/test/" + fileInsideZip , false);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 1, numFilesInZip(tempFolder + "/" + zipFile));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(tempFolder + "/" + zipFile, fileInsideZip));

	std::vector<unsigned char> fileContent = getFileContent(tempFolder + "/" + zipFile, fileInsideZip, "secret");
	std::string fileContentAsString(fileContent.begin(), fileContent.end());
	
	//TODO: normally a would test it with the \n at the end, but it seems,
	//		that on windows a carriage return (13) and a new line (10) is added instead
	//		of one simple \n
	//std::string contentAsString("a\n");
	//CPPUNIT_ASSERT_EQUAL_MESSAGE("file content", contentAsString, fileContentAsString);

	std::string contentAsString("a");
	CPPUNIT_ASSERT_MESSAGE("file content", boost::algorithm::starts_with(fileContentAsString, contentAsString));
}

void ZipTest::test_addFile_Content_WithPasswordProtection(void) {
	std::vector<unsigned char> content;
	content.push_back('a');
	content.push_back('z');
	content.push_back('7');

	bool expected = true;
	zip->open(tempFolder + "/" + zipFile, Zip::CreateAndOverwrite, "secret");
	bool actual = zip->addFile(fileInsideZip, content);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 1, numFilesInZip(tempFolder + "/" + zipFile));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(tempFolder + "/" + zipFile, fileInsideZip));

	std::vector<unsigned char> fileContent;
	fileContent = getFileContent(tempFolder + "/" + zipFile, fileInsideZip, "secret");
	std::string fileContentAsString(fileContent.begin(), fileContent.end());
	std::string contentAsString(content.begin(), content.end());

	CPPUNIT_ASSERT_EQUAL_MESSAGE("file content", contentAsString, fileContentAsString);
}

void ZipTest::test_addFile_Content_FromAString_WithPasswordProtection(void) {
	std::string contentAsString("this is a string");
	std::vector<unsigned char> content;
	content.insert(content.end(), contentAsString.begin(),
			contentAsString.end());
	bool expected = true;
	zip->open(tempFolder + "/" + zipFile, Zip::CreateAndOverwrite, "secret");
	bool actual = zip->addFile(fileInsideZip, content);
	zip->close();

	CPPUNIT_ASSERT_EQUAL(expected, actual);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("count", 1, numFilesInZip(tempFolder + "/" + zipFile));
	CPPUNIT_ASSERT_EQUAL_MESSAGE("contains", true, containsFile(tempFolder + "/" + zipFile, fileInsideZip));

	std::vector<unsigned char> fileContent;
	fileContent = getFileContent(tempFolder + "/" + zipFile, fileInsideZip, "secret");
	std::string fileContentAsString(fileContent.begin(), fileContent.end());

	CPPUNIT_ASSERT_EQUAL_MESSAGE("file content", contentAsString, fileContentAsString);
}

bool ZipTest::containsFile(const std::string & zipFileName, const std::string & fileName) {
	Unzip unzip;

	if(! unzip.open(zipFileName)){
		return false;
	}

	if(! unzip.containsFile(fileName)){
		return false;
	}

	if(! unzip.isFile(fileName)){
		return false;
	}

	return true;
}

bool ZipTest::containsFolder(const std::string & zipFileName, const std::string & folderName) {
	Unzip unzip;

	if(! unzip.open(zipFileName)){
		return false;
	}

	std::string folderNameWithTrailingSlash = folderName;
	if(! boost::algorithm::ends_with(folderNameWithTrailingSlash, "/")){
		folderNameWithTrailingSlash += "/";
	}

	if(! unzip.containsFile(folderNameWithTrailingSlash)){
		return false;
	}

	if(! unzip.isFolder(folderNameWithTrailingSlash)){
		return false;
	}

	return true;
}

int ZipTest::numFilesInZip(const std::string & zipFileName) {
	Unzip unzip;

	if(! unzip.open(zipFileName)){
		return 0;
	}

	int numFilesFromZip = unzip.getNumFiles();
	unzip.close();

	return numFilesFromZip;
}

void ZipTest::createFolder(const std::string & folderName) {
	boost::filesystem::create_directory(folderName);
}

void ZipTest::createFile(const std::string & fileName, const std::string & content)
{
	std::ofstream fileStream(fileName);
	fileStream << content;
}

void ZipTest::copyFile(const std::string& src, const std::string& dest) {
	//workaround, because boost::filesystem::copy_file don't link
	//because there are some missing symbols...

#ifdef WIN32
	boost::filesystem::copy_file(src, dest);
#else
	std::string cmd = "cp " + src + " " + dest;
	system(cmd.c_str());
#endif
}

std::vector<unsigned char> ZipTest::getFileContent(const std::string & zipFile, const std::string & fileName, const std::string & password)
{
	Unzip unzip;
	std::vector<unsigned char> result;
	if(unzip.open(zipFile, password)){
		result = unzip.getFileContent(fileName);
	}
	unzip.close();
	return result;
}

} //cppzip
