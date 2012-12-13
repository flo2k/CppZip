/*!
 * \mainpage CppZip is a c++ zip library.
 *
 * CppZip is an open source c++ zip library and is based on zlib and minizip. CppZip is implemented with standard C++ (C++11)
 * and some funtions of the boost libraries.
 *
 * It's implementation focus is
 *  - on easy to use - a clear and well documented API
 *  - and to run on multiple platforms (Windows, Linux, ...).
 *
 * There are two classes in the library:
 *
 * - \ref cppzip::Zip "Zip" compresses and zips files and folders.
 *  - Adding files from memory
 *  - Adding files from file system
 *  - Adding complete (and recursive) folders from file system
 *  - Delete files
 *  - Replace files
 * - \ref cppzip::Unzip "Unzip" uncompresses and unzips zip files.
 *  - Inspect files inside a zip (e.g. file list)
 *  - Extract to file system
 *  - Extract to memory
 *
 * \author Florian Künzner and Andreas Bauer
 * \version 0.2
 * \date 13.12.2012
 *
 * \note At the moment there are problems with special chars (ä, ö, ü, ß, ...):
 * When zipping and unzipping files with special chars inside zip files then
 * the file names doesn't will be created correct when unzip.
 *
 * More informations:
 * - \subpage getting_started
 * - \subpage zip_example
 * - \subpage unzip_example
 * - \subpage license
 *
 * \remark Thanks to all the people who have spent a lot of time to implement these libs, where CppZip is based on:
 *  - zlib
 *  - minizip
 *  - boost
 *  - c++ stl
 *  - c++11
 */

/*!
 * \page getting_started Getting started
 * TODO: add some getting started examples:
 * - describe the dependencies: zlib, minizip, boost
 * - how to compile
 * - how to use as shared library
 * - how to use as static library
 * - how to use as dll
*/

/*!
 * \page zip_example Zip examples
 *
 * \section add_a_single_file Add a single file into a zip:
 * \code
 *  //open zip file
 *  Zip zip;
 *  zip->open("file.zip");
 *
 *  //adds a file
 *  zip->addFile("filename.txt");
 *
 *  //close
 *  zip->close();
 * \endcode
 *
 * \section add_a_file_from_memory Adds a content from memory into a file inside the zip:
 * \code
 *  //open zip file
 *  Zip zip;
 *  zip->open("file.zip");
 *
 *  //prepare content
 *  std::string aString("this is a string");
 *  std::vector<unsigned char> content;
 *  content.insert(content.end(), aString.begin(), aString.end());
 *
 *  //add content
 *  zip->addFile("filename.txt", content);
 *
 *  //close
 *  zip->close();
 * \endcode
*/

/*!
 * \page unzip_example Unzip examples
 *
 * \section extracts_into_memory Extracts a file from zip into memory:
 * \code
 *  //open zip file
 *  Unzip zip;
 *  zip.open("file.zip");
 *
 *  //retrieve content
 *  std::vector<unsigned char> content = zip->getFileContent("filename.txt");
 *
 *  //convert to a std::string and print it to console
 *  std::string contantAsString(content.begin(), content.end());
 *  std::cout << "content: " << contantAsString << std::endl;
 *
 *  //close zip
 *  zip.close();
 * \endcode
 *
 * \section extracts_into_file Extracts a single file from zip into file system:
 * \code
 *  //open zip file
 *  Unzip zip;
 *  zip.open("file.zip");
 *
 *  //extract file
 *  zip->extractFileTo("fileInsideZip.txt", "pathOnFileSystem.txt");
 *
 *  //close zip
 *  zip.close();
 * \endcode
 *
 * \section extracts_all Extracts all inside a zip to file system:
 * \code
 *  //open zip file
 *  Unzip zip;
 *  zip.open("file.zip");
 *
 *  //extract file
 *  zip->extractAllTo("path/on/fileSytem");
 *
 *  //close zip
 *  zip.close();
 * \endcode
*/

/*!
 * \page license License
 *
 * CppZip -- IO on .zip files using minizip and zlib\n
 * Version 0.2, 13.12.2012\n
 * part of the CppZip project - (https://github.com/flo2k/CppZip)\n
 *
 * Copyright (C) 2012 Florian Künzner and Andreas Bauer (CppZip) (https://github.com/flo2k/CppZip)\n
 *
 * ---------------------------------------------------------------------------
 *
 * Condition of use and distribution are the same than minizip and zlib :\n
 *
 * This software is provided 'as-is', without any express or implied\n
 * warranty. In no event will the authors be held liable for any damages\n
 * arising from the use of this software.\n
 *
 * Permission is granted to anyone to use this software for any purpose,\n
 * including commercial applications, and to alter it and redistribute it\n
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not\n
 *    claim that you wrote the original software. If you use this software\n
 *    in a product, an acknowledgment in the product documentation would be\n
 *    appreciated but is not required.\n
 * 2. Altered source versions must be plainly marked as such, and must not be\n
 *    misrepresented as being the original software.\n
 * 3. This notice may not be removed or altered from any source distribution.\n
 *
 * ---------------------------------------------------------------------------
 *
 */
