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

#include "PathUtilities.h"

namespace
{
    using namespace Mezzanine;

    /// @brief Gets how many directories deep a path is.
    /// @param Directory The directory to check the depth of.
    /// @param DirSep The directory separator(s) for splitting and rebuilding the path.
    /// @param ExitIfNegative If true, the function to return immediately if the depth count becomes negative.
    /// @return Returns an Integer representing how many directories down (or up, if negative) the path goes.
    Integer GetDirectoryDepth_Common(const StringView Directory, const StringView DirSep, const Boole ExitIfNegative)
    {
        Integer Depth = 0;
        String TempDir;

        StringView::const_iterator StrIt = Directory.begin();
        while( StrIt != Directory.end() )
        {
            if( DirSep.find_first_of(*StrIt) != StringView::npos ) {
                if( TempDir == ".." ) {
                    Depth -= 1;
                }else if( !TempDir.empty() && TempDir != "." ) {
                    Depth += 1;
                }
                TempDir.clear();

                if( ExitIfNegative && Depth < 0 ) {
                    break;
                }
            }else{
                TempDir.append(1,*StrIt);
            }
            ++StrIt;
        }
        return Depth;
    }

    /// @brief Builds a String path based on a number of directory/file names in a String vector.
    /// @note This function relies on the PathRoot argument to generate absolute paths.
    /// @param PathRoot This String will be prepended to the result of the ToBuild parameter.
    /// @param ToBuild A vector of Strings containing the overall path to be built.
    /// @param FileName The file portion of the path to be built.
    /// @param Separator The directory separator for splitting and rebuilding the path.
    /// @return Returns a String containing the rebuilt path.
    String BuildPath_Common(const StringView PathRoot, const StringVector& ToBuild,
                            const StringView FileName, const Char8 Separator)
    {// Char count gets used to reserve space on the return string
        size_t CharCount = PathRoot.size() + ToBuild.size() + FileName.size();
        StringVector::const_iterator BuildIt = ToBuild.begin();
        std::for_each(BuildIt,ToBuild.cend(),[&CharCount](const String& CurrStr) {
            CharCount += CurrStr.length();
        });
        String Ret;
        Ret.reserve(CharCount);
        Ret.append(PathRoot);
        // Build the path
        while( BuildIt != ToBuild.end() )
        {
            Ret.append( *BuildIt );
            Ret.append( 1, Separator );
            ++BuildIt;
        }
        Ret.append(FileName);
        return Ret;
    }

    /// @brief Removes all needless instances of "." or ".." and makes appropriate edits to the provided path.
    /// @param ToRemove The path to remove dot segments from.
    /// @param DirSep The directory separator(s) for splitting and rebuilding the path.
    /// @param IsAbsolute Whether or not the provided path is absolute.
    /// @return Returns a vector that is the tokenized path with extraneous parts of the path removed.
    StringVector RemoveDotSegments_Common(const StringView ToRemove, const StringView DirSep, const Boole IsAbsolute)
    {
        StringVector RetSegments;
        StringVector SplitPath = StringTools::Split(ToRemove,DirSep);
        for( StringVector::iterator VecIt = SplitPath.begin() ; VecIt != SplitPath.end() ; ++VecIt )
        {
            if( (*VecIt) == ".." ) {
                if( !RetSegments.empty() ) {
                    RetSegments.pop_back();
                }else if( !IsAbsolute ) {
                    RetSegments.push_back(*VecIt);
                }
            }else if( (*VecIt) != "." ) {
                RetSegments.push_back(*VecIt);
            }
        }
        return RetSegments;
    }

    /// @brief Convenience method to verify the necessary separator is present when concatenating.
    /// @param FilePath The directory path to the file.
    /// @param FileName The name of the file.
    /// @param Separator The directory separator for splitting and rebuilding the path.
    /// @return Returns a full String that is the concatenated path and filename.
    String CombinePathAndFileName_Common(const StringView FilePath, const StringView FileName, const Char8 Separator)
    {
        if( FilePath.empty() ) {
            return String(FileName);
        }

        String FullPath(FilePath.data(),FilePath.size());
        if( FilePath.back() != Separator ) {
            FullPath.append(1,Separator);
        }
        FullPath.append(FileName);
        return FullPath;
    }
}

