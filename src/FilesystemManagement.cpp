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

#include <cstring>
#include <iostream>

#ifdef MEZZ_Windows
    #define WIN32_LEAN_AND_MEAN

    SAVE_WARNING_STATE
    SUPPRESS_VC_WARNING(4668)

    #include <Windows.h>

    RESTORE_WARNING_STATE
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

    /// @brief Converts the system error number to a Mezzanine ModifyResult.
    /// @param err The system error to be converted.
    /// @return Returns a ModifyResult value corresponding to the system error code.
    Filesystem::ModifyResult ConvertErrNo(DWORD err)
    {
        switch( err )
        {
            case ERROR_SUCCESS:            return Filesystem::ModifyResult::Success;
            case ERROR_ALREADY_EXISTS:     return Filesystem::ModifyResult::AlreadyExists;
            case ERROR_FILE_EXISTS:        return Filesystem::ModifyResult::AlreadyExists;
            case ERROR_FILE_NOT_FOUND:     return Filesystem::ModifyResult::DoesNotExist;
            case ERROR_PATH_NOT_FOUND:     return Filesystem::ModifyResult::DoesNotExist;
            case ERROR_INVALID_NAME:       return Filesystem::ModifyResult::InvalidPath;
            case ERROR_BAD_PATHNAME:       return Filesystem::ModifyResult::InvalidPath;
            //case :         return Filesystem::ModifyResult::LoopingPath;
            //case :  return Filesystem::ModifyResult::NameTooLong;
            case ERROR_ACCESS_DENIED:      return Filesystem::ModifyResult::PermissionDenied;
            //case :         return Filesystem::ModifyResult::ReadOnly;
            //case :       return Filesystem::ModifyResult::NotADirectory;
            case ERROR_DIR_NOT_EMPTY:      return Filesystem::ModifyResult::NotEmpty;
            //case :           return Filesystem::ModifyResult::IOError;
            case ERROR_NOT_ENOUGH_MEMORY:  return Filesystem::ModifyResult::NoSpace;
            case ERROR_OUTOFMEMORY:        return Filesystem::ModifyResult::NoSpace;
            case ERROR_DISK_FULL:          return Filesystem::ModifyResult::NoSpace;
            //case :        return Filesystem::ModifyResult::MaxLinksExceeded;
            case ERROR_PATH_BUSY:          return Filesystem::ModifyResult::CurrentlyBusy;
            case ERROR_REQUEST_ABORTED:    return Filesystem::ModifyResult::OperationCanceled;
            default:                       return Filesystem::ModifyResult::Unknown;
        }
    }
