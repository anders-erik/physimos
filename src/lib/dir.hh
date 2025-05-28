#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

#include "lib/file.hh"


namespace fs = std::filesystem;


class Dir {
    std::string path_str;
    bool valid_dir_during_creation = false;

    void process_dir(){
        if (fs::exists(path_str) && fs::is_directory(path_str))
            valid_dir_during_creation = true;
        else
            std::cout << "Non valid Dir." << std::endl;
            
    }

public:

    Dir() {};
    Dir(std::string path_str){
        new_path(path_str);
    };

    void new_path(std::string new_path_str){
        path_str = new_path_str;
        process_dir();
    }

    std::vector<File> get_philes_with_extension(std::string ext){
        std::vector<File> philes;
        std::vector<std::string> file_path_str = get_file_paths_with_extension(ext);

        for(std::string& path_str : file_path_str){
            File phile {path_str};
            philes.push_back(phile);
        }

        return philes;
    }

    std::vector<std::string> get_file_paths_with_extension(std::string extension_to_match){
        
        std::vector<std::string> file_path_str;
        
        try {
            for (const fs::directory_entry& entry : fs::directory_iterator(path_str)) {

                fs::path path = entry.path();

                std::string extension = path.extension();
                std::string extension_no_dot = extension.substr(1, extension.size()-1);

                if(extension_no_dot == extension_to_match)
                    file_path_str.push_back(path);

            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error: in Dir.get_file_paths_with_extension(). " << e.what() << std::endl;
        }

        return file_path_str;
    }
};