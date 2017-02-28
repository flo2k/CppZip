#include "ZipPrivate.h"

namespace cppzip {

zip_fileinfo convertInnerZipFileInfo_to_zipFileInfo(std::shared_ptr<InnerZipFileInfo> innerZipFileInfo)
{
    zip_fileinfo zipFileInfo;

    //time stuff
    zipFileInfo.tmz_date.tm_year = innerZipFileInfo->time_year;
    zipFileInfo.tmz_date.tm_mon  = innerZipFileInfo->time_month;
    zipFileInfo.tmz_date.tm_mday = innerZipFileInfo->time_day_of_month;
    zipFileInfo.tmz_date.tm_hour = innerZipFileInfo->time_hour;
    zipFileInfo.tmz_date.tm_min  = innerZipFileInfo->time_min;
    zipFileInfo.tmz_date.tm_sec  = innerZipFileInfo->time_sec;
    zipFileInfo.dosDate = 0;

    //file attributes
    zipFileInfo.internal_fa = innerZipFileInfo->internal_fileAttributes;
    zipFileInfo.external_fa = innerZipFileInfo->external_fileAttributes;

    return zipFileInfo;
}

//std::shared_ptr<InnerZipFileInfo> convertInnerZipFileInfo_to_zipFileInfo(zip_fileinfo zipInfo)
//{
//	std::shared_ptr<InnerZipFileInfo> innerZipFileInfo;
//
//	return innerZipFileInfo;
//}

const char* formatPassword(const std::string& password)
{
    return password.empty() ? nullptr : password.c_str();
}

} //cppzip
