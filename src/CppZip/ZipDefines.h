/*!
 * \file ZipDefines.h
 *
 * Zip.h -- IO on .zip files using minizip and zlib
 * Version 1.0.1.3, 02.12.2017
 * part of the CppZip project - (https://github.com/flo2k/CppZip)
 *
 * Created: 02.01.2017, Florian Künzner
 *
 * Copyright (C) 2014-2017 Florian Künzner (CppZip) (https://github.com/flo2k/CppZip)
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

#ifndef ZIPDEFINES_H_
#define ZIPDEFINES_H_

#include "CppZip_Global.h"

namespace cppzip {

/*! \brief Open flags */
class CPPZIP_SHARED_EXPORT OpenFlags {
public:
    /*! \brief Open flags */
    enum Flags {
        CreateAndOverwrite    //!< \brief Creates an new zip file, if exists overwrites.
       ,OpenExisting        //!< \brief Opens an existing zip file
    };
};

} //cppzip

#endif /* ZIPDEFINES_H_ */
