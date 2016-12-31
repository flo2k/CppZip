#ifndef CPPZIP_GLOBAL_H
#define CPPZIP_GLOBAL_H

#if defined(_MSC_VER)
    #define DECL_EXPORT __declspec(dllexport)
    #define DECL_IMPORT __declspec(dllimport)
#else
    #define DECL_EXPORT
    #define DECL_IMPORT
#endif

#if defined(CPPZIP_LIBRARY)
#  define CPPZIP_SHARED_EXPORT DECL_EXPORT
#else
#  define CPPZIP_SHARED_EXPORT DECL_IMPORT
#endif

#endif //CPPZIP_GLOBAL_H
