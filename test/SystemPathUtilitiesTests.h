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
#ifndef Mezz_Filesystem_SystemPathUtilitiesTests_h
#define Mezz_Filesystem_SystemPathUtilitiesTests_h

/// @file
/// @brief This file tests of some utilities that work with the system path.

#include "MezzTest.h"

#include "SystemPathUtilities.h"
#include "PathUtilities.h"
#include "StringTools.h"

AUTOMATIC_TEST_GROUP(SystemPathUtilitiesTests,SystemPathUtilities)
{
    using namespace Mezzanine;

    {//Host output
        TestLog << "Your Current PATH as parsed by GetSystemPATH():\n";
        StringVector PATH = Filesystem::GetSystemPATH();
        for( StringVector::const_iterator Iter = PATH.begin() ; Iter != PATH.end() ; Iter++ )
            { TestLog << "\t\"" << *Iter << "\"\n"; }
        TestLog << "End of current system PATH\n";
    }//Host output

    {//GetSystemPATH
        String PosixPath;
        Char8 HostSep = Filesystem::GetPathSeparator_Host();
        PosixPath.append("/a/b/c").append(1,HostSep).append("/bin").append(1,HostSep).append(1,HostSep);

        StringVector SplitPosixPath = Filesystem::GetSystemPATH( PosixPath );
        TEST_EQUAL("GetSystemPATH(const_StringView)-Count",size_t(3),SplitPosixPath.size());
        TEST_EQUAL("GetSystemPATH(const_StringView)-Element1",String("/a/b/c"),SplitPosixPath[0]);
        TEST_EQUAL("GetSystemPATH(const_StringView)-Element2",String("/bin"),SplitPosixPath[1]);
        TEST_EQUAL("GetSystemPATH(const_StringView)-Element3",String(""),SplitPosixPath[2]);
    }//GetSystemPATH

    {//Which
        auto GetCommandResults = [](String Cmd) -> String {
            Cmd.append(" > CommandResults.txt");
            system(Cmd.data());
            std::ifstream ResultFile("CommandResults.txt");
            return String( std::istreambuf_iterator<char>(ResultFile), {} );
        };
    #ifdef MEZZ_Windows
        String SysWhichcmd = GetCommandResults("where cmd");
        String MezzWhichcmd = Filesystem::Which("cmd");
        // Windows/NTFS isn't a case sensitive filesystem and one of the test machines kept
        // returning "C:/Windows/system32" instead of "C:/Windows/System32".
        StringTools::ToLowerCase(SysWhichcmd.begin(),SysWhichcmd.end());
        StringTools::ToLowerCase(MezzWhichcmd.begin(),MezzWhichcmd.end());
        // where seems to like to insert newlines at the end of it's results, even if it's only one.
        if( StringTools::IsNewline( SysWhichcmd.back() ) ) {
            SysWhichcmd.pop_back();
        }
        TEST_EQUAL("Which(const_StringView)-cmd",SysWhichcmd,MezzWhichcmd);
    #else
        String SysWhichls = GetCommandResults("which ls");
        String MezzWhichls = Filesystem::Which("ls");
        TEST_EQUAL("Which(const_StringView)-ls",SysWhichls,MezzWhichls);
    #endif

        if( Filesystem::RemoveFile("CommandResults.txt") == false ) {
            TEST_RESULT("CommandResults-CleanupFailed",Testing::TestResult::Warning);
        }
    }//Which
}

#endif