#else // MEZZ_Windows
    /// @brief Converts the system error number to a Mezzanine ModifyResult.
    /// @param err The system error to be converted.
    /// @return Returns a ModifyResult value corresponding to the system error code.
    Filesystem::ModifyResult ConvertErrNo(int err)
    {
        switch( err )
        {
            case EEXIST:        return Filesystem::ModifyResult::AlreadyExists;
            case ENOENT:        return Filesystem::ModifyResult::DoesNotExist;
            case EINVAL:        return Filesystem::ModifyResult::InvalidPath;
            case ELOOP:         return Filesystem::ModifyResult::LoopingPath;
            case ENAMETOOLONG:  return Filesystem::ModifyResult::NameTooLong;
            case EACCES:        return Filesystem::ModifyResult::PermissionDenied;
            case EPERM:         return Filesystem::ModifyResult::PermissionDenied;
            case EROFS:         return Filesystem::ModifyResult::ReadOnly;
            case ENOTDIR:       return Filesystem::ModifyResult::NotADirectory;
            case EISDIR:        return Filesystem::ModifyResult::IsADirectory;
            case ENOTEMPTY:     return Filesystem::ModifyResult::NotEmpty;
            case EIO:           return Filesystem::ModifyResult::IOError;
            case ENOSPC:        return Filesystem::ModifyResult::NoSpace;
            case EMLINK:        return Filesystem::ModifyResult::MaxLinksExceeded;
            case EBUSY:         return Filesystem::ModifyResult::CurrentlyBusy;
            case ECANCELED:     return Filesystem::ModifyResult::OperationCanceled;
            default:            return Filesystem::ModifyResult::Unknown;
        }
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
        std::wstring WidePath = ConvertToWideString(FilePath);
        DWORD dwAttrib = GetFileAttributesW(WidePath.c_str());
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

    ModifyResult CopyFile(const StringView OldFilePath, const StringView NewFilePath, const Boole FailIfExists)
    {
    #ifdef MEZZ_Windows
        DWORD CopyFlags = COPY_FILE_COPY_SYMLINK;
        if( FailIfExists ) {
            CopyFlags |= COPY_FILE_FAIL_IF_EXISTS;
        }
        std::wstring WideOldPath = ConvertToWideString(OldFilePath);
        std::wstring WideNewPath = ConvertToWideString(NewFilePath);
        return ( ::CopyFileExW(WideOldPath.c_str(),WideNewPath.c_str(),NULL,NULL,NULL,CopyFlags) != 0 ?
                 ModifyResult::Success :
                 ConvertErrNo( ::GetLastError() ) );
    #else // MEZZ_Windows
        if( FailIfExists && FileExists(NewFilePath.data()) ) {
            return ModifyResult::AlreadyExists;
        }
        std::ifstream SrcStream;
        std::ofstream DestStream;

        SrcStream.open(String(OldFilePath),std::ios_base::binary | std::ios_base::in);//Converting to String is Ew.
        DestStream.open(String(NewFilePath),std::ios_base::binary | std::ios_base::out);//Converting to String is Ew.

        DestStream << SrcStream.rdbuf();
        return ModifyResult::Success;
    #endif // MEZZ_Windows
    }

    ModifyResult MoveFile(const StringView OldFilePath, const StringView NewFilePath, const Boole FailIfExists)
    {
    #ifdef MEZZ_Windows
        DWORD MoveFlags = MOVEFILE_COPY_ALLOWED;
        if( !FailIfExists ) {
            MoveFlags |= MOVEFILE_REPLACE_EXISTING;
        }
        std::wstring WideOldPath = ConvertToWideString(OldFilePath);
        std::wstring WideNewPath = ConvertToWideString(NewFilePath);
        return ( ::MoveFileExW(WideOldPath.data(),WideNewPath.data(),MoveFlags) != 0 ?
                 ModifyResult::Success :
                 ConvertErrNo( ::GetLastError() ) );
    #else // MEZZ_Windows
        if( FailIfExists && FileExists(NewFilePath.data()) ) {
            return ModifyResult::AlreadyExists;
        }
        return ( ::rename(OldFilePath.data(),NewFilePath.data()) == 0 ?
                 ModifyResult::Success :
                 ConvertErrNo(errno) );
    #endif // MEZZ_Windows
    }

    ModifyResult RemoveFile(const StringView FilePath)
    {
    #ifdef MEZZ_Windows
        std::wstring WidePath = ConvertToWideString(FilePath);
        return ( ::DeleteFileW(WidePath.data()) != 0 ?
                 ModifyResult::Success :
                 ConvertErrNo( ::GetLastError() ) );
    #else // MEZZ_Windows
        return ( ::unlink(FilePath.data()) == 0 ?
                 ModifyResult::Success :
                 ConvertErrNo(errno) );
    #endif // MEZZ_Windows
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Symlinks

    ModifyResult CreateSymlink(const StringView SymPath, const StringView TargetPath)
    {
    #ifdef MEZZ_Windows
        DWORD LinkFlags = 0;
        std::wstring WideSymPath = ConvertToWideString(SymPath);
        std::wstring WideTargetPath = ConvertToWideString(TargetPath);
        return ( ::CreateSymbolicLinkW(WideSymPath.data(),WideTargetPath.data(),LinkFlags) != 0 ?
                 ModifyResult::Success :
                 ConvertErrNo( ::GetLastError() ) );
    #else // MEZZ_Windows
        return ( ::symlink(SymPath.data(),TargetPath.data()) == 0 ?
                 ModifyResult::Success :
                 ConvertErrNo(errno) );
    #endif // MEZZ_Windows
    }

    ModifyResult CreateDirectorySymlink(const StringView SymPath, const StringView TargetPath)
    {
    #ifdef MEZZ_Windows
        DWORD LinkFlags = SYMBOLIC_LINK_FLAG_DIRECTORY;
        std::wstring WideSymPath = ConvertToWideString(SymPath);
        std::wstring WideTargetPath = ConvertToWideString(TargetPath);
        return ( ::CreateSymbolicLinkW(WideSymPath.data(),WideTargetPath.data(),LinkFlags) != 0 ?
                 ModifyResult::Success :
                 ConvertErrNo( ::GetLastError() ) );
    #else // MEZZ_Windows
        return ( ::symlink(SymPath.data(),TargetPath.data()) == 0 ?
                 ModifyResult::Success :
                 ConvertErrNo(errno) );
    #endif // MEZZ_Windows
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Basic Directory Management

    Boole DirectoryExists(const StringView DirectoryPath)
    {
    #ifdef MEZZ_Windows
        std::wstring WidePath = ConvertToWideString(DirectoryPath);
        DWORD dwAttrib = GetFileAttributesW(WidePath.data());
        return ( dwAttrib != INVALID_FILE_ATTRIBUTES && ( dwAttrib & FILE_ATTRIBUTE_DIRECTORY ) );
    #else // MEZZ_Windows
        struct stat st;
        if( stat(DirectoryPath.data(),&st) == 0 ) {
            return S_ISDIR(st.st_mode);
        }
        return false;
    #endif // MEZZ_Windows
    }

    ModifyResult CreateDirectory(const StringView DirectoryPath)
    {
    #ifdef MEZZ_Windows
        std::wstring WidePath = ConvertToWideString(DirectoryPath);
        return ( ::CreateDirectoryW(WidePath.data(),NULL) != 0 ?
                 ModifyResult::Success :
                 ConvertErrNo( ::GetLastError() ) );
    #else // MEZZ_Windows
        return ( ::mkdir(DirectoryPath.data(),0755) == 0 ?
                 ModifyResult::Success :
                 ConvertErrNo(errno) );
    #endif // MEZZ_Windows
    }

    ModifyResult CreateDirectoryPath(const StringView DirectoryPath)
    {
        ModifyResult Result = ModifyResult::Success;
    #ifdef MEZZ_Windows
        StringVector FolderVec = StringTools::Split(DirectoryPath,"/\\");
    #else // MEZZ_Windows
        StringVector FolderVec = StringTools::Split(DirectoryPath,"/");
    #endif // MEZZ_Windows
        size_t VecIndex = 0;
        String PathAttempt;
        Char8 SysSlash = GetDirectorySeparator_Host();
    #ifdef MEZZ_Windows
        // For windows and windows like machines, see if the first entry is a drive.
        if( IsPathAbsolute_Host(DirectoryPath) ) {
            PathAttempt.append( FolderVec.at(0) );
            PathAttempt.append( 1, SysSlash );
            VecIndex++;
        }
    #else // MEZZ_Windows
        if( IsPathAbsolute_Host(DirectoryPath) ) {
            PathAttempt.append( 1, SysSlash );
        }
    #endif // MEZZ_Windows
        while( ( Result == ModifyResult::Success || Result == ModifyResult::AlreadyExists )
               && VecIndex < FolderVec.size() )
        {
            PathAttempt.append( FolderVec.at(VecIndex) );
            PathAttempt.append( 1, SysSlash );
            if( !Filesystem::IsDotSegment( FolderVec.at(VecIndex) ) ) {
                Result = Filesystem::CreateDirectory( PathAttempt );
            }
            ++VecIndex;
        }
        return Result;
    }

    ModifyResult RemoveDirectory(const StringView DirectoryPath)
    {
    #ifdef MEZZ_Windows
        std::wstring WidePath = ConvertToWideString(DirectoryPath);
        return ( ::RemoveDirectoryW(WidePath.data()) != 0 ?
                 ModifyResult::Success :
                 ConvertErrNo( ::GetLastError() ) );
    #elif MEZZ_CompilerIsEmscripten
        // rmdir on emscripten fails to find the directory specified if it contains a trailing slash.
        if( IsDirectorySeparator_Host( DirectoryPath.back() ) ) {
            String FixedPath(DirectoryPath.data(),DirectoryPath.size() - 1);
            return ( ::rmdir( FixedPath.data() ) == 0 ?
                     ModifyResult::Success :
                     ConvertErrNo(errno) );
        }else{
            return ( ::rmdir( DirectoryPath.data() ) == 0 ?
                     ModifyResult::Success :
                     ConvertErrNo(errno) );
        }
    #else // MEZZ_Windows
        return ( ::rmdir(DirectoryPath.data()) == 0 ?
                 ModifyResult::Success :
                 ConvertErrNo(errno) );
    #endif // MEZZ_Windows
    }
}//Filesystem
}//Mezzanine
