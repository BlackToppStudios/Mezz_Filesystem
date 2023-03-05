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
#ifndef Mezz_Filesystem_DirectoryContentsTests_h
#define Mezz_Filesystem_DirectoryContentsTests_h

/// @file
/// @brief A few tests of some utilities that retrieve the contents of directories.

#include "MezzTest.h"

#include "DirectoryContents.h"
#include "FilesystemManagement.h"

AUTOMATIC_TEST_GROUP(DirectoryContentsTests,DirectoryContents)
{
    using namespace Mezzanine;

    const StringView FileData1 = "This is a simple test file, with some length.";
    const StringView FileData2 = "This is a larger test file text for testing larger files.";
    const StringView FileData3 = "How much wood would a woodchuck chuck if a woodchuck could chuck wood?";
    const StringView FileData4 = "When in the course of human events, it becomes necessary for one people to dissolve "
                                 "the political bands which have connected them with another, and to assume among the "
                                 "powers of the earth, the separate and equal station to which the Laws of Nature and "
                                 "of Nature's God entitle them, a decent respect to the opinions of mankind requires "
                                 "that they should declare the causes which impel them to the separation.";

    {// GetDirectoryContentNames
        const String ContentNamesDir{"ContentNames/"};
        const String ContentNamesSubDir{"NameTestDir"};

        const String TestFile1Name{"NameTestFile1.txt"};
        const String TestFile2Name{"NameTestFile2.txt"};
        const String TestFile3Name{"NameTestFile3.txt"};
        const String TestFile4Name{"NameTestFile4.txt"};

        const String ContentNamesSubDirPath{ContentNamesDir + ContentNamesSubDir + "/"};

        const String TestFile1Path{ContentNamesDir + TestFile1Name};
        const String TestFile2Path{ContentNamesDir + TestFile2Name};
        const String TestFile3Path{ContentNamesDir + TestFile3Name};
        const String TestFile4Path{ContentNamesDir + TestFile4Name};

        auto CleanContentNames = [&]() {
            TestLog << "Removing file \"" << TestFile1Path << "\": ";
            TestLog << ( Filesystem::RemoveFile(TestFile1Path) == true ? "Succeeded" : "Failed" ) << "\n";

            TestLog << "Removing file \"" << TestFile2Path << "\": ";
            TestLog << ( Filesystem::RemoveFile(TestFile2Path) == true ? "Succeeded" : "Failed" ) << "\n";

            TestLog << "Removing file \"" << TestFile3Path << "\": ";
            TestLog << ( Filesystem::RemoveFile(TestFile3Path) == true ? "Succeeded" : "Failed" ) << "\n";

            TestLog << "Removing file \"" << TestFile4Path << "\": ";
            TestLog << ( Filesystem::RemoveFile(TestFile4Path) == true ? "Succeeded" : "Failed" ) << "\n";

            TestLog << "Removing directory \"" << ContentNamesSubDirPath << "\": ";
            TestLog << ( Filesystem::RemoveDirectory(ContentNamesSubDirPath) == true ? "Succeeded" : "Failed" ) << "\n";

            TestLog << "Removing directory \"" << ContentNamesDir << "\": ";
            TestLog << ( Filesystem::RemoveDirectory(ContentNamesDir) == true ? "Succeeded" : "Failed" ) << "\n";
        };//*/

        TestLog << "Pre-ContentNames cleanup starting.  These should fail.\n";
        CleanContentNames();
        TestLog << "Pre-ContentNames cleanup complete.\n";

        if( Filesystem::CreateDirectory(ContentNamesDir) == false ) {
            TEST_RESULT("CreateContentNamesDir",Testing::TestResult::Failed)
            return;
        }

        std::ofstream NameTestFile1;
        NameTestFile1.open(TestFile1Path);
        NameTestFile1 << FileData1;
        NameTestFile1.close();
        std::ofstream NameTestFile2;
        NameTestFile2.open(TestFile2Path);
        NameTestFile2 << FileData2;
        NameTestFile2.close();
        std::ofstream NameTestFile3;
        NameTestFile3.open(TestFile3Path);
        NameTestFile3 << FileData3;
        NameTestFile3.close();
        std::ofstream NameTestFile4;
        NameTestFile4.open(TestFile4Path);
        NameTestFile4 << FileData4;
        NameTestFile4.close();

        if( Filesystem::CreateDirectory(ContentNamesSubDirPath) == false ) {
            TEST_RESULT("CreateNameTestSubDir",Testing::TestResult::Failed)
            return;
        }

        StringVector ContentNames = Filesystem::GetDirectoryContentNames(ContentNamesDir);
        TEST_EQUAL("GetDirectoryContentNames-Count",
                   size_t(5),ContentNames.size())
        TEST_EQUAL("GetDirectoryContentNames-DirFound",
                   true,std::find(ContentNames.begin(),ContentNames.end(),ContentNamesSubDir) != ContentNames.end())
        TEST_EQUAL("GetDirectoryContentNames-FirstFound",
                   true,std::find(ContentNames.begin(),ContentNames.end(),TestFile1Name) != ContentNames.end())
        TEST_EQUAL("GetDirectoryContentNames-SecondFound",
                   true,std::find(ContentNames.begin(),ContentNames.end(),TestFile2Name) != ContentNames.end())
        TEST_EQUAL("GetDirectoryContentNames-ThirdFound",
                   true,std::find(ContentNames.begin(),ContentNames.end(),TestFile3Name) != ContentNames.end())
        TEST_EQUAL("GetDirectoryContentNames-FourthFound",
                   true,std::find(ContentNames.begin(),ContentNames.end(),TestFile4Name) != ContentNames.end())

        TestLog << "Post-ContentNames cleanup.  These should succeed.\n";
        CleanContentNames();
        TestLog << "Post-ContentNames cleanup complete.\n";
    }// GetDirectoryContentNames

    {// GetDirectoryContents
        using ArchiveEntryIterator = ArchiveEntryVector::iterator;

        const String DirectoryContentsDir{"DirContents/"};
        const String DirectoryContentsSubDir{"TestDir"};

        const String TestFile2Name{"ContentTestFile2.txt"};
        const String TestFile3Name{"ContentTestFile3.txt"};
        const String TestFile4Name{"ContentTestFile4.txt"};

        const String DirectoryContentsSubDirPath{DirectoryContentsDir + DirectoryContentsSubDir + "/"};

        const String TestFile2Path{DirectoryContentsDir + TestFile2Name};
        const String TestFile3Path{DirectoryContentsDir + TestFile3Name};
        const String TestFile4Path{DirectoryContentsDir + TestFile4Name};

        auto CleanDirectoryContents = [&]() {
            TestLog << "Removing file \"" << TestFile2Path << "\": ";
            TestLog << ( Filesystem::RemoveFile(TestFile2Path) == true ? "Succeeded" : "Failed" ) << "\n";

            TestLog << "Removing file \"" << TestFile3Path << "\": ";
            TestLog << ( Filesystem::RemoveFile(TestFile3Path) == true ? "Succeeded" : "Failed" ) << "\n";

            TestLog << "Removing file \"" << TestFile4Path << "\": ";
            TestLog << ( Filesystem::RemoveFile(TestFile4Path) == true ? "Succeeded" : "Failed" ) << "\n";

            TestLog << "Removing directory \"" << DirectoryContentsSubDirPath << "\": ";
            TestLog << ( Filesystem::RemoveDirectory(DirectoryContentsSubDirPath) == true ? "Succeeded" : "Failed" ) << "\n";

            TestLog << "Removing directory \"" << DirectoryContentsDir << "\": ";
            TestLog << ( Filesystem::RemoveDirectory(DirectoryContentsDir) == true ? "Succeeded" : "Failed" ) << "\n";
        };

        TestLog << "Pre-DirectoryContents cleanup.  These should fail.\n";
        CleanDirectoryContents();
        TestLog << "Pre-DirectoryContents cleanup complete.\n";

        if( Filesystem::CreateDirectory(DirectoryContentsDir) == false ) {
            TEST_RESULT("CreateDirContentsDir",Testing::TestResult::Failed)
            return;
        }

        std::ofstream ContentTestFile2;
        ContentTestFile2.open(TestFile2Path);
        ContentTestFile2 << FileData2;
        ContentTestFile2.close();
        std::ofstream ContentTestFile3;
        ContentTestFile3.open(TestFile3Path);
        ContentTestFile3 << FileData3;
        ContentTestFile3.close();
        std::ofstream ContentTestFile4;
        ContentTestFile4.open(TestFile4Path);
        ContentTestFile4 << FileData4;
        ContentTestFile4.close();

        if( Filesystem::CreateDirectory(DirectoryContentsSubDirPath) == false ) {
            TEST_RESULT("CreateDirContentsSubDir",Testing::TestResult::Failed)
            return;
        }

        ArchiveEntryVector ContentEntries = Filesystem::GetDirectoryContents(DirectoryContentsDir);
        TEST_EQUAL("GetDirectoryContents-Count",
                   size_t(4),ContentEntries.size())
        ArchiveEntryIterator EntryIt = ContentEntries.end();

        EntryIt = std::find_if(ContentEntries.begin(),ContentEntries.end(),[&](const ArchiveEntry& Entry){
            return ( Entry.Name == DirectoryContentsSubDir );
        });
        TEST_EQUAL("GetDirectoryContents-DirFound",true,EntryIt != ContentEntries.end())
        if( EntryIt != ContentEntries.end() ) {
            TEST_EQUAL("GetDirectoryContents-DirArchiveType",
                       static_cast<int>(ArchiveType::FileSystem),static_cast<int>((*EntryIt).Archive))
            TEST_EQUAL("GetDirectoryContents-DirEntryType",
                       static_cast<int>(EntryType::Directory),static_cast<int>((*EntryIt).Entry))
            TEST_EQUAL("GetDirectoryContents-DirName",
                       DirectoryContentsSubDir,(*EntryIt).Name)
            TEST_EQUAL("GetDirectoryContents-DirSize",
                       size_t(0),(*EntryIt).Size)
        }else{
            TEST_RESULT("GetDirectoryContents-DirArchiveType",Testing::TestResult::Failed)
            TEST_RESULT("GetDirectoryContents-DirEntryType",Testing::TestResult::Failed)
            TEST_RESULT("GetDirectoryContents-DirName",Testing::TestResult::Failed)
            TEST_RESULT("GetDirectoryContents-DirSize",Testing::TestResult::Failed)
        }

        EntryIt = std::find_if(ContentEntries.begin(),ContentEntries.end(),[&](const ArchiveEntry& Entry){
            return ( Entry.Name == TestFile2Name );
        });
        TEST_EQUAL("GetDirectoryContents-SecondFound",true,EntryIt != ContentEntries.end())
        if( EntryIt != ContentEntries.end() ) {
            TEST_EQUAL("GetDirectoryContents-SecondArchiveType",
                       static_cast<int>(ArchiveType::FileSystem),static_cast<int>((*EntryIt).Archive))
            TEST_EQUAL("GetDirectoryContents-SecondEntryType",
                       static_cast<int>(EntryType::File),static_cast<int>((*EntryIt).Entry))
            TEST_EQUAL("GetDirectoryContents-SecondName",
                       TestFile2Name,(*EntryIt).Name)
            TEST_EQUAL("GetDirectoryContents-SecondSize",
                       FileData2.size(),(*EntryIt).Size)
        }else{
            TEST_RESULT("GetDirectoryContents-SecondArchiveType",Testing::TestResult::Failed)
            TEST_RESULT("GetDirectoryContents-SecondEntryType",Testing::TestResult::Failed)
            TEST_RESULT("GetDirectoryContents-SecondName",Testing::TestResult::Failed)
            TEST_RESULT("GetDirectoryContents-SecondSize",Testing::TestResult::Failed)
        }

        EntryIt = std::find_if(ContentEntries.begin(),ContentEntries.end(),[&](const ArchiveEntry& Entry){
            return ( Entry.Name == TestFile3Name );
        });
        TEST_EQUAL("GetDirectoryContents-ThirdFound",true,EntryIt != ContentEntries.end())
        if( EntryIt != ContentEntries.end() ) {
            TEST_EQUAL("GetDirectoryContents-ThirdArchiveType",
                       static_cast<int>(ArchiveType::FileSystem),static_cast<int>((*EntryIt).Archive))
            TEST_EQUAL("GetDirectoryContents-ThirdEntryType",
                       static_cast<int>(EntryType::File),static_cast<int>((*EntryIt).Entry))
            TEST_EQUAL("GetDirectoryContents-ThirdName",
                       TestFile3Name,(*EntryIt).Name)
            TEST_EQUAL("GetDirectoryContents-ThirdSize",
                       FileData3.size(),(*EntryIt).Size)
        }else{
            TEST_RESULT("GetDirectoryContents-ThirdArchiveType",Testing::TestResult::Failed)
            TEST_RESULT("GetDirectoryContents-ThirdEntryType",Testing::TestResult::Failed)
            TEST_RESULT("GetDirectoryContents-ThirdName",Testing::TestResult::Failed)
            TEST_RESULT("GetDirectoryContents-ThirdSize",Testing::TestResult::Failed)
        }

        EntryIt = std::find_if(ContentEntries.begin(),ContentEntries.end(),[&](const ArchiveEntry& Entry){
            return ( Entry.Name == TestFile4Name );
        });
        TEST_EQUAL("GetDirectoryContents-FourthFound",true,EntryIt != ContentEntries.end())
        if( EntryIt != ContentEntries.end() ) {
            TEST_EQUAL("GetDirectoryContents-FourthArchiveType",
                       static_cast<int>(ArchiveType::FileSystem),static_cast<int>((*EntryIt).Archive))
            TEST_EQUAL("GetDirectoryContents-FourthEntryType",
                       static_cast<int>(EntryType::File),static_cast<int>((*EntryIt).Entry))
            TEST_EQUAL("GetDirectoryContents-FourthName",
                       TestFile4Name,(*EntryIt).Name)
            TEST_EQUAL("GetDirectoryContents-FourthSize",
                       FileData4.size(),(*EntryIt).Size)
        }else{
            TEST_RESULT("GetDirectoryContents-FourthArchiveType",Testing::TestResult::Failed)
            TEST_RESULT("GetDirectoryContents-FourthEntryType",Testing::TestResult::Failed)
            TEST_RESULT("GetDirectoryContents-FourthName",Testing::TestResult::Failed)
            TEST_RESULT("GetDirectoryContents-FourthSize",Testing::TestResult::Failed)
        }

        TestLog << "Post-DirectoryContents cleanup.  These should succeed.\n";
        CleanDirectoryContents();
        TestLog << "Post-DirectoryContents cleanup complete.\n";
    }// GetDirectoryContents
}

#endif
