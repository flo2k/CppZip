#include <iostream>
#include <string>
#include <stdexcept>
#include <time.h>

#include <boost/timer/timer.hpp>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/TestCaller.h>

//outputter
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TextOutputter.h>

//listener
#include <cppunit/TestListener.h>
#include <cppunit/TextTestProgressListener.h>
#include <cppunit/BriefTestProgressListener.h>

#include "ZipTest.h"
#include "UnzipTest.h"
#include "PerformanceTests.h"

class TimingListener: public CppUnit::TestListener {
public:
	void startTest(CppUnit::Test *test) {
		wasTestOk = true;

		//Output
		std::cout << test->getName();

		//prepare the timer
		timer.start();
	}

	void endTest(CppUnit::Test *test) {

		timer.stop();
		boost::timer::cpu_times ellapsed = timer.elapsed();
		double ellapsedMilliSeconds = ellapsed.wall;
		//double ellapsedMilliSeconds = ellapsed.system + ellapsed.user;
		ellapsedMilliSeconds /= 1000.0;
		ellapsedMilliSeconds /= 1000.0;

		std::string resultAsString;

		if(wasTestOk){
			resultAsString = "OK";
		} else {
			resultAsString = "ERROR";
		}

		std::cout << " : " << resultAsString << " (" << ellapsedMilliSeconds << " ms)" << std::endl;
	}

	virtual void addFailure(const CppUnit::TestFailure & failure) {
		wasTestOk = false;
	}

	// ... (interface to add/read test timing result)

private:
	boost::timer::cpu_timer timer;
	bool wasTestOk;
};

int RunAllTests(void) {
	// Get the top level suite from the registry
	CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

	CppUnit::TestResult result;

	// Add a listener that collects test result
	CppUnit::TestResultCollector resultCollector;
	result.addListener(&resultCollector);

	TimingListener timeListener;
	result.addListener(&timeListener);

	// Adds the test to the list of test to run
	CppUnit::TextTestRunner runner;
	runner.addTest(suite);

	// Run the tests.
	runner.run(result);

	// Output the result with the CompilerOutputter
	CppUnit::CompilerOutputter(&resultCollector, std::cerr).write();

	bool wasSucessful = resultCollector.wasSuccessful();

	// Return error code 1 if the one of test failed.
	return wasSucessful ? 0 : 1;
}

int RunPerformanceTests(void) {
	// Get the top level suite from the registry

	CppUnit::TestSuite * suite = new CppUnit::TestSuite();

	suite->addTest(new CppUnit::TestCaller<cppzip::PerformanceTests>("a", &cppzip::PerformanceTests::testZip_addMidSizedFiles));
	suite->addTest(new CppUnit::TestCaller<cppzip::PerformanceTests>("b", &cppzip::PerformanceTests::testZip_replaceMidSizedFiles));

	CppUnit::TestResult result;

	// Add a listener that colllects test result
	CppUnit::TestResultCollector resultCollector;
	result.addListener(&resultCollector);

	TimingListener timeListener;
	result.addListener(&timeListener);

	// Adds the test to the list of test to run
	CppUnit::TextTestRunner runner;
	runner.addTest(suite);

	// Run the tests.
	runner.run(result);

	// Output the result with the CompilerOutputter
	CppUnit::CompilerOutputter(&resultCollector, std::cerr).write();

	bool wasSucessful = resultCollector.wasSuccessful();

	// Return error code 1 if the one of test failed.
	return wasSucessful ? 0 : 1;
}

int main(void) {
	std::cout << "starting tests..." << std::endl;

	int ok = RunAllTests();
//	int ok = RunPerformanceTests();

	return ok;
}