namespace Mezzanine {
namespace Filesystem {
    ///////////////////////////////////////////////////////////////////////////////
    // Path Utilities

    String GetDirName(const char* FileName)
    {
        size_t SlashPos = String::npos, CurrPos = 0;
        while( FileName[CurrPos] != '\0' )
        {
            if( FileName[CurrPos] == '\\' || FileName[CurrPos] == '/' ) {
                SlashPos = CurrPos;
            }
            ++CurrPos;
        }
        if( SlashPos != String::npos ) {
            return String(FileName,SlashPos + 1);
        }
        return String();
    }

    String GetDirName(const StringView FileName)
    {
        size_t SlashPos = FileName.find_last_of("\\/");
        if( FileName.npos == SlashPos ) {
            return String();
        }else{
            return String( FileName.substr(0,SlashPos + 1) );
        }
    }

    String GetBaseName(const char* FileName)
    {
        size_t SlashPos = String::npos, CurrPos = 0;
        while( FileName[CurrPos] != '\0' )
        {
            if( FileName[CurrPos] == '\\' || FileName[CurrPos] == '/' ) {
                SlashPos = CurrPos;
            }
            ++CurrPos;
        }
        if( SlashPos != String::npos ) {
            return String(FileName + (SlashPos + 1),(CurrPos - SlashPos) - 1);
        }
        return String(FileName,CurrPos);
    }

