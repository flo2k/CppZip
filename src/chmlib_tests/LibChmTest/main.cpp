#include <iostream>
#include <fstream>
#include <cstring>

#include <experimental/filesystem>
#include <experimental/system_error>

using namespace std;

// https://github.com/jedwing/CHMLib

#define CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE 1000

#include <chm_lib.h>

int print_info_enumerator(struct chmFile* chmFile,
                          struct chmUnitInfo* unitInfo,
                          void* context);

int extract_enumerator(struct chmFile* chmFile,
                       struct chmUnitInfo* unitInfo,
                       void* context);

struct extract_context
{
    const char *base_path;
};


int main(int argc, char *argv[])
{

    //open
    chmFile* chm = chm_open("/data/projects/Projekte/Repositories/cppzip.zip/src/chmlib_tests/LibChmTest/TPOG.chm");
    if(chm == nullptr){
        std::cerr << "Can't open chm file" << std::endl;
    }

    if(chm){
        std::cout << "chm opened" << std::endl;
    }

    struct extract_context extractContext;
    //int enum_result = chm_enumerate(chm, CHM_ENUMERATE_ALL, print_info_enumerator, (void*)&extractContext);
    int enum_result = chm_enumerate(chm, CHM_ENUMERATE_ALL, print_info_enumerator, nullptr);
    if(enum_result == CHM_ENUMERATOR_FAILURE){
        std::cerr << "Print info failure" << std::endl;
    }

    enum_result = chm_enumerate(chm, CHM_ENUMERATE_ALL, extract_enumerator, nullptr);
    if(enum_result == CHM_ENUMERATOR_FAILURE){
        std::cerr << "Extract failure" << std::endl;
    }

    //close
    chm_close(chm);

    return 0;
}

int print_info_enumerator(struct chmFile* chmFile,
                          struct chmUnitInfo* unitInfo,
                          void* context)
{
    std::string path = unitInfo->path;

    //check paths
    if(!path.empty() && path.front() != '/'){ //files and folders starts with /
        return CHM_ENUMERATOR_CONTINUE;
    }

    if(path.find("/../") != std::string::npos) { //security hole, dangerous paths
        return CHM_ENUMERATOR_CONTINUE;
    }

    //print
    std::cout << path << std::endl;

    return CHM_ENUMERATOR_CONTINUE;
}

int extract_enumerator(struct chmFile* chmFile,
                       struct chmUnitInfo* unitInfo,
                       void* context)
{
    std::string path = unitInfo->path;

    //check paths
    if(!path.empty() && path.front() != '/'){ //files and folders starts with /
        return CHM_ENUMERATOR_CONTINUE;
    }

    if(path.find("/../") != std::string::npos) { //security hole, dangerous paths
        return CHM_ENUMERATOR_CONTINUE;
    }

    //resulting path
    path = "/data/projects/Projekte/Repositories/cppzip.zip/src/chmlib_tests/LibChmTest/xx" + path;

    //extract
    bool isFile = !path.empty() && path.back() != '/';
    if(isFile){
        LONGUINT64 fileLengthRemain = unitInfo->length;
        LONGUINT64 offset = 0;
        LONGUINT64 len = 0;

        std::vector<unsigned char> fileContent;
        unsigned char buffer[CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE];

        while(fileLengthRemain > 0){
            len = chm_retrieve_object(chmFile, unitInfo, buffer, offset, CPPZIP_UNZIP_CHAR_ARRAY_BUFFER_SIZE);

            for(LONGUINT64 i = 0; i < len; ++i){
                fileContent.push_back(buffer[i]);
            }

            offset += len;
            fileLengthRemain -= len;

        }

        //std::ofstream outfile(path.c_str());
        std::ofstream outfile(path, std::ios::out | std::ios::binary);
        for (const auto& c: fileContent) {
            outfile << c;
        }
        outfile.close();
    } else {
        std::error_code error_code;
        std::experimental::filesystem::create_directories(path, error_code);
    }


    return CHM_ENUMERATOR_CONTINUE;
}
