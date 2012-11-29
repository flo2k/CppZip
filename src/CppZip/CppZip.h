/*!
 * \mainpage CppZip is a c++ zip library.
 *
 * CppZip is a c++ zip library and is based on zlib and minizip. CppZip implemented with C++ standard (C++11)
 * and some funtions of the boost libraries.
 *
 * It's implementation focus is
 *  - on easy to use - a clear and well documented API
 *  - and to run on multiple platforms (Windows, Linux, ...).
 *
 * There are two classes in the framework:
 *
 * - Zip compresses and zips files and directories.
 * - Unzip uncompresses and unzips zip files.
 *
 * \author Florian Künzner
 * \version 0.2
 * \date 22.09.2012
 *
 * \note At the moment there are problems with special chars (ä, ö, ü, ß, ...):
 * When zipping and unzipping files with special chars inside zip files then
 * the file names doesn't will be created correct when unzip.
 *
 * More informations:
 * - \subpage getting_started
 * - \subpage zip_example
 * - \subpage unzip_example
 *
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
