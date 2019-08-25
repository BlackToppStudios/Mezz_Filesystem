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
#ifndef Mezz_Filesystem_PathUtilities_h
#define Mezz_Filesystem_PathUtilities_h

#ifndef SWIG
    #include "StringTools.h"
#endif

namespace Mezzanine {
namespace Filesystem {
    ///////////////////////////////////////////////////////////////////////////////
    // Dir and Base Name

    /// @brief Get the directory portion of a String.
    /// @param PathAndFile A complete path and filename.
    /// @return If passed "/a/b/c.txt" or "c:\windirs\crash.exe" this will return "/a/b/" or "c:\windirs\".
    String MEZZ_LIB GetDirName(const char* PathAndFile);
    /// @brief Get the directory portion of a String.
    /// @param PathAndFile A complete path and filename.
    /// @return If passed "/a/b/c.txt" or "c:\windirs\crash.exe" this will return "/a/b/" or "c:\windirs\".
    String MEZZ_LIB GetDirName(const StringView PathAndFile);
    /// @brief Get the filename portion of a String.
    /// @param PathAndFile A complete path and filename.
    /// @return If passed "/a/b/c.txt" or "c:\windirs\crash.exe" this will return "c.txt" or "crash.exe".
    String MEZZ_LIB GetBaseName(const char* PathAndFile);
    /// @brief Get the filename portion of a String.
    /// @param PathAndFile A complete path and filename.
    /// @return If passed "/a/b/c.txt" or "c:\windirs\crash.exe" this will return "c.txt" or "crash.exe".
    String MEZZ_LIB GetBaseName(const StringView PathAndFile);

    ///////////////////////////////////////////////////////////////////////////////
    // Separators

    /// @brief Gets the character used by posix to separate directories.
    /// @return Returns a forward slash '/'.
    template<typename CharType = char, typename = std::enable_if_t< StringTools::is_char<CharType>::value > >
    constexpr CharType GetDirectorySeparator_Posix()
        { return '/'; }
    /// @brief Gets the character used by windows to separate directories.
    /// @return Returns a backward slash '\\'.
    template<typename CharType = char, typename = std::enable_if_t< StringTools::is_char<CharType>::value > >
    constexpr CharType GetDirectorySeparator_Windows()
        { return '\\'; }
    /// @brief Get the character readable by all platforms to separate directories.
    /// @return Returns a forward slash '/'.
    template<typename CharType = char, typename = std::enable_if_t< StringTools::is_char<CharType>::value > >
    constexpr CharType GetDirectorySeparator_Universal()
        { return GetDirectorySeparator_Posix<CharType>(); }
    /// @brief Gets the character used by the client OS to separate directories.
    /// @return Returns a backslash '\\' on windows and forward slash '/' on other operating systems.
    template<typename CharType = char, typename = std::enable_if_t< StringTools::is_char<CharType>::value > >
    constexpr CharType GetDirectorySeparator_Host()
    {
    #ifdef MEZZ_Windows
        return GetDirectorySeparator_Windows<CharType>();
    #else
        return GetDirectorySeparator_Posix<CharType>();
    #endif
    }

    /// @brief Checks to see if a character is used by posix to separate directories.
    /// @param ToCheck The character to check.
    /// @return Returns true if the character specified is '/'.
    template<typename CharType, typename = std::enable_if_t< StringTools::is_char<CharType>::value > >
    constexpr Boole IsDirectorySeparator_Posix(const CharType ToCheck)
        { return ( ToCheck == GetDirectorySeparator_Posix<CharType>() ); }
    /// @brief Checks to see if a character is used by windows to separate directories.
    /// @param ToCheck The character to check.
    /// @return Returns true if the character specified is '\\'.
    template<typename CharType, typename = std::enable_if_t< StringTools::is_char<CharType>::value > >
    constexpr Boole IsDirectorySeparator_Windows(const CharType ToCheck)
        { return ( ToCheck == GetDirectorySeparator_Windows<CharType>() ); }
    /// @brief Checks to see if a character is used to separate directories.
    /// @param ToCheck The character to check.
    /// @return Returns true if the character specified is '\\' or '/', false otherwise.
    template<typename CharType, typename = std::enable_if_t< StringTools::is_char<CharType>::value > >
    constexpr Boole IsDirectorySeparator(const CharType ToCheck)
        { return (IsDirectorySeparator_Posix<CharType>(ToCheck) || IsDirectorySeparator_Windows<CharType>(ToCheck)); }
    /// @brief Checks to see if a character is used by the host OS to separate directories.
    /// @param ToCheck The character to check.
    /// @return Returns true if the character is '\\' on windows, or '/' on linux, false otherwise.
    template<typename CharType, typename = std::enable_if_t< StringTools::is_char<CharType>::value > >
    constexpr Boole IsDirectorySeparator_Host(const CharType ToCheck)
    {
    #ifdef MEZZ_Windows
        return ( ToCheck == GetDirectorySeparator_Windows<CharType>() );
    #else
        return ( ToCheck == GetDirectorySeparator_Posix<CharType>() );
    #endif
    }

