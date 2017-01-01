/*
 * ExportTester.h
 *
 *  Created on: 01.01.2017
 *      Author: flo
 */

#ifndef EXPORTTESTER_H_
#define EXPORTTESTER_H_

#include <string>

#include "CppZip_Global.h"

namespace cppzip {

class CPPZIP_SHARED_EXPORT ExportTester {
public:
	ExportTester();
	virtual ~ExportTester();

	std::string getString();

private:
	std::string test;
};

} //cppzip

#endif /* EXPORTTESTER_H_ */
