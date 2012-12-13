/*
 * PerformanceTests.h
 *
 *  Created on: 12.12.2012
 *      Author: flo
 */

#ifndef PERFORMANCETESTS_H_
#define PERFORMANCETESTS_H_

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
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

public:
	void testZip_addMidSizedFiles(void);
	void testZip_replaceMidSizedFiles(void);

private:
	std::shared_ptr<Zip> zip;
	std::shared_ptr<Unzip> unzip;

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

#endif /* PERFORMANCETESTS_H_ */
