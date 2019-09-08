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
#ifndef Mezz_Filesystem_FilesystemManagementTests_h
#define Mezz_Filesystem_FilesystemManagementTests_h

/// @file
/// @brief This file tests utilities related to the creation, moving, and destruction of files and directories.

#include "MezzTest.h"

#include "FilesystemManagement.h"

namespace Mezzanine {
namespace Filesystem {
/// @brief Convenience streaming operator to enable the tests to compile.
/// @param Stream The destination stream.
/// @param Result The result to be streamed.
/// @return Returns a reference to the Stream parameter.
std::ostream& operator<<(std::ostream& Stream, Mezzanine::Filesystem::ModifyResult Result);
std::ostream& operator<<(std::ostream& Stream, Mezzanine::Filesystem::ModifyResult Result)
{
    using InternalType = typename std::underlying_type<Mezzanine::Filesystem::ModifyResult>::type;
    Stream << static_cast<InternalType>( Result );
    return Stream;
}
}//Filesystem
}//Mezzanine

AUTOMATIC_TEST_GROUP(FilesystemManagementTests,FilesystemManagement)
{
    using namespace Mezzanine;

    {// Basic File Management
        {// File Detection Test using ifstream
            std::ifstream UtilityFileDetect;
            UtilityFileDetect.open("./UtilityTestFile.txt");
            if( UtilityFileDetect ) {
                // Uh oh, we didn't clean up from the previous run.  Probably.
                TEST_RESULT("FileManagement-VerifyPreviousRunCleanup",Testing::TestResult::Failed);
            }else{
                TEST_RESULT("FileManagement-VerifyPreviousRunCleanup",Testing::TestResult::Success);
            }
            UtilityFileDetect.close();
        }// File Detection Test using ifstream
        std::ofstream UtilityFile;
        UtilityFile.open("./UtilityTestFile.txt",std::ios_base::out | std::ios_base::trunc);
        UtilityFile << "I exist!";
        UtilityFile.close();

        TEST_EQUAL("FileExists(const_StringView)-PassCheck",
                   true,Filesystem::FileExists("UtilityTestFile.txt"));
        TEST_EQUAL("CopyFile(const_StringView,const_StringView)-Fresh",
                   Filesystem::ModifyResult::Success,
                   Filesystem::CopyFile("UtilityTestFile.txt","UtilityTestCopy.txt",false));
        TEST_EQUAL("CopyFile(const_StringView,const_StringView)-Exists",
                   true,Filesystem::FileExists("UtilityTestCopy.txt"));
        TEST_EQUAL("CopyFile(const_StringView,const_StringView)-DuplicateFail",
                   Filesystem::ModifyResult::AlreadyExists,
                   Filesystem::CopyFile("UtilityTestCopy.txt","UtilityTestFile.txt",true));
        TEST_EQUAL("MoveFile(const_StringView,const_StringView)-CreateDest",
                   Filesystem::ModifyResult::Success,
                   Filesystem::CreateDirectory("MoveTarget/"));
        TEST_EQUAL("MoveFile(const_StringView,const_StringView)-ActualMove",
                   Filesystem::ModifyResult::Success,
                   Filesystem::MoveFile("UtilityTestCopy.txt","MoveTarget/UtilityTestCopy.txt",true));
        TEST_EQUAL("MoveFile(const_StringView,const_StringView)-SourceDoesntExist",
                   false,Filesystem::FileExists("UtilityTestCopy.txt"));
        TEST_EQUAL("MoveFile(const_StringView,const_StringView)-DestExists",
                   true,Filesystem::FileExists("MoveTarget/UtilityTestCopy.txt"));
        TEST_EQUAL("MoveFile(const_StringView,const_StringView)-Rename",
                   Filesystem::ModifyResult::Success,
                   Filesystem::MoveFile("MoveTarget/UtilityTestCopy.txt","MoveTarget/RenamedCopy.txt",true));
        TEST_EQUAL("RemoveFile(const_StringView)-DeleteMoved",
                   Filesystem::ModifyResult::Success,
                   Filesystem::RemoveFile("MoveTarget/RenamedCopy.txt"));
        TEST_EQUAL("RemoveFile(const_StringView)-VerifyMoved",
                   false,Filesystem::FileExists("MoveTarget/RenamedCopy.txt"));
        TEST_EQUAL("RemoveFile(const_StringView)-DeleteOriginal",
                   Filesystem::ModifyResult::Success,
                   Filesystem::RemoveFile("UtilityTestFile.txt"));
        TEST_EQUAL("RemoveFile(const_StringView)-VerifyDeleted",
                   false,Filesystem::FileExists("UtilityTestFile.txt"));
        TEST_EQUAL("MoveFile(const_StringView,const_StringView)-DestroyDest",
                   Filesystem::ModifyResult::Success,
                   Filesystem::RemoveDirectory("MoveTarget"));
    }// Basic File Management

    {// Symlinks
        // No tests for symlinks exist yet.
    }// Symlinks

    {// Basic Directory Management
        String BasePathTestDir("DirTestingDir/");

        // Perform cleanup from any unclean runs of the tests
        Boole Exists = Filesystem::DirectoryExists(BasePathTestDir);
        if( Exists ) {
            TEST_RESULT("DirectoryManagement-VerifyPreviousRunCleanup",Testing::TestResult::Failed);
        }else{
            TEST_RESULT("DirectoryManagement-VerifyPreviousRunCleanup",Testing::TestResult::Success);
        }

        TEST_EQUAL("CreateDirectory(const_StringView)",
                   Filesystem::ModifyResult::Success,
                   Filesystem::CreateDirectory(BasePathTestDir));
        TEST_EQUAL("DirectoryExists(const_StringView)",
                   true,Filesystem::DirectoryExists(BasePathTestDir));

        //const String BaseDir = "./";
        //const String Depth1 = "Depth1/";
        //const String Depth2 = "Depth2/";
        //const String Depth3 = "Depth3/";
        //const String DepthDir = BaseDir + Depth1 + Depth2 + Depth3;
        const String FullPathTestDir = "DirTestingDir/Depth1/Depth2/Depth3";

        TEST_EQUAL("CreateDirectoryPath(const_StringView)",
                   Filesystem::ModifyResult::Success,
                   Filesystem::CreateDirectoryPath(FullPathTestDir));

        //TEST_EQUAL("CreateDirectoryPath(const_StringView)-PreVerify-Depth3",
        //           true,Filesystem::DirectoryExists(FullPathTestDir));
        TEST_EQUAL("RemoveDirectory(const_StringView)-PathDepth3",
                   Filesystem::ModifyResult::Success,
                   Filesystem::RemoveDirectory(FullPathTestDir));
        //TEST_EQUAL("CreateDirectoryPath(const_StringView)-PostVerify-Depth3",
        //           false,Filesystem::DirectoryExists(FullPathTestDir));

        //TEST_EQUAL("CreateDirectoryPath(const_StringView)-PreVerify-Depth2",
        //           true,Filesystem::DirectoryExists("./DirTestingDir/Depth1/Depth2/"));
        TEST_EQUAL("RemoveDirectory(const_StringView)-PathDepth2",
                   Filesystem::ModifyResult::Success,
                   Filesystem::RemoveDirectory("DirTestingDir/Depth1/Depth2"));
        //TEST_EQUAL("CreateDirectoryPath(const_StringView)-PostVerify-Depth2",
        //           false,Filesystem::DirectoryExists("./DirTestingDir/Depth1/Depth2/"));

        //TEST_EQUAL("CreateDirectoryPath(const_StringView)-PreVerify-Depth1",
        //           true,Filesystem::DirectoryExists("./DirTestingDir/Depth1/"));
        TEST_EQUAL("RemoveDirectory(const_StringView)-PathDepth1",
                   Filesystem::ModifyResult::Success,
                   Filesystem::RemoveDirectory("DirTestingDir/Depth1"));
        //TEST_EQUAL("CreateDirectoryPath(const_StringView)-PostVerify-Depth1",
        //           false,Filesystem::DirectoryExists("./DirTestingDir/Depth1"));

        /*TEST_EQUAL("CreateDirectoryPath(const_StringView)",
                   Filesystem::ModifyResult::Success,
                   Filesystem::CreateDirectoryPath(FullPathTestDir));
        TEST_EQUAL("CreateDirectoryPath(const_StringView)-Verify",
                   true,Filesystem::DirectoryExists(FullPathTestDir));
        TEST_EQUAL("RemoveDirectory(const_StringView)-PathDepth3",
                   Filesystem::ModifyResult::Success,
                   Filesystem::RemoveDirectory(FullPathTestDir));
        TEST_EQUAL("RemoveDirectory(const_StringView)-PathDepth2",
                   Filesystem::ModifyResult::Success,
                   Filesystem::RemoveDirectory("./DirTestingDir/Depth1/Depth2/"));
        TEST_EQUAL("RemoveDirectory(const_StringView)-PathDepth1",
                   Filesystem::ModifyResult::Success,
                   Filesystem::RemoveDirectory("./DirTestingDir/Depth1/"));//*/

        TEST_EQUAL("RemoveDirectory(const_StringView)-BaseDir",
                   Filesystem::ModifyResult::Success,
                   Filesystem::RemoveDirectory(BasePathTestDir));
    }// Basic Directory Management
}

#endif
