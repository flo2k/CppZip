/*!
 * ZipCommon.h -- IO on .zip files using minizip and zlib
 * Version 0.1, 06.12.2012
 * part of the CppZip project - (https://github.com/flo2k/CppZip)
 *
 * Created: 06.12.2012, Florian Künzner
 *
 * Copyright (C) 2011-2012 Florian Künzner (CppZip) (https://github.com/flo2k/CppZip)
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

#ifndef ZIPCOMMON_H_
#define ZIPCOMMON_H_

#include <string>

namespace cppzip {

/*!
 * InnerZipFileInfo contains all Infos to save a file into zip.
 * The informations are very similar to zip_fileinfo from zip.h
 */
struct InnerZipFileInfo{
	std::string fileName;
	std::string extraField;
	std::string comment;
	unsigned int time_sec;            /*! seconds after the minute - [0,59] */
	unsigned int time_min;            /*! minutes after the hour - [0,59] */
	unsigned int time_hour;           /*! hours since midnight - [0,23] */
	unsigned int time_day_of_month;   /*! day of the month - [1,31] */
	unsigned int time_month;          /*! months since January - [0,11] */
	unsigned int time_year;           /*! years - [1980..2044] */
	unsigned long dosDate;            /*! if dos_date == 0, tmu_date is used */
	unsigned long internal_fileAttributes;
	unsigned long external_fileAttributes;
};

} //cppzip

#endif /* ZIPCOMMON_H_ */
