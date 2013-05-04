/*
 * ZipPrivate.h -- IO on .zip files using minizip and zlib
 * Version 0.3.0.1, 04.05.2013
 * part of the CppZip project - (https://github.com/flo2k/CppZip)
 *
 * Created: 12.12.2012, Florian Künzner
 *
 * Copyright (C) 2012-2013 Florian Künzner and Andreas Bauer (CppZip) (https://github.com/flo2k/CppZip)
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

#ifndef ZIPPRIVATE_H_
#define ZIPPRIVATE_H_

#include <memory>

#include "ZipCommon.h"
#include "minizip/zip.h"

namespace cppzip{

	/*!
	 * \brief  Converts a InnerZipFileInfo to a zip_fileinfo.
	 *
	 * \param  innerZipFileInfo is the element to convert.
	 *
	 * \return the zip_fileinfo.
	 */
	zip_fileinfo convertInnerZipFileInfo_to_zipFileInfo(
			std::shared_ptr<InnerZipFileInfo> innerZipFileInfo);

	/*!
	 * \brief  Converts a zip_fileinfo to a zip_fileinfo.
	 *
	 * \param  zipInfo is the element to convert.
	 *
	 * \return the InnerZipFileInfo.
	 */
	std::shared_ptr<InnerZipFileInfo> convertInnerZipFileInfo_to_zipFileInfo(
			zip_fileinfo zipInfo);

} //cppzip


#endif /* ZIPPRIVATE_H_ */
