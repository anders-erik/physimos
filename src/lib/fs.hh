#ifndef PLIB_FS_HH
#define PLIB_FS_HH

#include <string>
#include <vector>

namespace plib {

    typedef enum FS_STATUS: unsigned char {
        FsOk = 0,
        FsError = 1,
    } FS_STATUS;


    std::string fs_cat(std::string path_std_string);
    std::vector<unsigned char>& fs_cat_bin(std::string path_std_string);

    FS_STATUS fs_echo(std::string path_std_string, std::string contents);
    FS_STATUS fs_echo_append(std::string path_std_string, std::string contents);
    FS_STATUS fs_echo_bin(std::string path_std_string, std::vector<unsigned char>& bin_contents);

}



#endif

