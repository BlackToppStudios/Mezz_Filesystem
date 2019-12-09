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

#include "CrossPlatformExport.h"

#ifdef MEZZ_Windows
    #define _WIN32_WINNT 0x0601
#endif

#include "DirectoryContents.h"
#include "StringTools.h"

#ifdef MEZZ_Windows
    #define WIN32_LEAN_AND_MEAN

    #include <Windows.h>
#else
    #include <stdio.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <dirent.h>
    #include <unistd.h>
#endif

#include "PlatformUndefs.h"

namespace
{
    using namespace Mezzanine;

#ifdef MEZZ_Windows
    /// @brief Convenience type because Windows likes fatty Strings.
    using WideString = std::wstring;

    /// @brief Demotes a size_t to an int, which is required by the Win API.
    /// @brief ToDemote The large integral type to be demoted.
    /// @return Returns an int of the same value if it can be stored, or asserts otherwise.
    [[nodiscard]]
    int IntDemote(const size_t ToDemote)
    {
        assert( ToDemote <= static_cast<size_t>( std::numeric_limits<int>::max() ) );
        return static_cast<int>(ToDemote);
    }
    /// @brief Converts a system time type to a standard time type.
    /// @param TimeVal the system time type to convert.
    /// @return Returns a converted standard time value.
    [[nodiscard]]
    UInt64 ConvertTime(const FILETIME TimeVal) noexcept
    {
        LARGE_INTEGER Converter;
        Converter.HighPart = TimeVal.dwHighDateTime;
        Converter.LowPart = TimeVal.dwLowDateTime;
        return static_cast<UInt64>(Converter.QuadPart);
    }
    /*/// @brief Converts a standard time type to a system time type.
    /// @param TimeVal The standard time type to convert.
    /// @return Returns a converted system time value.
    FILETIME ConvertTime(const UInt64 TimeVal)
    {
        FILETIME Ret;
        LARGE_INTEGER Converter;
        Converter.QuadPart = TimeVal;
        Ret.dwHighDateTime = Converter.HighPart;
        Ret.dwLowDateTime = Converter.LowPart;
        return Ret;
    }//*/
    /// @brief Converts a narrow (8-bit) string to a wide (16-bit) string.
    /// @param Thin The string to be converted.
    /// @return Returns a wide string with the converted contents.
    [[nodiscard]]
    WideString ConvertToWideString(const StringView Thin)
    {
        WideString Ret;
        size_t ThinSize = Thin.size();
        if( ThinSize > 0 ) {
            size_t WideLength = ::MultiByteToWideChar(CP_UTF8,0,Thin.data(),IntDemote(ThinSize),nullptr,0);
            Ret.resize(WideLength,L'\0');
            ::MultiByteToWideChar(CP_UTF8,0,Thin.data(),IntDemote(ThinSize),&Ret[0],IntDemote(WideLength));
        }
        return Ret;
    }
    /// @brief Converts a wide (16-bit) string to a narrow (8-bit) string.
    /// @param Wide The string to be converted.
    /// @return Returns a narrow string with the converted contents.
    [[nodiscard]]
    String ConvertToNarrowString(const wchar_t* Wide)
    {
        String Ret;
        size_t WideLength = std::wcslen(Wide);
        if( WideLength > 0 ) {
            size_t ThinLength = ::WideCharToMultiByte(CP_UTF8,0,Wide,IntDemote(WideLength),nullptr,0,nullptr,nullptr);
            Ret.resize(ThinLength,'\0');
            ::WideCharToMultiByte(CP_UTF8,0,Wide,IntDemote(WideLength),&Ret[0],IntDemote(ThinLength),nullptr,nullptr);
        }
        return Ret;
    }
    /// @brief Performs all the necessary operations to prepare a String to be provided to Windows.
    /// @param ToPrepare The String to be prepared.
    /// @return Returns a WideString that can be passed into Windows OS File APIs.
    [[nodiscard]]
    WideString PreparePathForWindows(const StringView ToPrepare)
    {
        WideString ConvertedPath = ConvertToWideString(ToPrepare);
        if( ConvertedPath.back() != L'/' && ConvertedPath.back() != L'\\' ) {
            ConvertedPath.append(1,L'\\');
        }
        ConvertedPath.append(1,L'*');
        return ConvertedPath;
    }
    /// @brief Transposes all data from the Windows OS entry to a Mezzanine entry.
    /// @param Original The entry produced by the OS to be transposed.
    /// @param NewEntry The new Mezzanine entry to transpose to.
    void TransposeEntry(const WIN32_FIND_DATAW& Original, ArchiveEntry& NewEntry)
    {
        NewEntry.CreateTime = ConvertTime( Original.ftCreationTime );
        NewEntry.AccessTime = ConvertTime( Original.ftLastAccessTime );
        NewEntry.ModifyTime = ConvertTime( Original.ftLastWriteTime );
        NewEntry.Permissions = FilePermissions::Owner_Read; //Get more accurate info?
        if( ( Original.dwFileAttributes & FILE_ATTRIBUTE_READONLY ) == 0 ) {
            NewEntry.Permissions = NewEntry.Permissions | FilePermissions::Owner_Write;
        }
        size_t DotPos = NewEntry.Name.find_last_of('.');
        if( DotPos < NewEntry.Name.size() ) {
            String Extension = NewEntry.Name.substr( NewEntry.Name.find_last_of('.') );
            StringTools::ToLowerCase(Extension.begin(),Extension.end());
            if( Extension == ".exe" || Extension == ".bat" || Extension == ".com" || Extension == ".cmd" ) {
                NewEntry.Permissions = NewEntry.Permissions | FilePermissions::Owner_Execute;
            }
        }
        if( Original.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
            NewEntry.Entry = EntryType::Directory;
        }else{
            if( Original.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT ) {
                NewEntry.Entry = EntryType::Symlink;
            }else{
                NewEntry.Entry = EntryType::File;
            }

            LARGE_INTEGER FileSizeConverter;
            FileSizeConverter.LowPart = Original.nFileSizeLow;
            FileSizeConverter.HighPart = Original.nFileSizeHigh;
            NewEntry.Size = FileSizeConverter.QuadPart;
        }
    }
#else // MEZZ_Windows
    /// @brief Converts posix file permissions to Mezzanine permissions.
    /// @param Perms The permissions to be converted.
    /// @return Returns a UInt32 bitmask containing the converted file permissions.
    [[nodiscard]]
    UInt32 ConvertPosixPermissions(const mode_t Perms) noexcept
    {
        UInt32 Ret = 0;
        Ret |= static_cast<UInt32>( Perms & S_IRWXO );
        Ret |= static_cast<UInt32>( Perms & S_IRWXG );
        Ret |= static_cast<UInt32>( Perms & S_IRWXU );
        return Ret;
    }
    /*/// @brief Converts Mezzanine file permissions to posix permissions
    /// @param Perms The permissions to be converted.
    /// @return Returns a mode_t bitmask containing the converted file permissions.
    [[nodiscard]]
    mode_t ConvertMezzPermissions(const UInt32 Perms) noexcept
    {
        mode_t Ret = 0;
        Ret |= static_cast<mode_t>( Perms & static_cast<UInt32>(FilePermissions::Other_All) );
        Ret |= static_cast<mode_t>( Perms & static_cast<UInt32>(FilePermissions::Group_All) );
        Ret |= static_cast<mode_t>( Perms & static_cast<UInt32>(FilePermissions::Owner_All) );
        return Ret;
    }//*/
    /// @brief Transposes all data from the Posix OS entry to a Mezzanine entry.
    /// @param Original The entry produced by the OS to be transposed.
    /// @param NewEntry The new Mezzanine entry to transpose to.
    void TransposeEntry(const struct stat& Original, ArchiveEntry& NewEntry) noexcept
    {
        NewEntry.CreateTime = static_cast<UInt64>( Original.st_ctime );
        NewEntry.AccessTime = static_cast<UInt64>( Original.st_atime );
        NewEntry.ModifyTime = static_cast<UInt64>( Original.st_mtime );
        NewEntry.Permissions = static_cast<FilePermissions>( ConvertPosixPermissions(Original.st_mode) );

        if( S_ISDIR(Original.st_mode) ) {
            NewEntry.Entry = EntryType::Directory;
        }else{
            if( S_ISLNK(Original.st_mode) ) {
                NewEntry.Entry = EntryType::Symlink;
            }else if( S_ISREG(Original.st_mode) ) {
                NewEntry.Entry = EntryType::File;
            }else{
                // I dunno wtf we found
                return;
            }

            NewEntry.Size = static_cast<UInt64>(Original.st_size);
        }
    }
#endif // MEZZ_Windows
    /// @brief Checks to see if the String is a dot segment.
    /// @param ToCheck The String to be checked.
    /// @return Returns true if the String is "." or "..", false otherwise.
    [[nodiscard]]
    Boole IsDotSegment(const StringView ToCheck) noexcept
        { return ToCheck == "." || ToCheck == ".."; }
}