    /// @brief Get the character used to separate entries in the system PATH on Posix.
    /// @return Returns a colon ':'.
    template<typename CharType = char, typename = std::enable_if_t< StringTools::is_char<CharType>::value > >
    constexpr CharType GetPathSeparator_Posix()
        { return ':'; }
    /// @brief Get the character used to separate entries in the system PATH on Windows.
    /// @return Returns a semicolon ';'.
    template<typename CharType = char, typename = std::enable_if_t< StringTools::is_char<CharType>::value > >
    constexpr CharType GetPathSeparator_Windows()
        { return ';'; }
    /// @brief Get the character used to separate entries in the system PATH on the host.
    /// @return Returns a semicolon ';' on windows and colon ':' on other operating systems.
    template<typename CharType = char, typename = std::enable_if_t< StringTools::is_char<CharType>::value > >
    constexpr CharType GetPathSeparator_Host()
    {
    #ifdef MEZZ_Windows
        return GetPathSeparator_Windows<CharType>();
    #else
        return GetPathSeparator_Posix<CharType>();
    #endif
    }

    /// @brief Checks to see if a character is used to separate entries in the posix system path.
    /// @param ToCheck The character to check.
    /// @return Returns true if the character specified is ':'.
    template<typename CharType, typename = std::enable_if_t< StringTools::is_char<CharType>::value > >
    constexpr Boole IsPathSeparator_Posix(const CharType ToCheck)
        { return ( ToCheck == GetPathSeparator_Posix<CharType>() ); }
    /// @brief Checks to see if a character is used to separate entries in the windows system path.
    /// @param ToCheck The character to check.
    /// @return Returns true if the character specified is ';'.
    template<typename CharType, typename = std::enable_if_t< StringTools::is_char<CharType>::value > >
    constexpr Boole IsPathSeparator_Windows(const CharType ToCheck)
        { return ( ToCheck == GetPathSeparator_Windows<CharType>() ); }
    /// @brief Checks to see if a character is used to separate entries in the host system path.
    /// @param ToCheck The character to check.
    /// @return Returns true if the character is ';' on windows, or ':' on posix, false otherwise.
    template<typename CharType, typename = std::enable_if_t< StringTools::is_char<CharType>::value > >
    constexpr Boole IsPathSeparator_Host(const CharType ToCheck)
    {
    #ifdef MEZZ_Windows
        return ( ToCheck == GetPathSeparator_Windows<CharType>() );
    #else
        return ( ToCheck == GetPathSeparator_Posix<CharType>() );
    #endif
    }
    /// @brief Checks to see if a character is used to separate entries in the system path.
    /// @param ToCheck The character to check.
    /// @return Returns true if the character specified is ';' or ':'.
    template<typename CharType, typename = std::enable_if_t< StringTools::is_char<CharType>::value > >
    constexpr Boole IsPathSeparator(const CharType ToCheck)
        { return ( IsPathSeparator_Posix<CharType>(ToCheck) || IsPathSeparator_Windows<CharType>(ToCheck) ); }

    ///////////////////////////////////////////////////////////////////////////////
    // Absolute and Relative Paths

