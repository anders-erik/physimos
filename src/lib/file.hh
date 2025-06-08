#pragma once

#include <string>

#include "process.hh"
#include "str.hh"
#include "res.hh"

#include "err.hh"


class File {

    enum path_t {
        core,
        cwd,
    } path_type;

    Err error; // Error values returned using Res<{}, Err>

    Str relative_path; // Input path

    Str physimos_root_dir;  // current physimos root dir
    Str absolute_core_path; // the assembled absolute path

    
public:

    File() = default;
    File(Str relative_path_str);

    void set_rel_path(Str& path_str);
    Str& get_current_path();
    bool core_exists();

    void file_echo_overwrite_first_strsize_chars(Str str);

    /** Read whole file as a Str. */
    ResMove<Str> cat_as_str_core_xplat();
    /** Static: Read whole file as a Str. */
    static ResMove<Str> cat_as_str_core_xplat(Str& path_str);

private:

    const char* get_current_path_c_str();
    void update_absolute_path();








    // LEGACY BELOW


public:

    bool use_core_path = false;
    bool successful_read = false;

    std::string physimos_core_path_;
    std::string relative_path_;
    std::string file_contents_;
    
    File(std::string relative_path) : relative_path_ {relative_path} {
        std::string physiomos_dir = physimos_root_dir_or_die();
        physimos_core_path_ = physiomos_dir + "/" + relative_path;
    };


    // Copy file contents using std::string indirection
    Str cat_as_str_core(){

        use_core_path = true;
        successful_read = false;

        Str str = cat_file_as_str();
        // Str str = cat_as_str_core_xplat();

        return str;
    }
    void set_path_core(Str& path_str){

        // Str
        relative_path = path_str;
        physimos_root_dir = physimos_root_dir_or_die_str();
        absolute_core_path = physimos_root_dir + "/" + relative_path;

        // std::string
        std::string physimos_root_dir = physimos_root_dir_or_die();
        relative_path_ = relative_path.to_std_string();
        physimos_core_path_ = physimos_root_dir + "/" + relative_path_;
    }



    std::string copy_as_string_cwd(){
        successful_read = false;
        return cat_file_as_string();
    }



private:

    // Str indirection for std::string call
    Str cat_file_as_str();
    std::string cat_file_as_string();

};

