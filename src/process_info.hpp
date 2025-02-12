#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include <string>

#include "lib/result.hh"

typedef struct Process {
    int pid;
    int memory;
} Process;

void process_info_init();

std::string run_subcommand(std::string command_str);

std::string getGitRepoRootDir();
plib::Result get_physimos_root_dir();

#endif