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

#include "FilesystemManagement.h"
#include "PathUtilities.h"
#include "StringTools.h"

#ifdef MEZZ_Windows
    #include <Windows.h>
#else
    #include <stdio.h>
    #include <sys/stat.h>
    #include <sys/types.h>
    #include <unistd.h>
#endif

#include "PlatformUndefs.h"

namespace
{
    using namespace Mezzanine;

#ifdef MEZZ_Windows
    /// @brief Converts a narrow (8-bit) string to a wide (16-bit) string.
    /// @param Narrow The string to be converted.
    /// @return Returns a wide string with the converted contents.
    std::wstring ConvertToWideString(const StringView Narrow)
    {
        std::wstring Ret;
        if( !Narrow.empty() ) {
            int NarrowSize = static_cast<int>(Narrow.size());
            size_t WideLength = ::MultiByteToWideChar(CP_UTF8,0,Narrow.data(),NarrowSize,0,0);
            Ret.resize(WideLength,L'\0');
            ::MultiByteToWideChar(CP_UTF8,0,Narrow.data(),NarrowSize,&Ret[0],static_cast<int>(Ret.size()));
        }
        return Ret;
    }
#endif // MEZZ_Windows
}

namespace Mezzanine {
namespace Filesystem {
    ///////////////////////////////////////////////////////////////////////////////
    // Basic File Management

    Boole FileExists(const StringView FilePath)
    {
    #ifdef MEZZ_Windows
        std::wstring ConvertedPath = ConvertToWideString(FilePath);
        DWORD dwAttrib = GetFileAttributesW(ConvertedPath.c_str());
        return ( dwAttrib != INVALID_FILE_ATTRIBUTES && !( dwAttrib & FILE_ATTRIBUTE_DIRECTORY ) );
    #else // MEZZ_Windows
        //return ( ::access(PathAndName.c_str(),F_OK) );
        struct stat st;
        if( stat(FilePath.data(),&st) == 0 ) {
            return S_ISREG(st.st_mode);
        }
        return false;
    #endif // MEZZ_Windows
    }

    Boole CopyFile(const StringView OldFilePath, const StringView NewFilePath, const Boole FailIfExists)
    {
    #ifdef MEZZ_Windows
        DWORD CopyFlags = COPY_FILE_COPY_SYMLINK;
        if( FailIfExists ) {
            CopyFlags |= COPY_FILE_FAIL_IF_EXISTS;
        }
        std::wstring ConvertedOldPath = ConvertToWideString(OldFilePath);
        std::wstring ConvertedNewPath = ConvertToWideString(NewFilePath);
        return ( ::CopyFileExW(ConvertedOldPath.c_str(),ConvertedNewPath.c_str(),NULL,NULL,NULL,CopyFlags) != 0 );
    #else // MEZZ_Windows
        if( FailIfExists && FileExists(NewFilePath.data()) ) {
            return false;
        }
        std::ifstream SrcStream;
        std::ofstream DestStream;

        SrcStream.open(OldFilePath,std::ios_base::binary | std::ios_base::in);
        DestStream.open(NewFilePath,std::ios_base::binary | std::ios_base::out);

        DestStream << SrcStream.rdbuf();
        return true;
    #endif // MEZZ_Windows
    }

    Boole MoveFile(const StringView OldFilePath, const StringView NewFilePath, const Boole FailIfExists)
    {
    #ifdef MEZZ_Windows
        DWORD MoveFlags = MOVEFILE_COPY_ALLOWED;
        if( !FailIfExists ) {
            MoveFlags |= MOVEFILE_REPLACE_EXISTING;
        }
        std::wstring ConvertedOldPath = ConvertToWideString(OldFilePath);
        std::wstring ConvertedNewPath = ConvertToWideString(NewFilePath);
        return ( ::MoveFileExW(ConvertedOldPath.data(),ConvertedNewPath.data(),MoveFlags) != 0 );
    #else // MEZZ_Windows
        if( FailIfExists && FileExists(NewFilePath.data()) ) {
            return false;
        }
        return ( ::rename(OldFilePath.data(),NewFilePath.data()) == 0 );
    #endif // MEZZ_Windows
    }

