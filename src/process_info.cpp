
#include "unistd.h"

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include <cstdio>
#include <memory>
#include <stdexcept>
// #include "stdio.h"

#include "process_info.hpp"
#include "lib/string.hh"

std::string physimos_root_dir = "";
std::string installed_root_dir_linux = "~/.cache/physimos/";


namespace fs = std::filesystem;
// By redirecting to stdout all output is captured.
std::string run_subcommand_redir_stderr_to_stdout(std::string command_str){

    std::string returnString = "";
    
    unsigned int BUFFER_SIZE = 1000;
    char buffer[BUFFER_SIZE];

    FILE* pipe = popen(command_str.data(), "r");

    // int ch;
    if (pipe) {

        while (!feof(pipe)) {
            if (fgets(buffer, BUFFER_SIZE, pipe) != nullptr) {
                returnString += buffer;
            }
        }

        pclose(pipe);
        
        return returnString;
    }


    return std::string("");
    
}



// has to be run from within the physimos project!
// TODO: if the git command fails my whole check breaks my program...
std::string getGitRepoRootDir() {

    std::string cwdIsGitRepo_command = "git rev-parse --is-inside-work-tree  2>&1";
    std::string cwdIsGitRepo_string = run_subcommand_redir_stderr_to_stdout(cwdIsGitRepo_command);
    if (cwdIsGitRepo_string != "true")
        return std::string("");


    std::string getRepoRootDirCommand = "git rev-parse --show-toplevel 2>&1";
    std::string repoRootDir_output = run_subcommand_redir_stderr_to_stdout(getRepoRootDirCommand);
    if (repoRootDir_output.size() == 0)
        return std::string("");

    
    return repoRootDir_output;
}

std::string physimos_root_dir_or_die(){

    // Cache
    if (physimos_root_dir != "")
        return physimos_root_dir;


    // Environment var
    char* PHYSIMOS_ROOT_DIR = std::getenv("PHYSIMOS_ROOT_DIR");
    if (!plib::cstr_is_empty_or_null(PHYSIMOS_ROOT_DIR))
        return std::string(PHYSIMOS_ROOT_DIR);


    // Try git repo
    std::string git_repo_dir = getGitRepoRootDir();
    if (git_repo_dir.size() > 0)
        return git_repo_dir;

    std::cout << "UNABLE TO FIND physimos_root_dir. Exiting" << std::endl;
    
    exit(1);
}

plib::Result get_physimos_root_dir(){

    std::string tmp_physimos_root_dir = "";

    // Cache
    if (physimos_root_dir != "")
        return plib::Result {plib::Ok, &physimos_root_dir};
    

    // Environment var
    char* PHYSIMOS_ROOT_DIR = std::getenv("PHYSIMOS_ROOT_DIR");
    if (!plib::cstr_is_empty_or_null(PHYSIMOS_ROOT_DIR))
        return plib::Result{ plib::Ok, new std::string(PHYSIMOS_ROOT_DIR) };


    // Try git repo
    std::string git_repo_dir = getGitRepoRootDir();
    if (git_repo_dir.size() > 0)
        return plib::Result{ plib::Ok, new std::string(git_repo_dir) };



    return plib::Result {plib::Error, new plib::ResultInfo {plib::RootDirNotFound, "Unable to find the physimos root dir."}};
}




Process process;
long pageSize;


// Inflate memory
#include <vector>
std::vector<double> doubleBloater = {};
std::string memBloater = "";


void process_info_init() {
    // std::cout  << std::endl;

    // BLOAT MEMORY
    // for(int i = 0; i<1000; i++)
    //     doubleBloater.push_back(1.123123);
    // memBloater = memBloater + "asdfashfjkasfhdkshdkfhakshdfafakjshdfkhasjdfkahkdafhskfhdkasfhjkashdkjshdfjaskdhfkasdjfhkasjdfhksajdghfkghdfagdfjksghdfjksgda";
    
    int pid = getpid();
    process.pid = (int) pid;

    // std::cout << "pid = " << process.pid << std::endl;
    // printf("Process ID = %i \n", process.pid);

    // MEMORY USAGE
    std::string line;
    std::ifstream proc("/proc/self/status");

    if (!proc) {
        std::cerr << "Error opening /proc/self/status\n";
        return;
    }

    while (std::getline(proc, line)) {
        if (line.find("VmRSS:") == 0) { 
            // std::cout << "Memory Usage: " << std::endl;
            // std::cout << line << std::endl;
        }
        else if (line.find("RssAnon:") == 0){
            // std::cout << line << std::endl;
            // std
            // std::string RssAnon_mem_string = line.substr(0, 9);
            int anonInt;
            sscanf(line.c_str(), "RssAnon: %d kB", &anonInt);
            std::cout << "Memory usage (RssAnon) = " << anonInt << " kB" << std::endl;
            
            // for (char& c : RssAnon_mem_string) {
            //     c;
            // }
        }
        else if (line.find("RssFile:") == 0) {
            // std::cout << line << std::endl;
        }
        else if (line.find("RssShmem:") == 0) {
            // std::cout << line << std::endl;
        }
    }




    // MEMORY USING STATM
    pageSize = sysconf(_SC_PAGE_SIZE);
    // std::cout << "pageSize = " << pageSize << std::endl;
    
    std::ifstream statm("/proc/self/statm");
    if (!statm) {
        std::cerr << "Error opening /proc/self/statm\n";
        return;
    }
    std::getline(statm, line);
    // std::cout << "" << line << std::endl;
    // std::string first_token = line.substr(1, line.find(' '));
    // int first_value = std::atoi(first_token.data());
    // std::cout << "Memory = " << first_value*pageSize << std::endl;
    
    // int 
    // std::cout << "Memory = " << first_token << std::endl;
    
}


