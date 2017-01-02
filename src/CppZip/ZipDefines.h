/*
 * OpenFlags.h
 *
 *  Created on: 02.01.2017
 *      Author: flo
 */

#ifndef ZIPDEFINES_H_
#define ZIPDEFINES_H_

#include "CppZip_Global.h"

namespace cppzip {

class CPPZIP_SHARED_EXPORT OpenFlags {
public:
	/*! \brief Open flags */
	enum Flags {
		CreateAndOverwrite	//!< \brief Creates an new zip file, if exists overwrites.
	   ,OpenExisting		//!< \brief Opens an existing zip file
	};
};

} //cppzip

#endif /* ZIPDEFINES_H_ */