    /// @brief Gets whether or not a path is absolute.
    /// @details A path is absolute if it defines an explicit location of a resource. @n @n
    /// This will check if the path is absolute on either windows or posix platforms.
    /// @param ToCheck The path to check.
    /// @return Returns true if the specified path is absolute, false otherwise.
    Boole MEZZ_LIB IsPathAbsolute(const StringView ToCheck);
    /// @brief Gets whether or not a path is absolute on the host.
    /// @details A path is absolute if it defines an explicit location of a resource. @n @n
    /// This will call the version of this method that matches the compiled platform.
    /// @param ToCheck The path to check.
    /// @return Returns true if the specified path is considered absolute on the host platform, false otherwise.
    Boole MEZZ_LIB IsPathAbsolute_Host(const StringView ToCheck);
    /// @brief Gets whether or not a path is absolute on Posix.
    /// @details A path is absolute if it defines an explicit location of a resource. @n @n
    /// This function is slightly heavier than it's Windows counterpart as it will verify the absence of Windows
    /// style directory separators and fail if it finds any.
    /// @param ToCheck The path to check.
    /// @return Returns true if the specified path is considered absolute on a Posix platform, false otherwise.
    Boole MEZZ_LIB IsPathAbsolute_Posix(const StringView ToCheck);
    /// @brief Gets whether or not a path is absolute on Windows.
    /// @details A path is absolute if it defines an explicit location of a resource.
    /// @param ToCheck The path to check.
    /// @return Returns true if the specified path is considered absolute on a Windows platform, false otherwise.
    Boole MEZZ_LIB IsPathAbsolute_Windows(const StringView ToCheck);
    /// @brief Gets whether or not a path is relative.
    /// @warning This method is imperfect and doesn't check for a faulty path, just that it's not absolute.
    /// @note A path is relative if it requires using the current working directory to define the location of
    /// a resource.
    /// @details This will check if the path is absolute on either Windows or Posix platforms.
    /// @param ToCheck The path to check.
    /// @return Returns true if the specified path is relative, false otherwise.
    Boole MEZZ_LIB IsPathRelative(const StringView ToCheck);
    /// @brief Gets whether or not a path is relative on the host.
    /// @warning This method is imperfect and doesn't check for a faulty path, just that it's not absolute.
    /// @note A path is relative if it requires using the current working directory to define the location of
    /// a resource.
    /// @details This will call the version of this method that matches the compiled platform.
    /// @param ToCheck The path to check.
    /// @return Returns true if the specified path is considered relative on the host platform, false otherwise.
    Boole MEZZ_LIB IsPathRelative_Host(const StringView ToCheck);
    /// @brief Gets whether or not a path is relative on Posix.
    /// @warning This method is imperfect and doesn't check for some faults that can occur in a path, but does
    /// take the extra time to verify the absence of Windows style directory separators (\\).
    /// @note A path is relative if it requires using the current working directory to define the location of
    /// a resource.
    /// @param ToCheck The path to check.
    /// @return Returns true if the specified path is considered relative on a Posix platform, false otherwise.
    Boole MEZZ_LIB IsPathRelative_Posix(const StringView ToCheck);
    /// @brief Gets whether or not a path is relative on Windows.
    /// @warning This method is imperfect and doesn't check for some faults that can occur in a path, nor does
    /// it check for proper Windows style directory separators as most uses of filesystem paths in the Windows
    /// API can tolerate Windows or Posix syle.
    /// @note A path is relative if it requires using the current working directory to define the location of
    /// a resource.
    /// @param ToCheck The path to check.
    /// @return Returns true if the specified path is considered relative on a Windows platform, false otherwise.
    Boole MEZZ_LIB IsPathRelative_Windows(const StringView ToCheck);

    ///////////////////////////////////////////////////////////////////////////////
    // Path Checks

    /// @brief Gets how many directories deep a path is.
    /// @param ToCheck The path to get the depth of.
    /// @param ExitIfNegative If true, the function to return immediately if the depth count becomes negative.
    /// @return Returns an Integer representing how many directories down (or up, if negative) the path goes.
    Integer MEZZ_LIB GetDirectoryDepth(const StringView ToCheck, const Boole ExitIfNegative);
    /// @brief Checks whether or not a given path is a subdirectory or file of a base path.
    /// @note This method will make no attempt to remove dot segments or perform any other normalization
    /// routine to the provided path, thus these transformations must be done manually before calling this
    /// method if they are needed.
    /// @exception This function will only operate if both paths are absolute, or both are relative.  Mixing
    /// of absolute and relative is not supported by this function.  Attempting to compare an absolute and
    /// relative path will result in an PARAMETERS_EXCEPTION being thrown.
    /// @param BasePath The base path to check.
    /// @param CheckPath The path being checked against base path to see if it points to a subdirectory or file.
    /// @return Returns true if CheckPath is a subdirectory or file of BasePath, false otherwise.
    Boole MEZZ_LIB IsSubPath(const StringView BasePath, const StringView CheckPath);

    ///////////////////////////////////////////////////////////////////////////////
    // Path Utilities

    /// @brief Builds a String path based on a number of directory/file names in a String vector.
    /// @note This function relies on the PathRoot argument to generate absolute paths.
    /// @param PathRoot This String will be prepended to the result of the ToBuild parameter.
    /// @param ToBuild A vector of Strings containing the overall path to be built.
    /// @param FileName The file portion of the path to be built.
    /// @param UseWindowsSlash If true, path will be generated with '\\' separators. Otherwise '/' will be used.
    /// @return Returns a String containing the rebuilt path.
    String MEZZ_LIB BuildPath(const StringView PathRoot, const StringVector& ToBuild,
                              const StringView FileName, const Boole UseWindowsSlash = false);
    /// @brief Removes all needless instances of "." or ".." and makes appropriate edits to the path.
    /// @details A dot segment is "." or "..".  They often get in the way of path parsing and this method will
    /// remove any extraneous dot segments that may exist in the provided String.
    /// @param ToRemove The String to remove dot segments from.
    /// @return Returns a copy of the ToRemove parameter with all needless dot segments removed.
    String MEZZ_LIB RemoveDotSegments(const StringView ToRemove);

    /// @brief Convenience method to verify the necessary system separator is present when concatenating.
    /// @param FilePath The directory path to the file.
    /// @param FileName The name of the file.
    /// @return Returns a full String that is the concatenated path and filename.
    String MEZZ_LIB CombinePathAndFileName(const StringView FilePath, const StringView FileName);
}//Filesystem
}//Mezzanine

#endif