namespace Mezzanine {
namespace Filesystem {
    ///////////////////////////////////////////////////////////////////////////////
    // Directory Contents

    StringVector GetDirectoryContentNames(const StringView DirectoryPath)
    {
        StringVector Ret;
    #ifdef MEZZ_Windows
        WIN32_FIND_DATAW FileData;
        HANDLE FileHandle = INVALID_HANDLE_VALUE;

        WideString ConvertedPath = PreparePathForWindows(DirectoryPath);
        FileHandle = ::FindFirstFileW( ConvertedPath.data(), &FileData );
        if( FileHandle == INVALID_HANDLE_VALUE ) {
            return Ret;
        }

        do{
            String EntryName = ConvertToNarrowString( FileData.cFileName );
            if( IsDotSegment(EntryName) ) {
                continue;
            }
            Ret.push_back( std::move(EntryName) );
        }while( ::FindNextFileW( FileHandle, &FileData ) );

        ::FindClose(FileHandle);
    #else
        struct dirent* DirEntry;
        DIR* Directory = ::opendir( DirectoryPath.data() );
        if( Directory ) {
            while( ( DirEntry = ::readdir(Directory) ) )
            {
                String EntryName = DirEntry->d_name;
                if( IsDotSegment(EntryName) ) {
                    continue;
                }

                Ret.push_back( std::move(EntryName) );
            }

            ::closedir(Directory);
        }
    #endif
        return Ret;
    }

