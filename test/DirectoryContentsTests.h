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

    String FileData1 = "This is a simple test file.";
    String FileData2 = "This is a larger test file text for testing.";
    String FileData3 = "How much wood would a woodchuck chuck if a woodchuck could chuck wood?";
    String FileData4 = "When in the Course of human events, it becomes necessary for one people to dissolve "
                       "the political bands which have connected them with another, and to assume among the "
                       "powers of the earth, the separate and equal station to which the Laws of Nature and "
                       "of Nature's God entitle them, a decent respect to the opinions of mankind requires "
                       "that they should declare the causes which impel them to the separation.";

    if( Filesystem::CreateDirectory("Content/") == false ) {
        TEST_RESULT("CreateContentDir",Testing::TestResult::Failed);
        return;
    }

    {// GetDirectoryContentNames
        std::ofstream NameTestFile1;
        NameTestFile1.open("./Content/NameTestFile1.txt");
        NameTestFile1 << FileData1;
        NameTestFile1.close();
        std::ofstream NameTestFile2;
        NameTestFile2.open("./Content/NameTestFile2.txt");
        NameTestFile2 << FileData2;
        NameTestFile2.close();
        std::ofstream NameTestFile3;
        NameTestFile3.open("./Content/NameTestFile3.txt");
        NameTestFile3 << FileData3;
        NameTestFile3.close();
        std::ofstream NameTestFile4;
        NameTestFile4.open("./Content/NameTestFile4.txt");
        NameTestFile4 << FileData4;
        NameTestFile4.close();

        if( Filesystem::CreateDirectory("Content/NameTestDir/") == false ) {
            TEST_RESULT("CreateNameTestDir",Testing::TestResult::Failed);
            return;
        }

        StringVector ContentNames = Filesystem::GetDirectoryContentNames("Content/");
        TEST_EQUAL("GetDirectoryContentNames-Count",
                   size_t(5),ContentNames.size());
        TEST_EQUAL("GetDirectoryContentNames-DirFound",
                   true,std::find(ContentNames.begin(),ContentNames.end(),"NameTestDir") != ContentNames.end());
        TEST_EQUAL("GetDirectoryContentNames-FirstFound",
                   true,std::find(ContentNames.begin(),ContentNames.end(),"NameTestFile1.txt") != ContentNames.end());
        TEST_EQUAL("GetDirectoryContentNames-SecondFound",
                   true,std::find(ContentNames.begin(),ContentNames.end(),"NameTestFile2.txt") != ContentNames.end());
        TEST_EQUAL("GetDirectoryContentNames-ThirdFound",
                   true,std::find(ContentNames.begin(),ContentNames.end(),"NameTestFile3.txt") != ContentNames.end());
        TEST_EQUAL("GetDirectoryContentNames-FourthFound",
                   true,std::find(ContentNames.begin(),ContentNames.end(),"NameTestFile4.txt") != ContentNames.end());

        if( Filesystem::RemoveFile("./Content/NameTestFile1.txt") == false ) {
            TEST_RESULT("NameTestFile1-CleanupFailed",Testing::TestResult::Warning);
        }
        if( Filesystem::RemoveFile("./Content/NameTestFile2.txt") == false ) {
            TEST_RESULT("NameTestFile2-CleanupFailed",Testing::TestResult::Warning);
        }
        if( Filesystem::RemoveFile("./Content/NameTestFile3.txt") == false ) {
            TEST_RESULT("NameTestFile3-CleanupFailed",Testing::TestResult::Warning);
        }
        if( Filesystem::RemoveFile("./Content/NameTestFile4.txt") == false ) {
            TEST_RESULT("NameTestFile4-CleanupFailed",Testing::TestResult::Warning);
        }
        if( Filesystem::RemoveDirectory("Content/NameTestDir/") == false ) {
            TEST_RESULT("NameTestDir-CleanupFailed",Testing::TestResult::Warning);
        }
    }// GetDirectoryContentNames

    {// GetDirectoryContents
        using ArchiveEntryIterator = ArchiveEntryVector::iterator;

        std::ofstream ContentTestFile2;
        ContentTestFile2.open("./Content/ContentTestFile2.txt");
        ContentTestFile2 << FileData2;
        ContentTestFile2.close();
        std::ofstream ContentTestFile3;
        ContentTestFile3.open("./Content/ContentTestFile3.txt");
        ContentTestFile3 << FileData3;
        ContentTestFile3.close();
        std::ofstream ContentTestFile4;
        ContentTestFile4.open("./Content/ContentTestFile4.txt");
        ContentTestFile4 << FileData4;
        ContentTestFile4.close();

        if( Filesystem::CreateDirectory("Content/TestDir/") == false ) {
            TEST_RESULT("CreateContentTestDir",Testing::TestResult::Failed);
            return;
        }

        ArchiveEntryVector ContentEntries = Filesystem::GetDirectoryContents("Content/");
        TEST_EQUAL("GetDirectoryContents-Count",
                   size_t(4),ContentEntries.size());
        ArchiveEntryIterator EntryIt = ContentEntries.end();

        EntryIt = std::find_if(ContentEntries.begin(),ContentEntries.end(),[](const ArchiveEntry& Entry){
            return ( Entry.Name == "TestDir" );
        });
        TEST_EQUAL("GetDirectoryContents-DirFound",true,EntryIt != ContentEntries.end());
        if( EntryIt != ContentEntries.end() ) {
            TEST_EQUAL("GetDirectoryContents-DirArchiveType",
                       static_cast<int>(ArchiveType::FileSystem),static_cast<int>((*EntryIt).Archive));
            TEST_EQUAL("GetDirectoryContents-DirEntryType",
                       static_cast<int>(EntryType::Directory),static_cast<int>((*EntryIt).Entry));
            TEST_EQUAL("GetDirectoryContents-DirName",
                       String("TestDir"),(*EntryIt).Name);
            TEST_EQUAL("GetDirectoryContents-DirSize",
                       size_t(0),(*EntryIt).Size);
        }else{
            TEST_RESULT("GetDirectoryContents-DirArchiveType",Testing::TestResult::Failed);
            TEST_RESULT("GetDirectoryContents-DirEntryType",Testing::TestResult::Failed);
            TEST_RESULT("GetDirectoryContents-DirName",Testing::TestResult::Failed);
            TEST_RESULT("GetDirectoryContents-DirSize",Testing::TestResult::Failed);
        }

        EntryIt = std::find_if(ContentEntries.begin(),ContentEntries.end(),[](const ArchiveEntry& Entry){
            return ( Entry.Name == "ContentTestFile2.txt" );
        });
        TEST_EQUAL("GetDirectoryContents-SecondFound",true,EntryIt != ContentEntries.end());
        if( EntryIt != ContentEntries.end() ) {
            TEST_EQUAL("GetDirectoryContents-SecondArchiveType",
                       static_cast<int>(ArchiveType::FileSystem),static_cast<int>((*EntryIt).Archive));
            TEST_EQUAL("GetDirectoryContents-SecondEntryType",
                       static_cast<int>(EntryType::File),static_cast<int>((*EntryIt).Entry));
            TEST_EQUAL("GetDirectoryContents-SecondName",
                       String("ContentTestFile2.txt"),(*EntryIt).Name);
            TEST_EQUAL("GetDirectoryContents-SecondSize",
                       FileData2.size(),(*EntryIt).Size);
        }else{
            TEST_RESULT("GetDirectoryContents-SecondArchiveType",Testing::TestResult::Failed);
            TEST_RESULT("GetDirectoryContents-SecondEntryType",Testing::TestResult::Failed);
            TEST_RESULT("GetDirectoryContents-SecondName",Testing::TestResult::Failed);
            TEST_RESULT("GetDirectoryContents-SecondSize",Testing::TestResult::Failed);
        }

        EntryIt = std::find_if(ContentEntries.begin(),ContentEntries.end(),[](const ArchiveEntry& Entry){
            return ( Entry.Name == "ContentTestFile3.txt" );
        });
        TEST_EQUAL("GetDirectoryContents-ThirdFound",true,EntryIt != ContentEntries.end());
        if( EntryIt != ContentEntries.end() ) {
            TEST_EQUAL("GetDirectoryContents-ThirdArchiveType",
                       static_cast<int>(ArchiveType::FileSystem),static_cast<int>((*EntryIt).Archive));
            TEST_EQUAL("GetDirectoryContents-ThirdEntryType",
                       static_cast<int>(EntryType::File),static_cast<int>((*EntryIt).Entry));
            TEST_EQUAL("GetDirectoryContents-ThirdName",
                       String("ContentTestFile3.txt"),(*EntryIt).Name);
            TEST_EQUAL("GetDirectoryContents-ThirdSize",
                       FileData3.size(),(*EntryIt).Size);
        }else{
            TEST_RESULT("GetDirectoryContents-ThirdArchiveType",Testing::TestResult::Failed);
            TEST_RESULT("GetDirectoryContents-ThirdEntryType",Testing::TestResult::Failed);
            TEST_RESULT("GetDirectoryContents-ThirdName",Testing::TestResult::Failed);
            TEST_RESULT("GetDirectoryContents-ThirdSize",Testing::TestResult::Failed);
        }

        EntryIt = std::find_if(ContentEntries.begin(),ContentEntries.end(),[](const ArchiveEntry& Entry){
            return ( Entry.Name == "ContentTestFile4.txt" );
        });
        TEST_EQUAL("GetDirectoryContents-FourthFound",true,EntryIt != ContentEntries.end());
        if( EntryIt != ContentEntries.end() ) {
            TEST_EQUAL("GetDirectoryContents-FourthArchiveType",
                       static_cast<int>(ArchiveType::FileSystem),static_cast<int>((*EntryIt).Archive));
            TEST_EQUAL("GetDirectoryContents-FourthEntryType",
                       static_cast<int>(EntryType::File),static_cast<int>((*EntryIt).Entry));
            TEST_EQUAL("GetDirectoryContents-FourthName",
                       String("ContentTestFile4.txt"),(*EntryIt).Name);
            TEST_EQUAL("GetDirectoryContents-FourthSize",
                       FileData4.size(),(*EntryIt).Size);
        }else{
            TEST_RESULT("GetDirectoryContents-FourthArchiveType",Testing::TestResult::Failed);
            TEST_RESULT("GetDirectoryContents-FourthEntryType",Testing::TestResult::Failed);
            TEST_RESULT("GetDirectoryContents-FourthName",Testing::TestResult::Failed);
            TEST_RESULT("GetDirectoryContents-FourthSize",Testing::TestResult::Failed);
        }

        if( Filesystem::RemoveFile("./Content/ContentTestFile2.txt") == false ) {
            TEST_RESULT("ContentTestFile1-CleanupFailed",Testing::TestResult::Warning);
        }
        if( Filesystem::RemoveFile("./Content/ContentTestFile3.txt") == false ) {
            TEST_RESULT("ContentTestFile2-CleanupFailed",Testing::TestResult::Warning);
        }
        if( Filesystem::RemoveFile("./Content/ContentTestFile4.txt") == false ) {
            TEST_RESULT("ContentTestFile3-CleanupFailed",Testing::TestResult::Warning);
        }
        if( Filesystem::RemoveDirectory("Content/TestDir/") == false ) {
            TEST_RESULT("ContentTestDir-CleanupFailed",Testing::TestResult::Warning);
        }
    }// GetDirectoryContents
    if( Filesystem::RemoveDirectory("Content/") == false ) {
        TEST_RESULT("ContentDir-CleanupFailed",Testing::TestResult::Warning);
    }
}

#endif
