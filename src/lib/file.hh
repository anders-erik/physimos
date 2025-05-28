#pragma once

#ifdef  PH_WINDOWS
    #include <windows.h>
#elif   PH_LINUX
    #include <fcntl.h>     // open
    #include <unistd.h>    // read, close
    #include <iostream>    // std::cerr
    #include <cstring>     // strerror
    #include <errno.h>     // errno
#endif

#include <iostream>

#include <string>

#include "process.hh"
#include "log.hh"

#include <filesystem>


struct File {

    std::string physimos_core_path;
    std::string relative_path;

    bool use_core_path = false;

    std::string file_contents;
    bool successful_read = false;

    File(std::string relative_path) : relative_path {relative_path} {

        std::string physiomos_dir = physimos_root_dir_or_die();
        physimos_core_path = physiomos_dir + "/" + relative_path;

        // file_contents = cat_file_as_string();
        // std::cout << file_contents << std::endl;
        
    };

    std::string copy_as_string_core(){
        use_core_path = true;
        successful_read = false;
        return cat_file_as_string();
    }
    std::string copy_as_string_cwd(){
        successful_read = false;
        return cat_file_as_string();
    }
    std::string& ref_as_string_cwd(){
        successful_read = false;
        file_contents = cat_file_as_string();
        return file_contents;
    }

    static void read_file_xplat(std::string path_str) {

        std::string return_string;

        const char* file_path = path_str.c_str();

        char buffer[1024];

    #ifdef PH_WINDOWS

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

    #elif  PH_LINUX

        int fd = open(file_path, O_RDONLY);
        if (fd == -1) {
            std::cerr << "open failed: " << strerror(errno) << '\n';
            return 1;
        }

        ssize_t bytesRead;
        while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, bytesRead); // write to stdout
        }

        if (bytesRead == -1) {
            std::cerr << "read failed: " << strerror(errno) << '\n';
        }

        close(fd);

    #endif


    }

private:

    std::string cat_file_as_string(){

        std::string string;
        std::ifstream _ifstream;

        

        if(use_core_path)
            _ifstream.open(physimos_core_path);
        else
            _ifstream.open(relative_path);
        
        if(!_ifstream.is_open()){
            std::string error_msg = "File: cat_file_as_string. Path: " + physimos_core_path;
            plib::plog_error("CONFIG", "OPEN_FILE", error_msg);
            return string;
        }

        try
        {
            std::stringstream buffer;
            buffer << _ifstream.rdbuf();
            string = buffer.str();
        }
        catch(const std::exception& e)
        {
            std::string error_msg = "File: cat_file_as_string. Path: " + physimos_core_path;
            plib::plog_error("CONFIG", "READ_FILE", error_msg);
        }
        
        successful_read = true;
        return string;
    }
};

