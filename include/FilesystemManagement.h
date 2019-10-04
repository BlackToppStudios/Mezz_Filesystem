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
#ifndef Mezz_Filesystem_FilesystemManagement_h
#define Mezz_Filesystem_FilesystemManagement_h

#ifndef SWIG
    #include "DataTypes.h"
#endif

namespace Mezzanine {
namespace Filesystem {
    /// @brief A Collection of possible results from common filesystem operations.
    /// @remarks Any value larger than success should be considered an error/failure. Any function that returns
    /// such a value did not complete and in most cases did not modify the filesystem.
    enum class [[nodiscard]] ModifyResult
    {
        Success = 0,       ///< Everything worked.
        // Existence Errors
        AlreadyExists,     ///< The file or directory to be created already exists.
        DoesNotExist,      ///< The file of directory specified wasn't found.
        // Path Errors
        InvalidPath,       ///< The path provided is invalid. Could be caused by a "." at the end.
        LoopingPath,       ///< The path provided contains links that form a loop.
        NameTooLong,       ///< The name of a file or directory specified is too long.
        // Permission Errors
        PermissionDenied,  ///< The filesystem permissions forbid the action to be taken.
        ReadOnly,          ///< The filesystem only permits read operations.
        // Directory Errors
        NotADirectory,     ///< The path was expected to direct to a directory, but doesn't.
        IsADirectory,      ///< The path was expected to direct to non-directory link, but does.
        NotEmpty,          ///< The directory specified is not empty.
        // Other Errors
        IOError,           ///< There was a hardware I/O error while completing the operation.
        NoSpace,           ///< There is insufficient space on the filesystem to complete the operation.
        MaxLinksExceeded,  ///< The operation would exceeded the maximum links supported for the directory.
        PrivilegeNotHeld,  ///< (Mostly Windows) A privilege needed for the operation is not held by the executing user.
        CurrentlyBusy,     ///< The file or directory specified is in use by the system and operation can't complete.
        OperationCanceled, ///< The operation was canceled or aborted.
        NotSupported,      ///< The operation is not supported on the host system.
        // Unknown
        Unknown            ///< Error is unknown.
    };//ModifyResult

    ///////////////////////////////////////////////////////////////////////////////
    // ModifyResult Operators

    /// @brief ModifyResult Equality with Boole operator.
    /// @param Left The ModifyResult to compare.
    /// @param Right The bool to compare.  True tests for a passing result, false tests for a failing result.
    /// @return If Right is testing for a passing result (is true) and Left is Success, returns true.
    /// If Right is testing for a failing result (is false) and Left is NOT Success, returns true.
    /// Otherwise, returns false.
    [[nodiscard]]
    Boole MEZZ_LIB operator==(const Filesystem::ModifyResult Left, const Boole Right) noexcept;
    /// @brief ModifyResult Equality with Boole operator.
    /// @param Left The bool to compare.  True tests for a passing result, false tests for a failing result.
    /// @param Right The ModifyResult to compare.
    /// @return If Left is testing for a passing result (is true) and Right is Success, returns true.
    /// If Left is testing for a failing result (is false) and Right is NOT Success, returns true.
    /// Otherwise, returns false.
    [[nodiscard]]
    Boole MEZZ_LIB operator==(const Boole Left, const Filesystem::ModifyResult Right) noexcept;
    /// @brief ModifyResult Inequality with Boole operator.
    /// @param Left The ModifyResult to compare.
    /// @param Right The bool to compare.  True tests for a failing result, false tests for a passing result.
    /// @return If Right is testing for a passing result (is false) and Left is Success, returns true.
    /// If Right is testing for a failing result (is true) and Left is NOT Success, returns true.
    /// Otherwise, returns false.
    [[nodiscard]]
    Boole MEZZ_LIB operator!=(const Filesystem::ModifyResult Left, const Boole Right) noexcept;
    /// @brief ModifyResult Inequality with Boole operator.
    /// @param Left The bool to compare.  True tests for a failing result, false tests for a passing result.
    /// @param Right The ModifyResult to compare.
    /// @return If Left is testing for a passing result (is false) and Right is Success, returns true.
    /// If Left is testing for a failing result (is true) and Right is NOT Success, returns true.
    /// Otherwise, returns false.
    [[nodiscard]]
    Boole MEZZ_LIB operator!=(const Boole Left, const Filesystem::ModifyResult Right) noexcept;

    ///////////////////////////////////////////////////////////////////////////////
    // Basic File Management

    /// @brief Verifies the existence of a file.
    /// @remarks If the path provided refers to a symlink, the link will be followed and the existence
    /// of the file pointed to by the symlink will be tested.
    /// @param FilePath The path and name of the file to check for.
    /// @return Returns true if the file at the specified file exists, false otherwise.
    [[nodiscard]]
    Boole MEZZ_LIB FileExists(const StringView FilePath) noexcept;

