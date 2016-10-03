/*!
 * ZipTest.h -- Tests Zip.h from CppZip
 * Version 0.1, 12.12.2012
 * part of the CppZip project - (https://github.com/flo2k/CppZip)
 *
 * Created: 12.12.2011, Florian Künzner
 *
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

#ifndef CPPZIP_PERFORMANCETESTS_H_
#define CPPZIP_PERFORMANCETESTS_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <memory>
#include <string>

namespace cppzip {

//forward declaration
class Zip;
class Unzip;

class PerformanceTests : public CppUnit::TestFixture {

	CPPUNIT_TEST_SUITE(PerformanceTests);
		CPPUNIT_TEST(testZip_addMidSizedFiles);
		CPPUNIT_TEST(testZip_replaceMidSizedFiles);
		CPPUNIT_TEST(testZip_deleteMidSizedFile);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

public:
	void testZip_addMidSizedFiles(void);
	void testZip_replaceMidSizedFiles(void);
	void testZip_deleteMidSizedFile(void);

private:
	std::shared_ptr<Zip> zip;
	std::shared_ptr<Unzip> unzip;

	std::string testZipsFolder;

	std::string midSizeFile1;
	std::string midSizeFile2;
	std::string midSizeFile3;
	std::string midSizeFile4;

	std::string dataFolder;
	std::string midSizedFilesFolder;
	std::string tempFolder;
	std::string zipFile;
	std::string zipFileWithMidSizedFiles;
};

} //cppzip

#endif /* CPPZIP_PERFORMANCETESTS_H_ */
