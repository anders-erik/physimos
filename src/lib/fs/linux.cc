
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


Str file_cat_as_str(const char* path)
{
    struct stat st;
    
    // const char* file_path = get_current_path_c_str();

    char buffer[10024];


    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        std::cerr << "open failed: " << strerror(errno) << '\n';
        return Str();
    }

    
    if (stat(path, &st) == 0) {
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



}


#endif