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
#ifndef Mezz_Filesystem_SpecialDirectoryUtilitiesTests_h
#define Mezz_Filesystem_SpecialDirectoryUtilitiesTests_h

/// @file
/// @brief This file tests of some utilities that work with the special platform directories.

#include "MezzTest.h"

#include "SpecialDirectoryUtilities.h"

AUTOMATIC_TEST_GROUP(SpecialDirectoryUtilitiesTests,SpecialDirectoryUtilities)
{
    using namespace Mezzanine;

#ifndef MEZZ_CompilerIsEmscripten
    auto GetCommandResults = [](String Cmd) -> String {
        Cmd.append(" > CommandResults.txt");
    SAVE_WARNING_STATE
    SUPPRESS_GCC_WARNING("-Wunused-result")
        system( Cmd.data() );
    RESTORE_WARNING_STATE
        std::ifstream ResultFile("CommandResults.txt");
        String Ret( std::istreambuf_iterator<char>(ResultFile), {} );
        if( !Ret.empty() ) {
            while( StringTools::IsNewline( Ret.back() ) || StringTools::IsSpace( Ret.back() ) ) {
                Ret.pop_back();
            }
        }
        return Ret;
    };
#endif

    {//Executable Directory
        char* ExeOnly = const_cast<char *>("game.exe");
        char** ExePtr = &ExeOnly;
        char* ExeDotSlash = const_cast<char *>("./game");
        char** ExeDotSlashPtr = &ExeDotSlash;
        char* ExeWinDotSlash = const_cast<char *>(".\\game");
        char** ExeWinDotSlashPtr = &ExeWinDotSlash;
        char* WinPath = const_cast<char *>("c:\\progra~1\\FunGameDir\\");
        char** WinPathPtr = &WinPath;
        char* PosPath = const_cast<char *>("/usr/bin/");
        char** PosPathPtr = &PosPath;
        char* ReasonableWin = const_cast<char *>("c:\\FunGameDir\\game.exe");
        char** ReasonableWinPtr = &ReasonableWin;
        char* ReasonablePos = const_cast<char *>("/usr/share/bin/game");
        char** ReasonablePosPtr = &ReasonablePos;

        TEST_EQUAL("GetExecutableDirFromArg(int,char**)-DirEmpty",
                   String(""),Filesystem::GetExecutableDirFromArg(0,nullptr));
        TEST_EQUAL("GetExecutableDirFromArg(int,char**)-ExeOnly",
                   String("."),Filesystem::GetExecutableDirFromArg(1,ExePtr));
        TEST_EQUAL("GetExecutableDirFromArg(int,char**)-DothSlash",
                   String("."),Filesystem::GetExecutableDirFromArg(1,ExeDotSlashPtr));
        TEST_EQUAL("GetExecutableDirFromArg(int,char**)-winDotSlash",
                   String("."),Filesystem::GetExecutableDirFromArg(1,ExeWinDotSlashPtr));
        TEST_EQUAL("GetExecutableDirFromArg(int,char**)-NoExeWin",
                   String(""),Filesystem::GetExecutableDirFromArg(1,WinPathPtr));
        TEST_EQUAL("GetExecutableDirFromArg(int,char**)-NoExePos",
                   String(""),Filesystem::GetExecutableDirFromArg(1,PosPathPtr));
        TEST_EQUAL("GetExecutableDirFromArg(int,char**)-ValidWin",
                   String("c:\\FunGameDir\\"),Filesystem::GetExecutableDirFromArg(1,ReasonableWinPtr));
        TEST_EQUAL("GetExecutableDirFromArg(int,char**)-ValidPos",
                   String("/usr/share/bin/"),Filesystem::GetExecutableDirFromArg(1,ReasonablePosPtr));

        Filesystem::CacheMainArgs(0,nullptr);
        TEST_EQUAL("GetExecutableDirFromArg()-DirEmpty",
                   String(""),Filesystem::GetExecutableDirFromArg());
        Filesystem::CacheMainArgs(1,ExePtr);
        TEST_EQUAL("GetExecutableDirFromArg()-ExeOnly",
                   String("."),Filesystem::GetExecutableDirFromArg());
        Filesystem::CacheMainArgs(1,ExeDotSlashPtr);
        TEST_EQUAL("GetExecutableDirFromArg()-DothSlash",
                   String("."),Filesystem::GetExecutableDirFromArg());
        Filesystem::CacheMainArgs(1,ExeWinDotSlashPtr);
        TEST_EQUAL("GetExecutableDirFromArg()-winDotSlash",
                   String("."),Filesystem::GetExecutableDirFromArg());
        Filesystem::CacheMainArgs(1,WinPathPtr);
        TEST_EQUAL("GetExecutableDirFromArg()-NoExeWin",
                   String(""),Filesystem::GetExecutableDirFromArg());
        Filesystem::CacheMainArgs(1,PosPathPtr);
        TEST_EQUAL("GetExecutableDirFromArg()-NoExePos",
                   String(""),Filesystem::GetExecutableDirFromArg());
        Filesystem::CacheMainArgs(1,ReasonableWinPtr);
        TEST_EQUAL("GetExecutableDirFromArg()-ValidWin",
                   String("c:\\FunGameDir\\"),Filesystem::GetExecutableDirFromArg());
        Filesystem::CacheMainArgs(1,ReasonablePosPtr);
        TEST_EQUAL("GetExecutableDirFromArg()-ValidPos",
                   String("/usr/share/bin/"),Filesystem::GetExecutableDirFromArg());

        /*
        // Benchmark Tests
        Whole Count = 10000; // set to 100000 or higher and remark out env test for faster results
        MaxInt ArgTime = 0;
        MaxInt SyscallTime = 0;
        MaxInt GetTime = 0;

        TestOutput << "Not testing GetExecutableDirFromArg(ArgC,ArgV) but here is the output so you can check if you want" << std::endl
             << "On your system with the real args this provides:\n\t\"" << Filesystem::GetExecutableDirFromArg(GetMainArgumentCount(), GetMainArgumentVector()) << "\"" << std::endl
             << std::endl;
        {
            std::vector<String> CacheDefeater;
            CacheDefeater.reserve(Count);
            TestOutput << "Calling GetExecutableDirFromArg(ArgC,ArgV) " << Count << " times and timing it." << std::endl;
            TimedTest Timed;
            for(Whole C=0; C<Count; C++)
                { CacheDefeater.push_back(Filesystem::GetExecutableDirFromArg(GetMainArgumentCount(),GetMainArgumentVector())); }
            Whole N = (rand()%Count);
            TestOutput << "To defeat the cache the " << N << "th call gave us \"" << CacheDefeater[N] << "\" and took ";
            ArgTime = Timed.GetLength();
            TestOutput << ArgTime << " microseconds." << std::endl << std::endl;
        }

        TestOutput << "Not testing GetExecutableDirFromSystem() but here is the output so you can check if you want" << std::endl
             << "On your system this provides:\n\t\"" << Filesystem::GetExecutableDirFromSystem() << "\"" << std::endl
             << std::endl;
        {
            std::vector<String> CacheDefeater;
            CacheDefeater.reserve(Count);
            TestOutput << "Calling GetExecutableDirFromSystem() " << Count << " times and timing it." << std::endl;
            TimedTest Timed;
            for(Whole C=0; C<Count; C++)
                { CacheDefeater.push_back(Filesystem::GetExecutableDirFromSystem()); }
            Whole N = (rand()%Count);
            TestOutput << "To defeat the cache the " << N << "th call gave us \"" << CacheDefeater[N] << "\" and took ";
            SyscallTime = Timed.GetLength();
            TestOutput << SyscallTime << " microseconds." << std::endl << std::endl;
        }


        TestOutput << "Not testing GetExecutableDir(ArgC,ArgV) but here is the output so you can check if you want" << std::endl
             << "On your system this provides:\n\t\"" << Filesystem::GetExecutableDir(GetMainArgumentCount(),GetMainArgumentVector()) << "\"" << std::endl
             << std::endl << std::endl;
        {
            std::vector<String> CacheDefeater;
            CacheDefeater.reserve(Count);
            TestOutput << "Calling GetExecutableDir(ArgC,ArgV) " << Count << " times and timing it." << std::endl;
            TimedTest Timed;
            for(Whole C=0; C<Count; C++)
                { CacheDefeater.push_back(Filesystem::GetExecutableDir(GetMainArgumentCount(),GetMainArgumentVector())); }
            Whole N = (rand()%Count);
            TestOutput << "To defeat the cache the " << N << "th call gave us \"" << CacheDefeater[N] << "\" and took ";
            GetTime = Timed.GetLength();
            TestOutput << GetTime << " microseconds." << std::endl << std::endl;
        }
        TEST_WARN( ArgTime < SyscallTime,"ArgIsFastest");
        */
    }//Executable Directory

    {//Working Directory
        if( Filesystem::CreateDirectory("Change") == true ) {
        #if defined(MEZZ_CompilerIsEmscripten)
            // Emscripten seems to build it's filesystem on the fly as you interact with it.
            // So not even root exists when queried with the system call.
            // Curiously, all the other tests for the working directory work as expected.
            const String CmdOriginDir = "/";
        #elif defined(MEZZ_Windows)
            const String CmdOriginDir = GetCommandResults("cd");
        #else
            const String CmdOriginDir = GetCommandResults("pwd");
        #endif
            const String OriginDir = Filesystem::GetWorkingDirectory();
            TEST_EQUAL("GetWorkingDirectory()-OriginDir",
                       CmdOriginDir,OriginDir);

            String TargetDir = OriginDir;
            if( !Filesystem::IsDirectorySeparator( TargetDir.back() ) ) {
                TargetDir.push_back( Filesystem::GetDirectorySeparator_Host() );
            }
            TargetDir.append("Change");

            Filesystem::ChangeWorkingDirectory("Change");
            TEST_EQUAL("ChangeWorkingDirectory(const_StringView)-ChangeRelative",
                       TargetDir,Filesystem::GetWorkingDirectory());

            Filesystem::ChangeWorkingDirectory("..");
            TEST_EQUAL("ChangeWorkingDirectory(const_StringView)-DotDot",
                       OriginDir,Filesystem::GetWorkingDirectory());

            Filesystem::ChangeWorkingDirectory(TargetDir);
            TEST_EQUAL("ChangeWorkingDirectory(const_StringView)-ChangeAbsolute",
                       TargetDir,Filesystem::GetWorkingDirectory());

            Filesystem::ChangeWorkingDirectory(OriginDir);
            TEST_EQUAL("ChangeWorkingDirectory(const_StringView)-OriginAbsolute",
                       OriginDir,Filesystem::GetWorkingDirectory());

            if( Filesystem::RemoveDirectory("Change") == false ) {
                TEST_RESULT("WorkingDirectory-CleanupFailed",Testing::TestResult::Failed);
            }
        }else{
            TEST_RESULT("WorkingDirectory-CreateDir",Testing::TestResult::Failed);
        }
    }//Working Directory

#ifdef MEZZ_CompilerIsEmscripten
    {//AppData Directories
        TEST_RESULT("AppDataDirectories",Testing::TestResult::Skipped);
    }//AppData Directories
#else
    {//AppData Directories
    #ifdef MEZZ_Windows
        const String LocalAppDataDir = GetCommandResults("echo %localappdata%");
        TEST_EQUAL("GetLocalAppDataDir()",LocalAppDataDir,Filesystem::GetLocalAppDataDir());

        const String ShareAppDataDir = GetCommandResults("echo %appdata%");
        TEST_EQUAL("GetShareableAppDataDir()",ShareAppDataDir,Filesystem::GetShareableAppDataDir());

        const String CurrentUserAppDataDir = GetCommandResults("echo %userprofile%");
        TEST_EQUAL("GetCurrentUserDataDir()",CurrentUserAppDataDir,Filesystem::GetCurrentUserDataDir());

        const String CommonUserAppDataDir = GetCommandResults("echo %public%");
        TEST_EQUAL("GetCommonUserDataDir()",CommonUserAppDataDir,Filesystem::GetCommonUserDataDir());
    #else
        String AppDataDir = GetCommandResults("echo ~");

        TEST_EQUAL("GetLocalAppDataDir()",AppDataDir,Filesystem::GetLocalAppDataDir());
        TEST_EQUAL("GetShareableAppDataDir()",AppDataDir,Filesystem::GetShareableAppDataDir());
        TEST_EQUAL("GetCurrentUserDataDir()",AppDataDir,Filesystem::GetCurrentUserDataDir());
        TEST_EQUAL("GetCommonUserDataDir()",AppDataDir,Filesystem::GetCommonUserDataDir());
    #endif
    }//AppData Directories
#endif
}

/*
/// @brief asks a few question about the location of the executable dir, because otherwise we don't know
void RunInteractiveTests()
{
    Logger Temp;

    Temp << "GetExecutableDirFromSystem() - On your system with the real args this provides:\n\t\""
         << Filesystem::GetExecutableDirFromSystem() << "\"" << std::endl
         << "Is that location correct? " ;
    TEST_RESULT(GetTestAnswerFromStdin(Temp.str()), "GetExecutableDirFromSystem()");

    Temp.str("");
    Temp << "GetExecutableDir(ArgC,ArgV) - On your system with the real args this provides:\n\t\""
         << Filesystem::GetExecutableDir(GetMainArgumentCount(),GetMainArgumentVector()) << "\"" << std::endl
         << "Is that location correct? " ;
    TEST_RESULT(GetTestAnswerFromStdin(Temp.str()), "GetExecutableDir(ArgC,ArgV)");
}
*/

#endif
