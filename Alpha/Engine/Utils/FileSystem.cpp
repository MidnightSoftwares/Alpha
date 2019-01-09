#include "FileSystem.h"

#include <Shlwapi.h>

const std::wstring& FileSystemUtils::ExecutableDirectory()
{
    static const auto DIRECTORY = []() -> std::wstring
    {
        WCHAR directory[MAX_PATH];
        GetModuleFileName(GetModuleHandle(nullptr), directory, MAX_PATH);
        PathRemoveFileSpec(directory);

        return directory;
    }();

    return DIRECTORY;
}
