
#ifdef  PH_LINUX
    #include <fcntl.h>     // open
    #include <unistd.h>    // read, close, write
    #include <iostream>    // std::cerr
    #include <cstring>     // strerror
    #include <errno.h>     // errno
    #include <sys/stat.h>
#elif  PH_WINDOWS
    #include <windows.h>
#else
    #error lib/file.cc/hh is not supported on current OS.
#endif


#include <iostream>
#include <fstream>
#include <filesystem>

#include "log.hh"

#include "file.hh"





Str File::get_current_path(){
    if(path_type == path_t::core)
        return absolute_core_path;
    else
        return relative_path;
}
const char* File::get_current_path_c_str(){

    return get_current_path().to_c_str();
    
}

void File::update_absolute_path(){

    physimos_root_dir = physimos_root_dir_or_die_str();
    absolute_core_path = physimos_root_dir + "/" + relative_path;

}


void File::set_path(Str & path_str){
    relative_path = path_str;
}


void File::echo_str_core_xplat(Str str){

    path_type = path_t::core;
    update_absolute_path();

#ifdef PH_LINUX
    echo_file_linux(str);
#elif  PH_WINDOWS
    echo_file_window();
#endif


}

Str File::cat_as_str_core_xplat() {

    path_type = path_t::core;
    update_absolute_path();

#ifdef PH_LINUX
    return cat_file_linux();
#elif  PH_WINDOWS
    return cat_core_file_window();
#endif


}


Str File::cat_as_str_core_xplat(Str & path_str){

    File file (path_str);

    return file.cat_as_str_core_xplat();

}




#ifdef PH_LINUX



Str File::cat_file_linux()
{
    struct stat st;
    
    const char* file_path = get_current_path_c_str();

    char buffer[10024];


    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        std::cerr << "open failed: " << strerror(errno) << '\n';
        return Str();
    }

    
    if (stat(file_path, &st) == 0) {
        std::cout << "File size: " << st.st_size << " bytes" << std::endl;
    } else {
        perror("stat failed");
    }

    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        // write(STDOUT_FILENO, buffer, bytesRead); // write to stdout
    }

    if (bytesRead == -1) {
        std::cerr << "read failed: " << strerror(errno) << '\n';
        return Str();
    }

    close(fd);

    Str new_str = (const char*) buffer;
    return new_str.cut_to_substr(0, (unsigned int) st.st_size);
}

void File::echo_file_linux(Str str){

    const char* file_path = get_current_path_c_str();

    int fd = open(file_path, O_WRONLY);
    if (fd == -1) {
        std::cerr << "open failed: " << strerror(errno) << '\n';
        log_error("LIB", "FILE", get_current_path());

        error = Err();
        error.severity = err_s::Error; // default
        error.module = err_m::Lib;
        error.type = err_t::ERRNO ;
        error.file = __FILE__;
        error.message = "echo_file_linux(): Failed to open file for writing. ";
        log_error(error);


        close(fd);
        return;
    }

    void* buffer_to_write = (void*) str.to_c_str();

    ssize_t bytes_written = write(fd, buffer_to_write, sizeof(buffer_to_write));
    if(bytes_written == -1)
        std::cerr << "Write failed: " << strerror(errno) << '\n';

    close(fd);
    return;

}


#elif  PH_WINDOWS



Str File::cat_core_file_windows()
{

    const char* file_path = absolute_core_path.to_c_str();

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


#endif










// LEGACY BELOW







std::string File::cat_file_as_string(){

    std::string string;
    std::ifstream _ifstream;

    

    if(use_core_path)
        _ifstream.open(physimos_core_path_);
    else
        _ifstream.open(relative_path_);
    
    if(!_ifstream.is_open()){
        std::string error_msg = "File: cat_file_as_string. Path: " + physimos_core_path_;
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
        std::string error_msg = "File: cat_file_as_string. Path: " + physimos_core_path_;
        plib::plog_error("CONFIG", "READ_FILE", error_msg);
    }
    
    successful_read = true;
    return string;
}



Str File::cat_file_as_str(){

    std::string string = cat_file_as_string();
    return Str(string.c_str());

}




