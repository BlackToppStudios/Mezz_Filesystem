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
#ifndef Mezz_Filesystem_PathUtilitiesTests_h
#define Mezz_Filesystem_PathUtilitiesTests_h

/// @file
/// @brief This file tests utilities that work with Strings relating to a resource path.

#include "MezzTest.h"

#include "PathUtilities.h"
#include "MezzException.h"

AUTOMATIC_TEST_GROUP(PathUtilitiesTests,PathUtilities)
{
    using namespace Mezzanine;

    {// Dir and Base Name
        TEST_EQUAL("GetDirName(const_char*)-ShouldBeEmptyCStr",
                   "",Filesystem::GetDirName("File.txt"))
        TEST_EQUAL("GetDirName(const_char*)-WindowsCStr",
                   "c:\\",Filesystem::GetDirName("c:\\untitled doc.txt"))
        TEST_EQUAL("GetDirName(const_char*)-EmptyNotCrashCStr",
                   "",Filesystem::GetDirName(""))
        TEST_EQUAL("GetDirName(const_char*)-UnixCStr",
                   "/a/b/",Filesystem::GetDirName("/a/b/c"))
        TEST_EQUAL("GetDirName(const_char*)-UnixDirCStr",
                   "/a/b/c/",Filesystem::GetDirName("/a/b/c/"))
        TEST_EQUAL("GetDirName(const_StringView)-ShouldBeEmpty",
                   "",Filesystem::GetDirName(String("File.txt")))
        TEST_EQUAL("GetDirName(const_StringView)-Windows",
                   "c:\\",Filesystem::GetDirName(String("c:\\untitled doc.txt")))
        TEST_EQUAL("GetDirName(const_StringView)-EmptyNotCrash",
                   "",Filesystem::GetDirName(String("")))
        TEST_EQUAL("GetDirName(const_StringView)-Unix",
                   "/a/b/",Filesystem::GetDirName(String("/a/b/c")))
        TEST_EQUAL("GetDirName(const_StringView)-UnixDir",
                   "/a/b/c/",Filesystem::GetDirName(String("/a/b/c/")))

        TEST_EQUAL("GetBaseName(const_char*)-ShouldCopyCStr",
                   "File.txt",Filesystem::GetBaseName("File.txt"))
        TEST_EQUAL("GetBaseName(const_char*)-WindowsCStr",
                   "untitled doc.txt",Filesystem::GetBaseName("c:\\untitled doc.txt"))
        TEST_EQUAL("GetBaseName(const_char*)-EmptyNotCrashCStr",
                   "",Filesystem::GetBaseName(""))
        TEST_EQUAL("GetBaseName(const_char*)-UnixCStr",
                   "c",Filesystem::GetBaseName("/a/b/c"))
        TEST_EQUAL("GetBaseName(const_char*)-UnixDirCStr",
                   "",Filesystem::GetBaseName("/a/b/c/"))
        TEST_EQUAL("GetBaseName(const_StringView)-ShouldCopy",
                   "File.txt",Filesystem::GetBaseName(String("File.txt")))
        TEST_EQUAL("GetBaseName(const_StringView)-Windows",
                   "untitled doc.txt",Filesystem::GetBaseName(String("c:\\untitled doc.txt")))
        TEST_EQUAL("GetBaseName(const_StringView)-EmptyNotCrash",
                   "",Filesystem::GetBaseName(String("")))
        TEST_EQUAL("GetBaseName(const_StringView)-Unix",
                   "c",Filesystem::GetBaseName(String("/a/b/c")))
        TEST_EQUAL("GetBaseName(const_StringView)-UnixDir",
                   "",Filesystem::GetBaseName(String("/a/b/c/")))
    }// Dir and Base Name

    {// Dot Segment Checks
        TEST_EQUAL("IsDotSegment(const_StringView)-SingleDot",true,Filesystem::IsDotSegment("."))
        TEST_EQUAL("IsDotSegment(const_StringView)-DoubleDot",true,Filesystem::IsDotSegment(".."))
        TEST_EQUAL("IsDotSegment(const_StringView)-NotADot",false,Filesystem::IsDotSegment("Hello"))
    }// Dot Segment Checks

    {// Separators
        TEST_EQUAL("IsDirectorySeparator(const_T)-Pass",true,Filesystem::IsDirectorySeparator('/'))
        TEST_EQUAL("IsDirectorySeparator(const_T)-Fail",false,Filesystem::IsDirectorySeparator('@'))
        TEST_EQUAL("IsDirectorySeparator_Posix(const_T)-Pass",true,Filesystem::IsDirectorySeparator_Posix('/'))
        TEST_EQUAL("IsDirectorySeparator_Posix(const_T)-Fail",false,Filesystem::IsDirectorySeparator_Posix('\\'))
        TEST_EQUAL("IsDirectorySeparator_Windows(const_T)-Pass",true,Filesystem::IsDirectorySeparator_Windows('\\'))
        TEST_EQUAL("IsDirectorySeparator_Windows(const_T)-Fail",false,Filesystem::IsDirectorySeparator_Windows('/'))
        TEST_EQUAL("GetDirectorySeparator_Posix()",'/',Filesystem::GetDirectorySeparator_Posix())
        TEST_EQUAL("GetDirectorySeparator_Windows()",'\\',Filesystem::GetDirectorySeparator_Windows())
        TEST_EQUAL("GetDirectorySeparator_Universal()",'/',Filesystem::GetDirectorySeparator_Universal())
    #ifdef MEZZ_Windows
        TEST_EQUAL("IsDirectorySeparator_Host(const_T)-Pass",true,Filesystem::IsDirectorySeparator_Host('\\'))
        TEST_EQUAL("IsDirectorySeparator_Host(const_T)-Fail",false,Filesystem::IsDirectorySeparator_Host('/'))
        TEST_EQUAL("GetDirectorySeparator_Host()",'\\',Filesystem::GetDirectorySeparator_Host())
    #else
        TEST_EQUAL("IsDirectorySeparator_Host(const_T)-Pass",true,Filesystem::IsDirectorySeparator_Host('/'))
        TEST_EQUAL("IsDirectorySeparator_Host(const_T)-Fail",false,Filesystem::IsDirectorySeparator_Host('\\'))
        TEST_EQUAL("GetDirectorySeparator_Host()",'/',Filesystem::GetDirectorySeparator_Host())
    #endif

        TEST_EQUAL("IsPathSeparator(const_T)-Pass",true,Filesystem::IsPathSeparator(';'))
        TEST_EQUAL("IsPathSeparator(const_T)-Fail",false,Filesystem::IsPathSeparator('&'))
        TEST_EQUAL("IsPathSeparator_Posix(const_T)-Pass",true,Filesystem::IsPathSeparator_Posix(':'))
        TEST_EQUAL("IsPathSeparator_Posix(const_T)-Fail",false,Filesystem::IsPathSeparator_Posix(';'))
        TEST_EQUAL("IsPathSeparator_Windows(const_T)-Pass",true,Filesystem::IsPathSeparator_Windows(';'))
        TEST_EQUAL("IsPathSeparator_Windows(const_T)-Fail",false,Filesystem::IsPathSeparator_Windows(':'))
        TEST_EQUAL("GetPathSeparator_Posix()",':',Filesystem::GetPathSeparator_Posix())
        TEST_EQUAL("GetPathSeparator_Windows()",';',Filesystem::GetPathSeparator_Windows())
    #ifdef MEZZ_Windows
        TEST_EQUAL("IsPathSeparator_Host(const_T)-Pass",true,Filesystem::IsPathSeparator_Host(';'))
        TEST_EQUAL("IsPathSeparator_Host(const_T)-Fail",false,Filesystem::IsPathSeparator_Host(':'))
        TEST_EQUAL("GetPathSeparator_Host()",';',Filesystem::GetPathSeparator_Host())
    #else
        TEST_EQUAL("IsPathSeparator_Host(const_T)-Pass",true,Filesystem::IsPathSeparator_Host(':'))
        TEST_EQUAL("IsPathSeparator_Host(const_T)-Fail",false,Filesystem::IsPathSeparator_Host(';'))
        TEST_EQUAL("GetPathSeparator_Host()",':',Filesystem::GetPathSeparator_Host())
    #endif
    }// Separators

    {// Absolute and Relative Paths
        const String WinAbsPath("C:\\MinGW\\");
        const String PosixAbsPath("/etc/IDontEvenKnow/");
        const String WinDotRelPath(".\\SubDir\\");
        const String DotRelPath("./SubDir/");
        const String NadaRelPath("SubDir/");

        TEST_EQUAL("IsPathAbsolute(const_StringView)-Pass",
                   true,Filesystem::IsPathAbsolute(WinAbsPath))
        TEST_EQUAL("IsPathAbsolute(const_StringView)-Fail",
                   false,Filesystem::IsPathAbsolute(DotRelPath))
        TEST_EQUAL("IsPathAbsolute_Posix(const_StringView)-Pass",
                   true,Filesystem::IsPathAbsolute_Posix(PosixAbsPath))
        TEST_EQUAL("IsPathAbsolute_Posix(const_StringView)-Fail",
                   false,Filesystem::IsPathAbsolute_Posix(NadaRelPath))
        TEST_EQUAL("IsPathAbsolute_Windows(const_StringView)-Pass",
                   true,Filesystem::IsPathAbsolute_Windows(WinAbsPath))
        TEST_EQUAL("IsPathAbsolute_Windows(const_StringView)-Fail",
                   false,Filesystem::IsPathAbsolute_Windows(WinDotRelPath))

        TEST_EQUAL("IsPathRelative(const_StringView)-Pass",
                   true,Filesystem::IsPathRelative(NadaRelPath))
        TEST_EQUAL("IsPathRelative(const_StringView)-Fail",
                   false,Filesystem::IsPathRelative(WinAbsPath))
        TEST_EQUAL("IsPathRelative_Posix(const_StringView)-Pass",
                   true,Filesystem::IsPathRelative_Posix(DotRelPath))
        TEST_EQUAL("IsPathRelative_Posix(const_StringView)-Fail",
                   false,Filesystem::IsPathRelative_Posix(PosixAbsPath))
        TEST_EQUAL("IsPathRelative_Windows(const_StringView)-Pass",
                   true,Filesystem::IsPathRelative_Windows(WinDotRelPath))
        TEST_EQUAL("IsPathRelative_Windows(const_StringView)-Fail",
                   false,Filesystem::IsPathRelative_Windows(PosixAbsPath))

    #ifdef MEZZ_Windows
        TEST_EQUAL("IsPathAbsolute_Host(const_StringView)-Pass",
                   true,Filesystem::IsPathAbsolute_Host(WinAbsPath))
        TEST_EQUAL("IsPathAbsolute_Host(const_StringView)-Fail",
                   false,Filesystem::IsPathAbsolute_Host(PosixAbsPath))
        TEST_EQUAL("IsPathRelative_Host(const_StringView)-Pass",
                   true,Filesystem::IsPathRelative_Host(WinDotRelPath))
        TEST_EQUAL("IsPathRelative_Host(const_StringView)-Fail",
                   false,Filesystem::IsPathRelative_Host(PosixAbsPath))
    #else
        TEST_EQUAL("IsPathAbsolute_Host(const_StringView)-Pass",
                   true,Filesystem::IsPathAbsolute_Host(PosixAbsPath))
        TEST_EQUAL("IsPathAbsolute_Host(const_StringView)-Fail",
                   false,Filesystem::IsPathAbsolute_Host(WinAbsPath))
        TEST_EQUAL("IsPathRelative_Host(const_StringView)-Pass",
                   true,Filesystem::IsPathRelative_Host(DotRelPath))
        TEST_EQUAL("IsPathRelative_Host(const_StringView)-Fail",
                   false,Filesystem::IsPathRelative_Host(PosixAbsPath))
    #endif
    }// Absolute and Relative Paths

    {// Path Checks
        const String DepthDirOne("C:\\FirstDir\\SecondDir\\ThirdDir\\");
        const String DepthDirTwo(".\\FirstDir\\..\\..\\..\\");
        const String DepthDirThree("../../FirstDir/SecondDir/");
        const String DepthDirFour("/user/home/./");

        TEST_EQUAL("GetDirectoryDepth_Windows(const_StringView,const_Boole)-First",
                   3,Filesystem::GetDirectoryDepth_Windows(DepthDirOne,true))
        TEST_EQUAL("GetDirectoryDepth_Windows(const_StringView,const_Boole)-Second",
                   3,Filesystem::GetDirectoryDepth_Windows(DepthDirOne,false))
        TEST_EQUAL("GetDirectoryDepth_Windows(const_StringView,const_Boole)-Third",
                   -1,Filesystem::GetDirectoryDepth_Windows(DepthDirTwo,true))
        TEST_EQUAL("GetDirectoryDepth_Windows(const_StringView,const_Boole)-Fourth",
                   -2,Filesystem::GetDirectoryDepth_Windows(DepthDirTwo,false))

        TEST_EQUAL("GetDirectoryDepth_Posix(const_StringView,const_Boole)-First",
                   -1,Filesystem::GetDirectoryDepth_Posix(DepthDirThree,true))
        TEST_EQUAL("GetDirectoryDepth_Posix(const_StringView,const_Boole)-Second",
                   0,Filesystem::GetDirectoryDepth_Posix(DepthDirThree,false))
        TEST_EQUAL("GetDirectoryDepth_Posix(const_StringView,const_Boole)-Third",
                   2,Filesystem::GetDirectoryDepth_Posix(DepthDirFour,true))
        TEST_EQUAL("GetDirectoryDepth_Posix(const_StringView,const_Boole)-Fourth",
                   2,Filesystem::GetDirectoryDepth_Posix(DepthDirFour,false))

    #ifdef MEZZ_Windows
        const String DepthDirFive("MyDir\\..\\..\\MyOtherDir\\");

        TEST_EQUAL("GetDirectoryDepth_Host(const_StringView)-First",
                   -1,Filesystem::GetDirectoryDepth_Host(DepthDirFive,true))
        TEST_EQUAL("GetDirectoryDepth_Host(const_StringView)-Second",
                   0,Filesystem::GetDirectoryDepth_Host(DepthDirFive,false))
    #else
        const String DepthDirFive("Games/TheGame/../../../NotGames/NotTheGame/");

        TEST_EQUAL("GetDirectoryDepth_Host(const_StringView)-First",
                   -1,Filesystem::GetDirectoryDepth_Host(DepthDirFive,true))
        TEST_EQUAL("GetDirectoryDepth_Host(const_StringView)-Second",
                   1,Filesystem::GetDirectoryDepth_Host(DepthDirFive,false))
    #endif

        const String BaseDirOne("/user/home/");
        const String BaseDirTwo("home/");
        const String BaseDirThree("C:\\MinGW\\bin\\");
        const String BaseDirFour("MyDocs\\Code\\");

        TEST_EQUAL("IsSubPath_Posix(const_StringView,const_StringView)-First-Pass",
                   true,Filesystem::IsSubPath_Posix(BaseDirOne,"/user/home/code/"))
        TEST_EQUAL("IsSubPath_Posix(const_StringView,const_StringView)-First-Fail",
                   false,Filesystem::IsSubPath_Posix(BaseDirOne,"/etc/home/code/"))
        TEST_EQUAL("IsSubPath_Posix(const_StringView,const_StringView)-Second-Pass",
                   true,Filesystem::IsSubPath_Posix(BaseDirTwo,"home/code/"))
        TEST_EQUAL("IsSubPath_Posix(const_StringView,const_StringView)-Second-Fail",
                   false,Filesystem::IsSubPath_Posix(BaseDirTwo,"home/../"))

        TEST_EQUAL("IsSubPath_Windows(const_StringView,const_StringView)-First-Pass",
                   true,Filesystem::IsSubPath_Windows(BaseDirThree,"C:\\MinGW\\bin\\include\\"))
        TEST_EQUAL("IsSubPath_Windows(const_StringView,const_StringView)-First-Fail",
                   false,Filesystem::IsSubPath_Windows(BaseDirThree,"C:\\System32\\"))
        TEST_EQUAL("IsSubPath_Windows(const_StringView,const_StringView)-Second-Pass",
                   true,Filesystem::IsSubPath_Windows(BaseDirFour,"MyDocs\\Code\\Project\\"))
        TEST_EQUAL("IsSubPath_Windows(const_StringView,const_StringView)-Second-Fail",
                   false,Filesystem::IsSubPath_Windows(BaseDirFour,"MyDocs\\Code\\..\\"))

    #ifdef MEZZ_Windows
        const String BaseDirFive("C:\\");

        TEST_EQUAL("IsSubPath_Host(const_StringView,const_StringView)-Pass",
                   true,Filesystem::IsSubPath_Host(BaseDirFive,"C:\\Users\\Me\\"))
        TEST_EQUAL("IsSubPath_Host(const_StringView,const_StringView)-Fail",
                   false,Filesystem::IsSubPath_Host(BaseDirFive,"C:\\"))
    #else
        const String BaseDirFive("/mnt/Cool Stuff/");

        TEST_EQUAL("IsSubPath_Host(const_StringView,const_StringView)-Pass",
                   true,Filesystem::IsSubPath_Host(BaseDirFive,"/mnt/Cool Stuff/Neat/"))
        TEST_EQUAL("IsSubPath_Host(const_StringView,const_StringView)-Fail",
                   false,Filesystem::IsSubPath_Host(BaseDirFive,"/mnt/Mildly Interesting/"))
    #endif

        TEST_THROW("IsSubPath_Posix(const_StringView,const_StringView)-Absolute/Relative-Throw",
                   Mezzanine::Exception::AbsoluteRelativeComparison,
                   [&](){ static_cast<void>( Filesystem::IsSubPath_Posix(BaseDirOne,"relative/path/") ); })
        TEST_THROW("IsSubPath_Posix(const_StringView,const_StringView)-Relative/Absolute-Throw",
                   Mezzanine::Exception::AbsoluteRelativeComparison,
                   [&](){ static_cast<void>( Filesystem::IsSubPath_Posix(BaseDirTwo,"/absolute/path/") ); })
        TEST_THROW("IsSubPath_Windows(const_StringView,const_StringView)-Absolute/Relative-Throw",
                   Mezzanine::Exception::AbsoluteRelativeComparison,
                   [&](){ static_cast<void>( Filesystem::IsSubPath_Windows(BaseDirThree,"relative\\path\\") ); })
        TEST_THROW("IsSubPath_Windows(const_StringView,const_StringView)-Relative/Absolute-Throw",
                   Mezzanine::Exception::AbsoluteRelativeComparison,
                   [&](){ static_cast<void>( Filesystem::IsSubPath_Windows(BaseDirFour,"C:\\absolute\\path\\") ); })
    }// Path Checks

    {// Path Utilities
        StringVector PathBuildOne = { "Users", "Person", "Code", "Project" };
        StringVector PathBuildTwo = { "user", "home", "code" };
        StringVector PathBuildThree = { "data", "common" };
        const String PathBuildOneResult("C:\\Users\\Person\\Code\\Project\\TestFile.txt");
        const String PathBuildTwoResult("/user/home/code/");
        const String PathBuildThreeResultWin("data\\common\\TestJPG.jpg");
        const String PathBuildThreeResultPos("data/common/TestJPG.jpg");

        TEST_EQUAL("BuildPath_Windows(const_StringView,const_StringVector&,const_StringView,const_Boole)-First",
                   PathBuildOneResult,Filesystem::BuildPath_Windows("C:\\",PathBuildOne,"TestFile.txt"))
        TEST_EQUAL("BuildPath_Posix(const_StringView,const_StringVector&,const_StringView,const_Boole)-First",
                   PathBuildTwoResult,Filesystem::BuildPath_Posix("/",PathBuildTwo,String()))
        TEST_EQUAL("BuildPath_Windows(const_StringView,const_StringVector&,const_StringView,const_Boole)-Second",
                   PathBuildThreeResultWin,Filesystem::BuildPath_Windows(String(),PathBuildThree,"TestJPG.jpg"))
        TEST_EQUAL("BuildPath_Posix(const_StringView,const_StringVector&,const_StringView,const_Boole)-Second",
                   PathBuildThreeResultPos,Filesystem::BuildPath_Posix(String(),PathBuildThree,"TestJPG.jpg"))

    #ifdef MEZZ_Windows
        StringVector PathBuildFour = { "User", "Pictures", "Old" };
        const String PathBuildFourAbsResult("C:\\User\\Pictures\\Old\\Me.jpg");
        const String PathBuildFourRelResult("User\\Pictures\\Old\\Them.jpg");

        TEST_EQUAL("BuildPath_Host(const_StringView,const_StringVector&,const_StringView)-Pass",
                   PathBuildFourAbsResult,Filesystem::BuildPath_Host("C:\\",PathBuildFour,"Me.jpg"))
        TEST_EQUAL("BuildPath_Host(const_StringView,const_StringVector&,const_StringView)-Fail",
                   PathBuildFourRelResult,Filesystem::BuildPath_Host(String(),PathBuildFour,"Them.jpg"))
    #else
        StringVector PathBuildFour = { "mnt", "media", "anime" };
        const String PathBuildFourAbsResult("/mnt/media/anime/TheBest.mkv");
        const String PathBuildFourRelResult("mnt/media/anime/TheOk-est.mkv");

        TEST_EQUAL("BuildPath_Host(const_StringView,const_StringVector&,const_StringView)-Pass",
                   PathBuildFourAbsResult,Filesystem::BuildPath_Host("/",PathBuildFour,"TheBest.mkv"))
        TEST_EQUAL("BuildPath_Host(const_StringView,const_StringVector&,const_StringView)-Fail",
                   PathBuildFourRelResult,Filesystem::BuildPath_Host(String(),PathBuildFour,"TheOk-est.mkv"))
    #endif

        const String DotSegPathOne("./data/common/");
        const String DotSegPathOneResult("data/common/");
        const String DotSegPathTwo("OtherData/./common/license/../../");
        const String DotSegPathTwoResult("OtherData/");
        const String DotSegPathThree("MoreData\\..\\.\\..\\");
        const String DotSegPathThreeResult("..\\");
        const String DotSegPathFour("C:\\Users\\..\\..\\");
        const String DotSegPathFourResult("C:\\");

        TEST_EQUAL("RemoveDotSegments_Posix(const_StringView)-First",
                   DotSegPathOneResult,Filesystem::RemoveDotSegments_Posix(DotSegPathOne))
        TEST_EQUAL("RemoveDotSegments_Posix(const_StringView)-Second",
                   DotSegPathTwoResult,Filesystem::RemoveDotSegments_Posix(DotSegPathTwo))
        TEST_EQUAL("RemoveDotSegments_Windows(const_StringView)-First",
                   DotSegPathThreeResult,Filesystem::RemoveDotSegments_Windows(DotSegPathThree))
        TEST_EQUAL("RemoveDotSegments_Windows(const_StringView)-Second",
                   DotSegPathFourResult,Filesystem::RemoveDotSegments_Windows(DotSegPathFour))

    #ifdef MEZZ_Windows
        const String DotSegPathFive("C:\\.\\Folder\\.\\..\\");
        const String DotSegPathFiveResult("C:\\");
        const String DotSegPathSix("SubDir\\SubSubDir\\..\\.\\MyDir\\");
        const String DotSegPathSixResult("SubDir\\MyDir\\");

        TEST_EQUAL("RemoveDotSegments_Host(const_StringView)-First",
                   DotSegPathFiveResult,Filesystem::RemoveDotSegments_Host(DotSegPathFive))
        TEST_EQUAL("RemoveDotSegments_Host(const_StringView)-Second",
                   DotSegPathSixResult,Filesystem::RemoveDotSegments_Host(DotSegPathSix))
    #else
        const String DotSegPathFive("/./Folder/./../");
        const String DotSegPathFiveResult("/");
        const String DotSegPathSix("SubDir/SubSubDir/.././MyDir/");
        const String DotSegPathSixResult("SubDir/MyDir/");

        TEST_EQUAL("RemoveDotSegments_Host(const_StringView)-First",
                   DotSegPathFiveResult,Filesystem::RemoveDotSegments_Host(DotSegPathFive))
        TEST_EQUAL("RemoveDotSegments_Host(const_StringView)-Second",
                   DotSegPathSixResult,Filesystem::RemoveDotSegments_Host(DotSegPathSix))
    #endif

        const String CombinePathOne("C:\\Users\\Person\\Desktop\\");
        const String CombinePathTwo("home/");
        const String CombinePathThree("/etc/dir");
        const String CombineNameOne("TestText.txt");
        const String CombineNameTwo("TestJPG.jpg");

        TEST_EQUAL("CombinePathAndFileName_Windows(const_StringView,const_StringView)-First",
                   String("C:\\Users\\Person\\Desktop\\TestText.txt"),
                   Filesystem::CombinePathAndFileName_Windows(CombinePathOne,CombineNameOne))
        TEST_EQUAL("CombinePathAndFileName_Windows(const_StringView,const_StringView)-Second",
                   String("C:\\Users\\Person\\Desktop\\TestJPG.jpg"),
                   Filesystem::CombinePathAndFileName_Windows(CombinePathOne,CombineNameTwo))
        TEST_EQUAL("CombinePathAndFileName_Posix(const_StringView,const_StringView)-First",
                   String("home/TestText.txt"),
                   Filesystem::CombinePathAndFileName_Posix(CombinePathTwo,CombineNameOne))
        TEST_EQUAL("CombinePathAndFileName_Posix(const_StringView,const_StringView)-Second",
                   String("home/TestJPG.jpg"),
                   Filesystem::CombinePathAndFileName_Posix(CombinePathTwo,CombineNameTwo))
        TEST_EQUAL("CombinePathAndFileName_Posix(const_StringView,const_StringView)-Third",
                   String("/etc/dir/TestText.txt"),
                   Filesystem::CombinePathAndFileName_Posix(CombinePathThree,CombineNameOne))
        TEST_EQUAL("CombinePathAndFileName_Posix(const_StringView,const_StringView)-Fourth",
                   String("/etc/dir/TestJPG.jpg"),
                   Filesystem::CombinePathAndFileName_Posix(CombinePathThree,CombineNameTwo))

    #ifdef MEZZ_Windows
        const String CombinePathFour("MyDocuments");

        TEST_EQUAL("CombinePathAndFileName_Host(const_StringView,const_StringView)-First",
                   String("MyDocuments\\TestText.txt"),
                   Filesystem::CombinePathAndFileName_Host(CombinePathFour,CombineNameOne))
        TEST_EQUAL("CombinePathAndFileName_Host(const_StringView,const_StringView)-Second",
                   String("MyDocuments\\TestJPG.jpg"),
                   Filesystem::CombinePathAndFileName_Host(CombinePathFour,CombineNameTwo))
    #else
        const String CombinePathFour("home");

        TEST_EQUAL("CombinePathAndFileName_Host(const_StringView,const_StringView)-First",
                   String("home/TestText.txt"),
                   Filesystem::CombinePathAndFileName_Host(CombinePathFour,CombineNameOne))
        TEST_EQUAL("CombinePathAndFileName_Host(const_StringView,const_StringView)-Second",
                   String("home/TestJPG.jpg"),
                   Filesystem::CombinePathAndFileName_Host(CombinePathFour,CombineNameTwo))
    #endif
    }// Path Utilities
}

#endif
