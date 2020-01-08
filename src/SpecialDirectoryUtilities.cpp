// � Copyright 2010 - 2018 BlackTopp Studios Inc.
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
    // We want Windows Vista APIs and up.
    #define _WIN32_WINNT 0x0601
#endif

#include "SpecialDirectoryUtilities.h"
#include "PathUtilities.h"
#include "StringTools.h"

#ifdef MEZZ_Windows
    #define WIN32_LEAN_AND_MEAN

    #include <Windows.h>
    #include <direct.h> //May not be friendly on some versions of MSVC
    #include <Shlobj.h>
#else
    #include <sys/types.h>
    #include <unistd.h>
    #include <pwd.h>

    #ifdef MEZZ_MacOSX
        #include <mach-o/dyld.h> // for _NSGetExecutablePath
    #endif
#endif

/*
#ifdef MEZZ_WINDOWS
    #include <Windows.h>
    #include <direct.h>
    #include <errno.h>
    #include <Shlobj.h>
#elif MEZZ_MACOSX
    #include <sys/types.h>
    #include <unistd.h>
    #include <pwd.h>
    #include <mach-o/dyld.h> // for _NSGetExecutablePath
#else
    #include <sys/types.h>
    #include <unistd.h>
    #include <pwd.h>
#endif
//*/

#include "PlatformUndefs.h"

namespace
{
    using namespace Mezzanine;

    /// @brief ArgC as it was passed into Main.
    /// @details This cannot be set statically, it must wait for main(int, char**) to
    /// be initialized, then call the appropriate function to set this.
    int ArgC = 0;
    /// @brief ArgC as it was passed into Main.
    /// @details This cannot be set statically, it must wait for main(int, char**) to
    /// be initialized, then call the appropriate function to set this.
    char** ArgV = nullptr;

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
    /// @brief Gets the filesystem location of a specific folder known by the system.
    /// @param FolderID The ID of the system/user/program folder to get the path of.
    /// @return Returns a String path to the location of the folder specified.
    String GetSystemFolder(REFKNOWNFOLDERID FolderID)
    {
        LPWSTR WPath = nullptr;
        HRESULT Result = S_OK;
        Result = SHGetKnownFolderPath(FolderID,0,nullptr,&WPath);
        if( SUCCEEDED(Result) ) {
            return ConvertToNarrowString(WPath);
        }
        return String();
    }
#else
    /// @brief Gets the filesystem location of the current users home folder.
    /// @return Returns a String path to the location of the users home folder.
    String GetHomeFholder()
    {
        struct passwd* pw = getpwuid( getuid() );
        if( pw != nullptr ) {
            return String(pw->pw_dir);
        }
        return String();
    }
#endif
}

namespace Mezzanine {
namespace Filesystem {
    ///////////////////////////////////////////////////////////////////////////////
    // MainArg Utilities

    void CacheMainArgs(int ArgCount, char** ArgVars)
    {
        ArgC = ArgCount;
        ArgV = ArgVars;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Execution Directory Utilities

    String GetExecutableDir()
    {
        return GetExecutableDir(ArgC,ArgV);
    }

    String GetExecutableDir(int ArgCount, char** ArgVars)
    {
        String Results( GetExecutableDirFromArg(ArgCount,ArgVars) );
        if( Results == "." || Results.empty() ) {
            return GetExecutableDirFromSystem();
        }else{
            return Results;
        }
    }

    String GetExecutableDirFromSystem()
    {
        char Results[FILENAME_MAX];
    #if defined(MEZZ_Windows)
        ::GetModuleFileName(nullptr,Results,FILENAME_MAX);
        return GetDirName(String(Results));
    #elif defined(MEZZ_MacOSX) || defined(MEZZ_Ios)
        uint32_t ResultsSize = sizeof(Results);
        if( _NSGetExecutablePath(Results,&ResultsSize) == 0 ) {
            return GetDirName(String(Results));
        }else{
            return String();
        }
    #else//Linux, Andriod, etc
        MaxInt Length = ::readlink("/proc/self/exe",Results,sizeof(Results)-1);
        if( Length != -1 ) {
            Results[Length] = '\0';
            return GetDirName(String(Results));
        }else{
            return String();
        }
    #endif//MEZZ_Windows
    }

    String GetExecutableDirFromArg()
    {
        return GetExecutableDirFromArg(ArgC,ArgV);
    }

    String GetExecutableDirFromArg(int ArgCount, char** ArgVars)
    {
        if( ArgCount > 0 ) {
            // No command is clearly bogus, must bail
            if( GetBaseName(ArgVars[0]).empty() ) {
                return String();
            }

            String Results(GetDirName(ArgVars[0]));

            // common cases of exe existing but dir is empty
            if( Results.empty() || Results == "./" || Results == ".\\" ) {
                return ".";
            }

            if( !Results.empty() ) {
                return Results;
            }
            return String();
        }else{
            return String();
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Working Directory Utilities

    void ChangeWorkingDirectory(const StringView ChangeTo)
    {
    #ifdef MEZZ_Windows
        if( _chdir( ChangeTo.data() ) ) {
            StringStream ExceptionStream;
            ExceptionStream << "Failed to change working directory to \"" << ChangeTo << "\"\n";
            throw std::runtime_error( ExceptionStream.str() );
        }
    #else
        if( chdir( ChangeTo.data() ) ) {
            StringStream ExceptionStream;
            ExceptionStream << "Failed to change working directory to \"" << ChangeTo << "\"\n";
            ExceptionStream << strerror(errno) << "\n";
            throw std::runtime_error( ExceptionStream.str() );
        }
    #endif
    }

    String GetWorkingDirectory()
    {
        char cCurrentPath[FILENAME_MAX];
    #ifdef MEZZ_Windows
        String Results ( _getcwd(cCurrentPath,sizeof(cCurrentPath)) );
    #else
        String Results ( getcwd(cCurrentPath,sizeof(cCurrentPath)) );
    #endif
        return Results;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Application Data Directory Utilities

    String GetLocalAppDataDir()
    {
    #ifdef MEZZ_Windows
        return GetSystemFolder(FOLDERID_LocalAppData);
    #else
        return GetHomeFolder();
    #endif
    }

    String GetShareableAppDataDir()
    {
    #ifdef MEZZ_Windows
        return GetSystemFolder(FOLDERID_RoamingAppData);
    #else
        return GetHomeFolder();
    #endif
    }

    String GetCurrentUserDataDir()
    {
    #ifdef MEZZ_Windows
        return GetSystemFolder(FOLDERID_Profile);
    #else
        return GetHomeFolder();
    #endif
    }

    String GetCommonUserDataDir()
    {
    #ifdef MEZZ_Windows
        return GetSystemFolder(FOLDERID_Public);
    #else
        return GetHomeFolder();
    #endif
    }
}//Filesystem
}//Mezzanine