    /// @brief Copies a file on disk to a new location.
    /// @note This function makes no attempt to copy file permissions or attributes, only data.
    /// @param OldFilePath The existing path to the file (including the filename) to be copied.
    /// @param NewFilePath The path (including the filename) to where the file should be copied.
    /// @param FailIfExists If true the operation will fail if a file with the target name already exists.
    /// @return Returns a ModifyResult value describing the result of the file copy.
    [[nodiscard]]
    ModifyResult MEZZ_LIB CopyFile(const StringView OldFilePath, const StringView NewFilePath,
                                   const Boole FailIfExists);
    /// @brief Moves a file on disk from one location to another.
    /// @remarks This function can be used to rename files.
    /// @param OldFilePath The existing path to the file (including the filename) to be moved.
    /// @param NewFilePath The path (including the filename) to where the file should be named.
    /// @param FailIfExists If true the operation will fail if a file with the target name already exists.
    /// @return Returns a ModifyResult value describing the result of the file move.
    [[nodiscard]]
    ModifyResult MEZZ_LIB MoveFile(const StringView OldFilePath, const StringView NewFilePath,
                                   const Boole FailIfExists);
    /// @brief Deletes a file existing on the filesystem.
    /// @param FilePath The existing path to the file (including the filename) to be deleted.
    /// @return Returns a ModifyResult value describing the result of the file removal(delete).
    [[nodiscard]]
    ModifyResult MEZZ_LIB RemoveFile(const StringView FilePath);

    ///////////////////////////////////////////////////////////////////////////////
    // Symlinks

    /// @brief Checks to see if the file at the specified path is a Symlink.
    /// @warning Symlinks on emscripten is entirely unsupported and no guarantees are made for their behavior.
    /// Additionally, they don't make much sense in a web browser.
    /// @param SymPath The path to the file to check.
    /// @return Returns true if a Symlink is located at the specified path, false otherwise.
    [[nodiscard]]
    Boole MEZZ_LIB SymlinkExists(const StringView SymPath) noexcept;
    /// @brief Creates a symbolic link to a file on disk.
    /// @warning Symlinks on emscripten is entirely unsupported and no guarantees are made for their behavior.
    /// Additionally, they don't make much sense in a web browser.
    /// @param SymPath A path (including the name of the symbolic link) to where the link should be placed.
    /// @param TargetPath A path to where the symbolic link will point to.
    /// @return Returns a ModifyResult value describing the result of the link creation.
    [[nodiscard]]
    ModifyResult MEZZ_LIB CreateSymlink(const StringView SymPath, const StringView TargetPath);
    /// @brief Creates a symbolic link to a directory on disk.
    /// @warning Symlinks on emscripten is entirely unsupported and no guarantees are made for their behavior.
    /// Additionally, they don't make much sense in a web browser.
    /// @param SymPath A path (including the name of the symbolic link) to where the link should be placed.
    /// @param TargetPath A path to where the symbolic link will point to.
    /// @return Returns a ModifyResult value describing the result of the link creation.
    [[nodiscard]]
    ModifyResult MEZZ_LIB CreateDirectorySymlink(const StringView SymPath, const StringView TargetPath);
    /// @brief Gets the target path of a Symlink.
    /// @warning Symlinks on emscripten is entirely unsupported and no guarantees are made for their behavior.
    /// Additionally, they don't make much sense in a web browser.
    /// @param SymPath The path to the Symlink to read the target of.
    /// @return Returns an Optional storing the path to the target if the file at SymPath is a Symlink, or
    /// an empty invalid Optional if the file wasn't found or wasn't a Symlink.
    [[nodiscard]]
    Optional<String> MEZZ_LIB GetSymlinkTargetPath(const StringView SymPath);
    /// @brief Removes a Symlink from the disk.
    /// @note Symlinks on Posix are always files, and thus can be removed with RemoveFile.  However, Windows in
    /// it's infinite wisdom has symlinks that can be files or directories, and thus lacks a single function for
    /// their removal.  That's where this function comes in handy and will ensure the correct version is called.
    /// @warning Symlinks on emscripten is entirely unsupported and no guarantees are made for their behavior.
    /// Additionally, they don't make much sense in a web browser.
    /// @param SymPath The path to the Symlink to remove.
    /// @return Returns a ModifyResult value describing the result of the removal.
    [[nodiscard]]
    ModifyResult MEZZ_LIB RemoveSymlink(const StringView SymPath);

    ///////////////////////////////////////////////////////////////////////////////
    // Basic Directory Management

    /// @brief Verifies the existence of a folder.
    /// @param DirectoryPath The path and name of the folder to check for.
    /// @return Returns true if the folder at the specified directory exists, false otherwise.
    [[nodiscard]]
    Boole MEZZ_LIB DirectoryExists(const StringView DirectoryPath) noexcept;

    /// @brief Creates a single new directory.
    /// @remarks This function will only create the directory specified at the end of the path.
    /// @param DirectoryPath The path for the newly created directory.
    /// @return Returns a ModifyResult value describing the result of the directory creation.
    [[nodiscard]]
    ModifyResult MEZZ_LIB CreateDirectory(const StringView DirectoryPath);
    /// @brief Creates all directories that do not exist in the provided path.
    /// @param DirectoryPath The path for the newly created directory or directories.
    /// @return Returns a ModifyResult value describing the result of the multi-directory creation.
    [[nodiscard]]
    ModifyResult MEZZ_LIB CreateDirectoryPath(const StringView DirectoryPath);
    /// @brief Remove an empty directory.
    /// @param DirectoryPath The Path to the directory to remove.
    /// @return Returns a ModifyResult value describing the result of the directory removal(delete).
    [[nodiscard]]
    ModifyResult MEZZ_LIB RemoveDirectory(const StringView DirectoryPath);
}//Filesystem
}//Mezzanine

#endif
