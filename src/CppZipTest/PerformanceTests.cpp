/*
 * PerformanceTests.cpp
 *
 *  Created on: 12.12.2012
 *      Author: flo
 */

#include "PerformanceTests.h"
#include <Zip.h>
#include <Unzip.h>
#include <cppunit/extensions/HelperMacros.h>

#include <boost/filesystem.hpp>

namespace cppzip {

CPPUNIT_TEST_SUITE_REGISTRATION(PerformanceTests);

void PerformanceTests::setUp(void) {
	zipFile = "test.zip";
	zipFileWithMidSizedFiles = "zipWithMidSizedFiles.zip";
	tempFolder = "temp";
	dataFolder = "data";
	midSizedFilesFolder = dataFolder + "/midSizedFiles";
	midSizeFile1 = "lorem1.txt";
	midSizeFile2 = "lorem2.txt";
	//midSizeFile3 = "eclipse-cpp-juno-SR1-linux-gtk-x86_64.tar.gz";
	//midSizeFile4 = "linuxmint-13-mate-dvd-64bit.iso";

	zip = std::shared_ptr<Zip>(new Zip());
	unzip = std::shared_ptr<Unzip>(new Unzip());

	boost::filesystem::remove_all(tempFolder);
}

void PerformanceTests::tearDown(void)
{
	zip->close();
	unzip->close();
	boost::filesystem::remove_all(tempFolder);
}

void PerformanceTests::testZip_addMidSizedFiles(void) {
	bool ok = zip->open(tempFolder + "/" + zipFile);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("open", true, ok);

	ok = zip->addFile(midSizedFilesFolder + "/" + midSizeFile1, false);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("add file 1", true, ok);

	ok = zip->addFile(midSizedFilesFolder + "/" + midSizeFile2, false);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("add file2", true, ok);

	//ok = zip->addFile(midSizedFilesFolder + "/" + midSizeFile3, false);
	//ok = zip->addFile(midSizedFilesFolder + "/" + midSizeFile4, false);
	ok = zip->close();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("close", true, ok);
}

void PerformanceTests::testZip_replaceMidSizedFiles(void)
{
	testZip_addMidSizedFiles();

	bool ok = zip->open(tempFolder + "/" + zipFile, Zip::OpenExisting);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("open", true, ok);

	ok = zip->replaceFile(midSizedFilesFolder + "/" + midSizeFile2, midSizedFilesFolder + "/" + midSizeFile2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("replace", true, ok);

	ok = zip->close();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("close", true, ok);
}

void PerformanceTests::testZip_deleteMidSizedFile(void) {
	testZip_addMidSizedFiles();

	bool ok = zip->open(tempFolder + "/" + zipFile, Zip::OpenExisting);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("open", true, ok);

	ok = zip->deleteFile(midSizedFilesFolder + "/" + midSizeFile2);
	CPPUNIT_ASSERT_EQUAL_MESSAGE("replace", true, ok);

	ok = zip->close();
	CPPUNIT_ASSERT_EQUAL_MESSAGE("close", true, ok);
}

} //cppzip
