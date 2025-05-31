
#include "file.hh"




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






Str File::cat_core_file_xplat() {


#ifdef PH_WINDOWS

    return cat_core_file_window();

#elif  PH_LINUX

    return cat_core_file_linux();

#endif


}




#ifdef PH_WINDOWS


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


#elif  PH_LINUX


Str File::cat_core_file_linux()
{
    const char* file_path = absolute_core_path.to_c_str();

    char buffer[10024];

    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        std::cerr << "open failed: " << strerror(errno) << '\n';
        return Str();
    }

    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytesRead); // write to stdout
    }

    if (bytesRead == -1) {
        std::cerr << "read failed: " << strerror(errno) << '\n';
        return Str();
    }

    close(fd);

    return Str ( (const char*) buffer );
}
#endif


