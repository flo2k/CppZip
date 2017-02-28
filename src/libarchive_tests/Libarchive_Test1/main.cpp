#include <iostream>

#include </usr/include/archive.h>
#include </usr/include/archive_entry.h>


#include <clocale>


using namespace std;

int main(int argc, char *argv[])
{
    std::setlocale(LC_ALL, "de_DE.UTF-8");

    cout << "libarchive _ version: " << archive_version_string() << endl;
    cout << "libarchive _ details: " << archive_version_details() << endl;

    cout << "Hello World!" << endl;

    struct archive *a = nullptr;
    struct archive_entry *entry;

    a = archive_read_new();

    //enable support
    archive_read_support_format_all(a);

    archive_read_support_filter_bzip2(a);
    archive_read_support_filter_gzip(a);
    archive_read_support_filter_compress(a);
    //archive_read_support_format_tar(a);
    //archive_read_support_format_cpio(a);


    //open archive
    int r = 0;

    std::string filename = "/data/projects/Projekte/Repositories/cppzip.zip/src/libarchive_tests/Libarchive_Test1/test.tar.gz";
    //std::string filename = "/data/projects/Projekte/Repositories/cppzip.zip/src/libarchive_tests/Libarchive_Test1/test.zip";
    //std::string filename = "/data/projects/Projekte/Repositories/cppzip.zip/src/libarchive_tests/Libarchive_Test1/TPOG.chm";

    if ((r = archive_read_open_filename(a, filename.c_str(), 10240))) {
        cout << "error: " << archive_error_string(a) << endl;
        exit(r);
    }


    for (;;) {
        int needcr = 0;
        r = archive_read_next_header(a, &entry);

        if (r == ARCHIVE_EOF){
            break;
        }
        if (r != ARCHIVE_OK) {
            cout << "error: " << archive_error_string(a) << endl;
            exit(1);
        }
        //cout << "file: " << archive_entry_pathname_utf8(entry) << endl;
        cout << "file: " << archive_entry_pathname(entry) << endl;

//        if (verbose && do_extract)
//            msg("x ");
//        if (verbose || !do_extract) {
//            msg(archive_entry_pathname(entry));
//            msg(" ");
//            needcr = 1;
//        }
//        if (do_extract) {
//            r = archive_write_header(ext, entry);
//            if (r != ARCHIVE_OK) {
//                errmsg(archive_error_string(a));
//                needcr = 1;
//            }
//            else {
//                r = copy_data(a, ext);
//                if (r != ARCHIVE_OK)
//                    needcr = 1;
//            }
//        }
//        if (needcr)
//            msg("\n");
    }

    archive_read_close(a);
    archive_read_free(a);
    a = nullptr;

    return 0;
}
