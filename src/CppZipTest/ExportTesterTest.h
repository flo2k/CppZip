/*
 * ExportTesterTest.h
 *
 *  Created on: 01.01.2017
 *      Author: flo
 */

#ifndef EXPORTTESTERTEST_H_
#define EXPORTTESTERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <memory>
#include <string>

namespace cppzip {

//forward declaration
class ExportTester;

class ExportTesterTest  : public CppUnit::TestFixture {

	CPPUNIT_TEST_SUITE(ExportTesterTest);
		CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

public:
	void test(void);

private:
	//std::shared_ptr<ExportTester> et;
};

} //cppzip

#endif /* EXPORTTESTERTEST_H_ */
