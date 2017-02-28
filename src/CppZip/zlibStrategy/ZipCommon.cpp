#include "ZipCommon.h"

namespace cppzip {

InnerZipFileInfo::InnerZipFileInfo()
{
    time_sec = 0;
    time_min = 0;
    time_hour = 0;
    time_day_of_month = 0;
    time_month = 0;
    time_year = 0;
    dosDate = 0;
    crc = 0;
    compressed_size = 0;
    uncompressed_size = 0;
    internal_fileAttributes = 0;
    external_fileAttributes = 0;
}

} //cppzip