    String GetBaseName(const StringView FileName)
    {
        size_t SlashPos = FileName.find_last_of("\\/");
        if( FileName.npos == SlashPos ) {
            return String( FileName );
        }else{
            return String( FileName.substr(SlashPos + 1,FileName.npos) );
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Absolute and Relative Paths

    Boole IsPathAbsolute(const StringView ToCheck)
    {
        return IsPathAbsolute_Windows(ToCheck) || IsPathAbsolute_Posix(ToCheck);
    }

    Boole IsPathAbsolute_Host(const StringView ToCheck)
    {
    #ifdef MEZZ_Windows
        return IsPathAbsolute_Windows(ToCheck);
    #else
        return IsPathAbsolute_Posix(ToCheck);
    #endif
    }

    Boole IsPathAbsolute_Posix(const StringView ToCheck)
    {
        if( !ToCheck.empty() ) {
            return ( ToCheck[0] == '/' );
        }
        return false;
    }

    Boole IsPathAbsolute_Windows(const StringView ToCheck)
    {
        if( ToCheck.size() >= 3 ) {
            return ( StringTools::IsAlphaLetter(ToCheck[0]) &&
                     ToCheck[1] == ':' &&
                     ( ToCheck[2] == '\\' || ToCheck[2] == '/' ) );
        }
        return false;
    }

    Boole IsPathRelative(const StringView ToCheck)
    {
        return !IsPathAbsolute(ToCheck);
    }

    Boole IsPathRelative_Host(const StringView ToCheck)
    {
    #ifdef MEZZ_Windows
        return IsPathRelative_Windows(ToCheck);
    #else
        return IsPathRelative_Posix(ToCheck);
    #endif
    }

    Boole IsPathRelative_Posix(const StringView ToCheck)
    {
        if( !ToCheck.empty() ) {
            return ( ToCheck[0] != '/' );
        }
        return false;
    }

    Boole IsPathRelative_Windows(const StringView ToCheck)
    {
        return !IsPathAbsolute(ToCheck);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Path Checks

    Integer GetDirectoryDepth_Host(const StringView ToCheck, const Boole ExitIfNegative)
    {
    #ifdef MEZZ_Windows
        return GetDirectoryDepth_Windows(ToCheck,ExitIfNegative);
    #else
        return GetDirectoryDepth_Posix(ToCheck,ExitIfNegative);
    #endif
    }

    Integer GetDirectoryDepth_Posix(const StringView ToCheck, const Boole ExitIfNegative)
    {
        String Separators = { GetDirectorySeparator_Posix() };
        return GetDirectoryDepth_Common(ToCheck,Separators,ExitIfNegative);
    }

    Integer GetDirectoryDepth_Windows(const StringView ToCheck, const Boole ExitIfNegative)
    {
        String Separators = { GetDirectorySeparator_Posix(), GetDirectorySeparator_Windows() };
        if( IsPathAbsolute_Windows(ToCheck) ) {
            return GetDirectoryDepth_Common(ToCheck.substr(2),Separators,ExitIfNegative);
        }else{
            return GetDirectoryDepth_Common(ToCheck,Separators,ExitIfNegative);
        }
    }

    Boole IsSubPath_Host(const StringView BasePath, const StringView CheckPath)
    {
    #ifdef MEZZ_Windows
        return IsSubPath_Windows(BasePath,CheckPath);
    #else
        return IsSubPath_Posix(BasePath,CheckPath);
    #endif
    }

    Boole IsSubPath_Posix(const StringView BasePath, const StringView CheckPath)
    {
        Boole BaseIsPosixAbsolute = IsPathAbsolute_Posix(BasePath);
        Boole CheckIsPosixAbsolute = IsPathAbsolute_Posix(CheckPath);

        if( BaseIsPosixAbsolute && !CheckIsPosixAbsolute ) {
            throw std::runtime_error("Attempting to compare absolute base path with relative sub-path.");
        }
        if( !BaseIsPosixAbsolute && CheckIsPosixAbsolute ) {
            throw std::runtime_error("Attempting to compare relative base path with absolute sub-path.");
        }

        String NormBasePath = RemoveDotSegments_Posix(BasePath);
        String NormCheckPath = RemoveDotSegments_Posix(CheckPath);
        String::const_iterator BaseIt = NormBasePath.begin();
        String::const_iterator BaseEnd = NormBasePath.end();
        String::const_iterator CheckIt = NormCheckPath.begin();
        String::const_iterator CheckEnd = NormCheckPath.end();

        while( BaseIt != BaseEnd )
        {
            if( CheckIt == CheckEnd || (*BaseIt) != (*CheckIt) ) {
                return false;
            }

            ++BaseIt;
            ++CheckIt;
        }

        String CheckRemains(CheckIt,CheckEnd);
        return ( GetDirectoryDepth_Posix(CheckRemains,true) > 0 );
    }

    Boole IsSubPath_Windows(const StringView BasePath, const StringView CheckPath)
    {
        Boole BaseIsWindowsAbsolute = IsPathAbsolute_Windows(BasePath);
        Boole CheckIsWindowsAbsolute = IsPathAbsolute_Windows(CheckPath);

        if( BaseIsWindowsAbsolute && !CheckIsWindowsAbsolute ) {
            throw std::runtime_error("Attempting to compare absolute base path with relative sub-path.");
        }
        if( !BaseIsWindowsAbsolute && CheckIsWindowsAbsolute ) {
            throw std::runtime_error("Attempting to compare relative base path with absolute sub-path.");
        }

        String NormBasePath = RemoveDotSegments_Windows(BasePath);
        String NormCheckPath = RemoveDotSegments_Windows(CheckPath);
        String::const_iterator BaseIt = NormBasePath.begin();
        String::const_iterator BaseEnd = NormBasePath.end();
        String::const_iterator CheckIt = NormCheckPath.begin();
        String::const_iterator CheckEnd = NormCheckPath.end();

        if( IsPathAbsolute_Windows(NormBasePath) ) {
            std::advance(BaseIt,2);
        }
        if( IsPathAbsolute_Windows(NormCheckPath) ) {
            std::advance(CheckIt,2);
        }

        while( BaseIt != BaseEnd )
        {
            if( CheckIt == CheckEnd || (*BaseIt) != (*CheckIt) ) {
                return false;
            }

            ++BaseIt;
            ++CheckIt;
        }

        String CheckRemains(CheckIt,CheckEnd);
        return ( GetDirectoryDepth_Windows(CheckRemains,true) > 0 );
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Path Utilities

    String BuildPath_Host(const StringView PathRoot, const StringVector& ToBuild, const StringView FileName)
    {
    #ifdef MEZZ_Windows
        return BuildPath_Windows(PathRoot,ToBuild,FileName);
    #else
        return BuildPath_Posix(PathRoot,ToBuild,FileName);
    #endif
    }

    String BuildPath_Posix(const StringView PathRoot, const StringVector& ToBuild, const StringView FileName)
    {
        return BuildPath_Common(PathRoot,ToBuild,FileName,GetDirectorySeparator_Posix());
    }

    String BuildPath_Windows(const StringView PathRoot, const StringVector& ToBuild, const StringView FileName)
    {
        return BuildPath_Common(PathRoot,ToBuild,FileName,GetDirectorySeparator_Windows());
    }

    String RemoveDotSegments_Host(const StringView ToRemove)
    {
    #ifdef MEZZ_Windows
        return RemoveDotSegments_Windows(ToRemove);
    #else
        return RemoveDotSegments_Posix(ToRemove);
    #endif
    }

    String RemoveDotSegments_Posix(const StringView ToRemove)
    {
        Boole AbsolutePath = IsPathAbsolute_Posix(ToRemove);
        String Separators = { GetDirectorySeparator_Posix() };
        size_t SplitStart = AbsolutePath ? ToRemove.find_first_of(Separators) + 1 : 0;
        size_t SplitEnd = ToRemove.find_last_of(Separators);
        size_t FileStart = ( SplitEnd != StringView::npos ? SplitEnd + 1 : SplitEnd );

        StringVector SplitPath = RemoveDotSegments_Common(ToRemove.substr(SplitStart,SplitEnd),"/",AbsolutePath);
        const StringView PathRoot = ToRemove.substr(0,SplitStart);
        const StringView FileName = ( FileStart != StringView::npos ? ToRemove.substr(FileStart) : String() );
        return BuildPath_Posix(PathRoot,SplitPath,FileName);
    }

    String RemoveDotSegments_Windows(const StringView ToRemove)
    {
        Boole AbsPath = IsPathAbsolute_Windows(ToRemove);
        String Separators = { GetDirectorySeparator_Posix(), GetDirectorySeparator_Windows() };
        size_t SplitStart = AbsPath ? ToRemove.find_first_of(Separators) + 1 : 0;
        size_t SplitEnd = ToRemove.find_last_of(Separators);
        size_t FileStart = ( SplitEnd != StringView::npos ? SplitEnd + 1 : SplitEnd );

        StringVector SplitPath = RemoveDotSegments_Common(ToRemove.substr(SplitStart,SplitEnd),Separators,AbsPath);
        const StringView PathRoot = ToRemove.substr(0,SplitStart);
        const StringView FileName = ( FileStart != StringView::npos ? ToRemove.substr(FileStart) : String() );
        return BuildPath_Windows(PathRoot,SplitPath,FileName);
    }

    String CombinePathAndFileName_Host(const StringView FilePath, const StringView FileName)
    {
    #ifdef MEZZ_Windows
        return CombinePathAndFileName_Windows(FilePath,FileName);
    #else
        return CombinePathAndFileName_Posix(FilePath,FileName);
    #endif
    }

    String CombinePathAndFileName_Posix(const StringView FilePath, const StringView FileName)
    {
        return CombinePathAndFileName_Common(FilePath,FileName,GetDirectorySeparator_Posix());
    }

    String CombinePathAndFileName_Windows(const StringView FilePath, const StringView FileName)
    {
        return CombinePathAndFileName_Common(FilePath,FileName,GetDirectorySeparator_Windows());
    }
}//Filesystem
}//Mezzanine
