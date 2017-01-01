/*
 * ExportTesterTest.cpp
 *
 *  Created on: 01.01.2017
 *      Author: flo
 */

#include "ExportTesterTest.h"

#include <ExportTester.h>

namespace cppzip {

CPPUNIT_TEST_SUITE_REGISTRATION(ExportTesterTest);

void ExportTesterTest::setUp(void)
{
	et = std::shared_ptr<ExportTester>(new ExportTester());
}

void ExportTesterTest::tearDown(void)
{

}

void ExportTesterTest::test(void)
{

}

} //cppzip