    Boole RemoveFile(const StringView FilePath)
    {
    #ifdef MEZZ_Windows
        std::wstring ConvertedPath = ConvertToWideString(FilePath);
        return ( ::DeleteFileW(ConvertedPath.data()) != 0 );
    #else // MEZZ_Windows
        return ( ::unlink(FilePath.data()) == 0 );
    #endif // MEZZ_Windows
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Symlinks

    Boole CreateSymlink(const StringView SymPath, const StringView TargetPath)
    {
    #ifdef MEZZ_Windows
        DWORD LinkFlags = 0;
        std::wstring ConvertedSymPath = ConvertToWideString(SymPath);
        std::wstring ConvertedTargetPath = ConvertToWideString(TargetPath);
        return ( ::CreateSymbolicLinkW(ConvertedSymPath.data(),ConvertedTargetPath.data(),LinkFlags) != 0 );
    #else // MEZZ_Windows
        return ( ::symlink(SymPath.data(),TargetPath.data()) == 0 );
    #endif // MEZZ_Windows
    }

    Boole CreateDirectorySymlink(const StringView SymPath, const StringView TargetPath)
    {
    #ifdef MEZZ_Windows
        DWORD LinkFlags = SYMBOLIC_LINK_FLAG_DIRECTORY;
        std::wstring ConvertedSymPath = ConvertToWideString(SymPath);
        std::wstring ConvertedTargetPath = ConvertToWideString(TargetPath);
        return ( ::CreateSymbolicLinkW(ConvertedSymPath.data(),ConvertedTargetPath.data(),LinkFlags) != 0 );
    #else // MEZZ_Windows
        return ( ::symlink(SymPath.data(),TargetPath.data()) == 0 );
    #endif // MEZZ_Windows
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Basic Directory Management

    Boole DirectoryExists(const StringView DirectoryPath)
    {
    #ifdef MEZZ_Windows
        std::wstring ConvertedPath = ConvertToWideString(DirectoryPath);
        DWORD dwAttrib = GetFileAttributesW(ConvertedPath.data());
        return ( dwAttrib != INVALID_FILE_ATTRIBUTES && ( dwAttrib & FILE_ATTRIBUTE_DIRECTORY ) );
    #else // MEZZ_Windows
        struct stat st;
        if( stat(DirectoryPath.data(),&st) == 0 ) {
            return S_ISDIR(st.st_mode);
        }
        return false;
    #endif // MEZZ_Windows
    }

    Boole CreateDirectory(const StringView DirectoryPath)
    {
    #ifdef MEZZ_Windows
        std::wstring ConvertedPath = ConvertToWideString(DirectoryPath);
        if( ::CreateDirectoryW(ConvertedPath.data(),NULL) < 0 ) {
            if( ERROR_ALREADY_EXISTS == ::GetLastError() ) {
                return false;
            }
            StringStream ExceptionStream;
            ExceptionStream << "Unable to create directory.  Error follows:" << std::endl;
            if( ERROR_PATH_NOT_FOUND == ::GetLastError() ) {
                ExceptionStream << "Path to requested directory does not exist.";
            }else{
                ExceptionStream << "Error Unknown. :(";
            }
            //MEZZ_EXCEPTION(ExceptionBase::IO_DIRECTORY_NOT_FOUND_EXCEPTION,ExceptionStream.str());
            throw std::runtime_error(ExceptionStream.str());
        }
        return true;
    #else // MEZZ_Windows
        if( ::mkdir(DirectoryPath.data(),0777) < 0 ) {
            if( EEXIST == errno ) {
                return false;
            }
            StringStream ExceptionStream;
            ExceptionStream << "Unable to create directory.  Error follows:" << std::endl;
            ExceptionStream << std::strerror(errno);
            //MEZZ_EXCEPTION(ExceptionBase::IO_DIRECTORY_NOT_FOUND_EXCEPTION,ExceptionStream.str());
            throw std::runtime_error(ExceptionStream.str());
        }
        return true;
    #endif // MEZZ_Windows
    }

    Boole CreateDirectoryPath(const StringView DirectoryPath)
    {
        Boole Result = true;
        StringVector FolderNames;
        StringVector FolderVec = StringTools::Split(DirectoryPath,"/\\");
        size_t StartIndex = 0;
        String PathAttempt;
        Char8 SysSlash = GetDirectorySeparator_Host();
    #ifdef MEZZ_Windows
        // For windows and windows like machines, see if the first entry is a drive.
        // Because attempting to make a drive is silly.
        if( FolderVec.at(0).find(':') != String::npos ) {
            PathAttempt.append( FolderVec.at(0) );
            PathAttempt.append( 1, SysSlash );
            StartIndex++;
        }
    #else // MEZZ_Windows
        PathAttempt.append( 1, SysSlash );
    #endif // MEZZ_Windows
        for( size_t VecIndex = StartIndex ; Result && VecIndex < FolderVec.size() ; ++VecIndex )
        {
            PathAttempt.append( FolderVec.at(VecIndex) );
            PathAttempt.append( 1, SysSlash );
            Result = CreateDirectory( PathAttempt );
        }
        return Result;
    }

    Boole RemoveDirectory(const StringView DirectoryPath)
    {
    #ifdef MEZZ_Windows
        std::wstring ConvertedPath = ConvertToWideString(DirectoryPath);
        return ( ::RemoveDirectoryW(ConvertedPath.data()) != 0 );
    #else // MEZZ_Windows
        return ( ::rmdir(DirectoryPath.data()) == 0 );
    #endif // MEZZ_Windows
    }
}//Filesystem
}//Mezzanine
