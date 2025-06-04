
#ifdef PH_WINDOWS

#include <windows.h>

#include "fs/windows.hh"


#include "str.hh"
#include "log.hh"
#include "err.hh"


namespace OS {


Str file_cat_as_str(const char* path){


    const char* file_path = path;

    char buffer[10024];

    HANDLE hFile = CreateFileA(
        file_path,
        GENERIC_READ,           // desired access
        FILE_SHARE_READ,        // share mode
        nullptr,                // security attributes
        OPEN_EXISTING,          // creation disposition
        FILE_ATTRIBUTE_NORMAL,  // flags and attributes
        nullptr                 // template file handle
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "CreateFile failed: " << GetLastError() << '\n';
        return 1;
    }

    DWORD bytesRead;

    while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, nullptr) && bytesRead > 0) {
        DWORD bytesWritten;
        WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), buffer, bytesRead, &bytesWritten, nullptr);
    }

    CloseHandle(hFile);


    return Str ( (const char*) buffer );
}


}


#endif