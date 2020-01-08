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
#ifndef Mezz_Filesystem_SpecialPathUtilities_h
#define Mezz_Filesystem_SpecialPathUtilities_h

#ifndef SWIG
    #include "DataTypes.h"
#endif

namespace Mezzanine {
namespace Filesystem {
    ///////////////////////////////////////////////////////////////////////////////
    // MainArg Utilities

    /// @brief Stores the main arguments for later use.
    /// @param ArgCount The number of elements in the ArgVars buffer.
    /// @param ArgVars A pointer to an array of c-strings containing arguments used to launch an executable.
    void MEZZ_LIB CacheMainArgs(int ArgCount, char** ArgVars);

    ///////////////////////////////////////////////////////////////////////////////
    // Execution Directory Utilities

    /// @brief Get the Path to the current executable, in a fast way if possible.
    /// @note This will attempt to use the main args cached with @ref CacheMainArgs. If that fails it will get the
    /// executable directory from the system instead.
    /// @return Returns a String containing the path to the running executable, or an empty String on failure..
    [[nodiscard]]
    String MEZZ_LIB GetExecutableDir();
    /// @brief Get the Path to the current executable, in a fast way if possible.
    /// @note If this fails to detect the executable directory from the parameters it will get the
    /// executable directory from the system instead. @n @n
    /// Not all platforms provide all the needed information to determine the executable directory.
    /// @warning If you pass bogus arguments to this bad things can and will happen (Infinite loops, segfaults, etc.).
    /// Ideally, just pass in what main gives you.
    /// @param ArgCount How many arguments are in ArgVars.
    /// @param ArgVars A pointer to an array of c-strings containing arguments.
    /// @return Returns a String containing the path to the running executable, or an empty String on failure..
    [[nodiscard]]
    String MEZZ_LIB GetExecutableDir(int ArgCount, char** ArgVars);

    /// @brief Uses a system call to get the current Directory the executable is in.
    /// @note This makes an external system call and is likely slower than GetExecutableDirFromArg
    /// @return Returns the path to the running executable, or an empty String on failure..
    [[nodiscard]]
    String MEZZ_LIB GetExecutableDirFromSystem();

    /// @brief Get the Path to the current executable.
    /// @note This uses the main args cached with @ref CacheMainArgs.
    /// @return Returns a String containing the path to the running executable, or an empty String on failure.
    [[nodiscard]]
    String MEZZ_LIB GetExecutableDirFromArg();
    /// @brief Get the Path to the current executable from provided parameters.
    /// @note Not all platforms provide all the needed information to determine the executable directory.
    /// @warning If you pass bogus arguments to this bad things can and will happen (Infinite loops, segfaults, etc.).
    /// Ideally, just pass in what main gives you.
    /// @param ArgCount How many arguments are in ArgVars.
    /// @param ArgVars A pointer to an array of c-strings containing arguments.
    /// @return Returns a String containing the path to the running executable, or an empty String on failure.
    [[nodiscard]]
    String MEZZ_LIB GetExecutableDirFromArg(int ArgCount, char** ArgVars);

    ///////////////////////////////////////////////////////////////////////////////
    // Working Directory Utilities

    /// @brief Changes the directory used as a basis for system commands.
    /// @exception If there is a system error while changing directory, an std::runtime_error is thrown.
    /// @param ChangeTo The new directory to work from.
    void MEZZ_LIB ChangeWorkingDirectory(const StringView ChangeTo);
    /// @brief Gets the current directory used as a basis for system commands.
    /// @return Returns a String containing the current working directory of the running executable.
    [[nodiscard]]
    String MEZZ_LIB GetWorkingDirectory();

    ///////////////////////////////////////////////////////////////////////////////
    // Application Data Directory Utilities

    /// @brief Gets the path to the directory intended for game and engine config data that is not meant to be shared.
    /// @note This is mostly a function for the benefit of Windows Users, as most Posix systems lack the notion of a
    /// system enforced AppData directory. Calling this function on Posix will result in the users home directory
    /// being returned.
    /// @return Returns a String path to the Local AppData Directory, or an empty String if there was an error.
    [[nodiscard]]
    String MEZZ_LIB GetLocalAppDataDir();
    /// @brief Gets the path to the directory intended for game and engine config data that is allowed to be shared.
    /// @note This is mostly a function for the benefit of Windows Users, as most Posix systems lack the notion of a
    /// system enforced AppData directory. Calling this function on Posix will result in the users home directory
    /// being returned.
    /// @return Returns a String path to the Sharable AppData Directory, or an empty String if there was an error.
    [[nodiscard]]
    String MEZZ_LIB GetShareableAppDataDir();
    /// @brief Gets the path to the directory intended for game saves and user profile data for the current user.
    /// @note This is mostly a function for the benefit of Windows Users, as most Posix systems lack the notion of a
    /// system enforced AppData directory. Calling this function on Posix will result in the users home directory
    /// being returned.
    /// @return Returns a String path to the Current User Data Directory, or an empty String if there was an error.
    [[nodiscard]]
    String MEZZ_LIB GetCurrentUserDataDir();
    /// @brief Gets the path to the directory intended for game saves and user profile data for all users.
    /// @note This is mostly a function for the benefit of Windows Users, as most Posix systems lack the notion of a
    /// system enforced AppData directory. Calling this function on Posix will result in the users home directory
    /// being returned.
    /// @return Returns a String path to the Common User Data Directory, or an empty String if there was an error.
    [[nodiscard]]
    String MEZZ_LIB GetCommonUserDataDir();
}//Filesystem
}//Mezzanine

#endif
