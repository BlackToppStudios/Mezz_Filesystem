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
    #ifdef MEZZ_CompilerIsEmscripten
        const String UtilityTestFile("UtilityTestOriginal.txt");
        const String UtilityTestCopy("UtilityTestCopy.txt");
        const String MoveTargetDir("MoveTarget/");
        const String MovedTestCopy("MoveTarget/UtilityTestCopy.txt");
        const String MovedRenamedTestCopy("MoveTarget/RenamedCopy.txt");
    #else
        const String UtilityTestFile("./UtilityTestOriginal.txt");
        const String UtilityTestCopy("./UtilityTestCopy.txt");
        const String MoveTargetDir("./MoveTarget/");
        const String MovedTestCopy("./MoveTarget/UtilityTestCopy.txt");
        const String MovedRenamedTestCopy("./MoveTarget/RenamedCopy.txt");
    #endif
        {// File Detection Test using ifstream
            std::ifstream UtilityFileDetect;
            UtilityFileDetect.open(UtilityTestFile);
            if( UtilityFileDetect ) {
                // Uh oh, we didn't clean up from the previous run.  Probably.
                TEST_RESULT("FileManagement-VerifyPreviousRunCleanup",Testing::TestResult::Failed);
            }
            UtilityFileDetect.close();
        }// File Detection Test using ifstream
        {// Test file creation
            std::ofstream UtilityFile;
            UtilityFile.open(UtilityTestFile,std::ios_base::out | std::ios_base::trunc);
            UtilityFile << "I exist! Temporarily! For this test! Then poof!";
            UtilityFile.close();
        }// Test file creation

        TEST_EQUAL("FileExists(const_StringView)-PassCheck",
                   true,Filesystem::FileExists(UtilityTestFile));
        TEST_EQUAL("CopyFile(const_StringView,const_StringView)-Fresh",
                   Filesystem::ModifyResult::Success,
                   Filesystem::CopyFile(UtilityTestFile,UtilityTestCopy,Filesystem::FileOverwrite::Allow));
        TEST_EQUAL("CopyFile(const_StringView,const_StringView)-Exists",
                   true,Filesystem::FileExists(UtilityTestCopy));
        TEST_EQUAL("CopyFile(const_StringView,const_StringView)-DuplicateFail",
                   Filesystem::ModifyResult::AlreadyExists,
                   Filesystem::CopyFile(UtilityTestCopy,UtilityTestFile,Filesystem::FileOverwrite::Deny));
        TEST_EQUAL("MoveFile(const_StringView,const_StringView)-CreateDest",
                   Filesystem::ModifyResult::Success,
                   Filesystem::CreateDirectory(MoveTargetDir));
        TEST_EQUAL("MoveFile(const_StringView,const_StringView)-ActualMove",
                   Filesystem::ModifyResult::Success,
                   Filesystem::MoveFile(UtilityTestCopy,MovedTestCopy,Filesystem::FileOverwrite::Deny));
        TEST_EQUAL("MoveFile(const_StringView,const_StringView)-SourceDoesntExist",
                   false,Filesystem::FileExists(UtilityTestCopy));
        TEST_EQUAL("MoveFile(const_StringView,const_StringView)-DestExists",
                   true,Filesystem::FileExists(MovedTestCopy));
        TEST_EQUAL("MoveFile(const_StringView,const_StringView)-Rename",
                   Filesystem::ModifyResult::Success,
                   Filesystem::MoveFile(MovedTestCopy,MovedRenamedTestCopy,Filesystem::FileOverwrite::Deny));
        TEST_EQUAL("RemoveFile(const_StringView)-DeleteMoved",
                   Filesystem::ModifyResult::Success,
                   Filesystem::RemoveFile(MovedRenamedTestCopy));
        TEST_EQUAL("RemoveFile(const_StringView)-VerifyMoved",
                   false,Filesystem::FileExists(MovedRenamedTestCopy));
        TEST_EQUAL("RemoveFile(const_StringView)-DeleteOriginal",
                   Filesystem::ModifyResult::Success,
                   Filesystem::RemoveFile(UtilityTestFile));
        TEST_EQUAL("RemoveFile(const_StringView)-VerifyDeleted",
                   false,Filesystem::FileExists(UtilityTestFile));
        TEST_EQUAL("FileManagement-Cleanup",
                   Filesystem::ModifyResult::Success,
                   Filesystem::RemoveDirectory(MoveTargetDir));
    }// Basic File Management

    #ifdef MEZZ_CompilerIsEmscripten
    {// Symlinks
        // Symlinks don't make sense on emscripten. Attempts were made to make it work and
        // was far too buggy because weird normalizing and slash inserting behaviors.
        TEST_RESULT("SymlinkManagement-(NotSupported)",
                    Testing::TestResult::Skipped);
    }// Symlinks
    #else
    {// Symlinks - File
        const String TargetName("./LinkTargetFile.txt");
        const String LinkName("./TotallyNotALink.txt");

        std::ofstream LinkTargetFile;
        LinkTargetFile.open(TargetName,std::ios_base::out | std::ios_base::trunc);
        LinkTargetFile << "I'm a link target!";
        LinkTargetFile.close();

        Filesystem::ModifyResult CreateResult = Filesystem::CreateSymlink(LinkName,TargetName);
        if( CreateResult == Filesystem::ModifyResult::Success ) {
            TEST_EQUAL("CreateSymlink(const_StringView,const_StringView)-File",
                       Filesystem::ModifyResult::Success,
                       CreateResult);

            TEST_EQUAL("SymlinkExists(const_StringView)-File-Link",
                       true,
                       Filesystem::SymlinkExists(LinkName));
            TEST_EQUAL("SymlinkExists(const_StringView)-File-Target",
                       false,
                       Filesystem::SymlinkExists(TargetName));

            Optional<String> LinkPathPass = Filesystem::GetSymlinkTargetPath(LinkName);
            TEST_EQUAL("GetSymlinkTargetPath(const_StringView)-File-Link-Validity",
                       true,
                       LinkPathPass.has_value());
            // Without this if, we get an uncaught exception that prevents the viewing of other test results.
            if( LinkPathPass.has_value() ) {
                TEST_EQUAL("GetSymlinkTargetPath(const_StringView)-File-Link-Value",
                           TargetName,
                           LinkPathPass.value());
            }else{
                TEST_RESULT("GetSymlinkTargetPath(const_StringView)-File-Link-Value",
                            Testing::TestResult::Failed);
            }

            Optional<String> LinkPathFail = Filesystem::GetSymlinkTargetPath(TargetName)
            TEST_EQUAL("GetSymlinkTargetPath(const_StringView)-File-Target-Validity",
                       false,
                       LinkPathFail.has_value());

            TEST_EQUAL("RemoveSymlink(const_StringView)-File",
                       Filesystem::ModifyResult::Success,
                       Filesystem::RemoveSymlink(LinkName));
            TEST_EQUAL("SymlinkManagement-File-CleanupTarget",
                       Filesystem::ModifyResult::Success,
                       Filesystem::RemoveFile(TargetName));
        }else if( CreateResult == Filesystem::ModifyResult::PrivilegeNotHeld ) {
            this->TestLog << "User Privileges are insufficient for this operation.\n";
            TEST_RESULT("CreateSymlink(const_StringView,const_StringView)-File",
                        Testing::TestResult::Skipped);
        }else if( CreateResult == Filesystem::ModifyResult::NotSupported ) {
            this->TestLog << "Creation of Symlinks are not supported on the host system.\n";
            TEST_RESULT("CreateSymlink(const_StringView,const_StringView)-File",
                        Testing::TestResult::Skipped);
        }else{
            this->TestLog << "Unknown Symlink error.  :(\n";
            TEST_RESULT("CreateSymlink(const_StringView,const_StringView)-File",
                        Testing::TestResult::Failed);
        }
    }// Symlinks - File

    {// Symlinks - Directory
        const String TargetName("./LinkTargetDir");
        const String LinkName("./TotallyNotALink");

        Filesystem::ModifyResult MakeDirResult = Filesystem::CreateDirectory(TargetName);
        if( MakeDirResult != Filesystem::ModifyResult::Success ) {
            this->TestLog << "Failed to create test directory.";
            TEST_RESULT("SymlinkManagement-Directory-Setup",
                        Testing::TestResult::Failed);
        }else{
            Filesystem::ModifyResult CreateResult = Filesystem::CreateDirectorySymlink(LinkName,TargetName);
            if( CreateResult == Filesystem::ModifyResult::Success ) {
                TEST_EQUAL("CreateSymlink(const_StringView,const_StringView)-Directory",
                           Filesystem::ModifyResult::Success,
                           CreateResult);

                TEST_EQUAL("SymlinkExists(const_StringView)-Directory-Link",
                           true,
                           Filesystem::SymlinkExists(LinkName));
                TEST_EQUAL("SymlinkExists(const_StringView)-Directory-Target",
                           false,
                           Filesystem::SymlinkExists(TargetName));

                Optional<String> LinkPathPass = Filesystem::GetSymlinkTargetPath(LinkName);
                TEST_EQUAL("GetSymlinkTargetPath(const_StringView)-Directory-Link-Validity",
                           true,
                           LinkPathPass.has_value());
                // Without this if, we get an uncaught exception that prevents the viewing of other test results.
                if( LinkPathPass.has_value() ) {
                    TEST_EQUAL("GetSymlinkTargetPath(const_StringView)-Directory-Link-Value",
                               TargetName,
                               LinkPathPass.value());
                }else{
                    TEST_RESULT("GetSymlinkTargetPath(const_StringView)-Directory-Link-Value",
                                Testing::TestResult::Failed);
                }

                Optional<String> LinkPathFail = Filesystem::GetSymlinkTargetPath(TargetName)
                TEST_EQUAL("GetSymlinkTargetPath(const_StringView)-Directory-Target-Validity",
                           false,
                           LinkPathFail.has_value());

                TEST_EQUAL("RemoveSymlink(const_StringView)-Directory",
                           Filesystem::ModifyResult::Success,
                           Filesystem::RemoveSymlink(LinkName));
                TEST_EQUAL("SymlinkManagement-Directory-CleanupTarget",
                           Filesystem::ModifyResult::Success,
                           Filesystem::RemoveDirectory(TargetName));
            }else if( CreateResult == Filesystem::ModifyResult::PrivilegeNotHeld ) {
                this->TestLog << "User Privileges are insufficient for this operation.\n";
                TEST_RESULT("CreateDirectorySymlink(const_StringView,const_StringView)-Directory",
                            Testing::TestResult::Skipped);
            }else if( CreateResult == Filesystem::ModifyResult::NotSupported ) {
                this->TestLog << "Creation of Symlinks are not supported on the host system.\n";
                TEST_RESULT("CreateDirectorySymlink(const_StringView,const_StringView)-Directory",
                            Testing::TestResult::Skipped);
            }else{
                this->TestLog << "Unknown Symlink error.  :(\n";
                TEST_RESULT("CreateDirectorySymlink(const_StringView,const_StringView)-Directory",
                            Testing::TestResult::Failed);
            }
        }
    }// Symlinks - Directory
    #endif

    {// Basic Directory Management
    #ifdef MEZZ_CompilerIsEmscripten
        const String BasePathTestDir("DirTestingDir/");
        const String DepthOneDir("DirTestingDir/Depth1/");
        const String DepthTwoDir("DirTestingDir/Depth1/Depth2/");
        const String DepthThreeDir("DirTestingDir/Depth1/Depth2/Depth3/");
    #else
        const String BasePathTestDir("./DirTestingDir/");
        const String DepthOneDir("./DirTestingDir/Depth1/");
        const String DepthTwoDir("./DirTestingDir/Depth1/Depth2/");
        const String DepthThreeDir("./DirTestingDir/Depth1/Depth2/Depth3/");
    #endif

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

        TEST_EQUAL("CreateDirectoryPath(const_StringView)",
                   Filesystem::ModifyResult::Success,
                   Filesystem::CreateDirectoryPath(DepthThreeDir));
        TEST_EQUAL("CreateDirectoryPath(const_StringView)-Verify",
                   true,Filesystem::DirectoryExists(DepthThreeDir));
        TEST_EQUAL("RemoveDirectory(const_StringView)-PathDepth3",
                   Filesystem::ModifyResult::Success,
                   Filesystem::RemoveDirectory(DepthThreeDir));
        TEST_EQUAL("RemoveDirectory(const_StringView)-PathDepth2",
                   Filesystem::ModifyResult::Success,
                   Filesystem::RemoveDirectory(DepthTwoDir));
        TEST_EQUAL("RemoveDirectory(const_StringView)-PathDepth1",
                   Filesystem::ModifyResult::Success,
                   Filesystem::RemoveDirectory(DepthOneDir));

        TEST_EQUAL("RemoveDirectory(const_StringView)-BaseDir",
                   Filesystem::ModifyResult::Success,
                   Filesystem::RemoveDirectory(BasePathTestDir));
    }// Basic Directory Management

    {// ModifyResult Operators
        Filesystem::ModifyResult Good = Filesystem::ModifyResult::Success;
        Filesystem::ModifyResult BadOne = Filesystem::ModifyResult::DoesNotExist;
        Filesystem::ModifyResult BadTwo = Filesystem::ModifyResult::InvalidPath;

        TEST_EQUAL("operator==(const_ModifyResult,const_Boole)-True-Pass",
                   true,operator==(Good,true));
        TEST_EQUAL("operator==(const_ModifyResult,const_Boole)-True-Fail",
                   false,operator==(BadOne,true));
        TEST_EQUAL("operator==(const_ModifyResult,const_Boole)-False-Pass",
                   true,operator==(BadTwo,false));
        TEST_EQUAL("operator==(const_ModifyResult,const_Boole)-False-Fail",
                   false,operator==(Good,false));

        TEST_EQUAL("operator==(const_Boole,const_ModifyResult)-True-Pass",
                   true,operator==(true,Good));
        TEST_EQUAL("operator==(const_Boole,const_ModifyResult)-True-Fail",
                   false,operator==(true,BadTwo));
        TEST_EQUAL("operator==(const_Boole,const_ModifyResult)-False-Pass",
                   true,operator==(false,BadOne));
        TEST_EQUAL("operator==(const_Boole,const_ModifyResult)-False-Fail",
                   false,operator==(false,Good));

        TEST_EQUAL("operator!=(const_ModifyResult,const_Boole)-True-Pass",
                   true,operator!=(BadOne,true));
        TEST_EQUAL("operator!=(const_ModifyResult,const_Boole)-True-Fail",
                   false,operator!=(Good,true));
        TEST_EQUAL("operator!=(const_ModifyResult,const_Boole)-False-Pass",
                   true,operator!=(Good,false));
        TEST_EQUAL("operator!=(const_ModifyResult,const_Boole)-False-Fail",
                   false,operator!=(BadTwo,false));

        TEST_EQUAL("operator!=(const_Boole,const_ModifyResult)-True-Pass",
                   true,operator!=(true,BadTwo));
        TEST_EQUAL("operator!=(const_Boole,const_ModifyResult)-True-Fail",
                   false,operator!=(true,Good));
        TEST_EQUAL("operator!=(const_Boole,const_ModifyResult)-False-Pass",
                   true,operator!=(false,Good));
        TEST_EQUAL("operator!=(const_Boole,const_ModifyResult)-False-Fail",
                   false,operator!=(false,BadOne));
    }// ModifyResult Operators
}

#endif