    ArchiveEntryVector GetDirectoryContents(const StringView DirectoryPath)
    {
        ArchiveEntryVector Ret;
    #ifdef MEZZ_Windows
        WIN32_FIND_DATAW FileData;
        //LARGE_INTEGER FileSizeConverter;
        HANDLE FileHandle = INVALID_HANDLE_VALUE;

        WideString ConvertedPath = PreparePathForWindows(DirectoryPath);
        FileHandle = ::FindFirstFileW( ConvertedPath.data(), &FileData );
        if( FileHandle == INVALID_HANDLE_VALUE ) {
            return Ret;
        }

        do{
            ArchiveEntry NewEntry;
            NewEntry.Name = ConvertToNarrowString( FileData.cFileName );
            if( IsDotSegment(NewEntry.Name) ) {
                continue;
            }

            NewEntry.Archive = ArchiveType::FileSystem;
            TransposeEntry(FileData,NewEntry);
            Ret.push_back(NewEntry);
        }while( ::FindNextFileW( FileHandle, &FileData ) );

        ::FindClose(FileHandle);
    #else
        struct dirent* DirEntry;
        std::cout << "Checking for the existence of directory \"" << DirectoryPath << "\"\n";
        DIR* Directory = ::opendir( DirectoryPath.data() );
        if( Directory ) {
            std::cout << "\"" << DirectoryPath << "\" exists!\n";
            struct stat FileStat;
            while( ( DirEntry = ::readdir(Directory) ) )
            {
                std::cout << "stat'ing \"" << DirEntry->d_name << "\".\n";
                if( ::stat(DirEntry->d_name,&FileStat) == -1 ) {
                    std::cout << "stat for \"" << DirEntry->d_name << "\" failed.\n";
                    continue;
                }

                ArchiveEntry NewEntry;
                NewEntry.Name = DirEntry->d_name;
                std::cout << "Checking to see if \"" << DirEntry->d_name << "\" is a dot segment.\n";
                if( IsDotSegment(NewEntry.Name) ) {
                    std::cout << "\"" << DirEntry->d_name << "\" is in fast a dot segment.\n";
                    continue;
                }

                NewEntry.Archive = ArchiveType::FileSystem;
                TransposeEntry(FileStat,NewEntry);
                std::cout << "Pushing \"" << DirEntry->d_name << "\" to result vector.\n";
                Ret.push_back(NewEntry);
            }

            ::closedir(Directory);
        }
    #endif
        return Ret;
    }
}//Filesystem
}//Mezzanine
