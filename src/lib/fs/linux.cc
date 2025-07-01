
#ifdef PH_LINUX

#include <fcntl.h>     // open
#include <unistd.h>    // read, close, write
#include <iostream>    // std::cerr
#include <cstring>     // strerror
#include <errno.h>     // errno
#include <sys/stat.h>

#include "str.hh"
#include "log.hh"
#include "err.hh"

#include "linux.hh"




namespace OS {


bool file_exists(const char * path)
{
    if (access(path, F_OK) == 0) {
        return true;
    } else {
        return false;
    }
}


bool file_echo_overwrite_first_strsize_chars(const char* path, Str& str)
{


    int fd = open(path, O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        std::cerr << "open failed: " << strerror(errno) << '\n';
        log_error("LIB", "FILE", Str(path));

        close(fd);
        return false;
    }

    void* buffer_to_write = (void*) str.to_c_str();

    ssize_t bytes_written = write(fd, buffer_to_write, str.size());
    if(bytes_written == -1)
        std::cerr << "Write failed: " << strerror(errno) << '\n';

    close(fd);
    return true;


}


ResMove<Str> file_cat_as_str(const char* path)
{    
    int fd = open(path, O_RDONLY);
    if (fd == -1) 
    {
        return Err {err_s::Error, err_m::Lib, err_t::ERRNO, errno, path};
    }

    // Get File Size
    struct stat st;
    if (stat(path, &st) != 0)
    {
        perror("stat failed");
        return Err{err_s::Error, err_m::Lib, err_t::filesystem, Str{"stat failed"}};
    }
    // std::cout << "File size: " << st.st_size << " bytes" << std::endl;
    int file_size = st.st_size;


    // Create buffer
    Str str {   (uint) file_size, 
                (char) 0x00      };


    // Read
    ssize_t bytesRead;
    while ((bytesRead = read(fd, str.data(), str.size())) > 0) 
    {
        // write(STDOUT_FILENO, buffer, bytesRead); // write to stdout
    }
    if (bytesRead == -1)
    {
        std::cerr << "read failed: " << strerror(errno) << '\n';
        return Err{err_s::Error, err_m::Lib, err_t::filesystem, Str{"read failed"}};
    }

    close(fd);

    return std::move(str);
}



}


#endif