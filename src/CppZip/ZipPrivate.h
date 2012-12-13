/*
 * ZipPrivate.h
 *
 *  Created on: 12.12.2012
 *      Author: flo
 */

#ifndef ZIPPRIVATE_H_
#define ZIPPRIVATE_H_

#include <memory>

#include "ZipCommon.h"
#include "zip.h"

namespace cppzip{

	zip_fileinfo convertInnerZipFileInfo_to_zipFileInfo(std::shared_ptr<InnerZipFileInfo> innerZipFileInfo);
	std::shared_ptr<InnerZipFileInfo> convertInnerZipFileInfo_to_zipFileInfo(zip_fileinfo zipInfo);

} //cppzip


#endif /* ZIPPRIVATE_H_ */
