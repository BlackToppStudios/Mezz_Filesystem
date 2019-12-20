// � Copyright 2010 - 2019 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef Mezz_Filesystem_SystemPathUtilities_h
#define Mezz_Filesystem_SystemPathUtilities_h

#ifndef SWIG
    #include "DataTypes.h"
#endif

namespace Mezzanine {
namespace Filesystem {
    ///////////////////////////////////////////////////////////////////////////////
    // System Path Utilities

    /// @brief Gets the PATH system variable split into a String per path.
    /// @param PATH The path String to be split. Defaults to the system PATH variable from getenv.
    /// @return Returns a vector of paths split from PATH.
    [[nodiscard]]
    StringVector MEZZ_LIB GetSystemPATH(const StringView PATH = getenv("PATH"));

    /// @brief Gets which executable will be invoked if an executable is invoked from the command line.
    /// @warning This function is case sensitive and not all operating systems are.
    /// @param ExecutableName The name of the executable to look for.
    /// @return Returns an empty String if the exe isn't found, or the first valid path to the exe if it is.
    [[nodiscard]]
    String MEZZ_LIB Which(const StringView ExecutableName);
}//Filesystem
}//Mezzanine

#endif
