#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include <string>

#include "lib/result.hh"

typedef struct Process {
    int pid;
    int memory;
} Process;

void process_info_init();

bool argument_flag_exists(int argc, char** argv, std::string flag);

std::string run_subcommand_redir_stderr_to_stdout(std::string command_str);

std::string extern installed_root_dir_linux;

std::string getGitRepoRootDir();
plib::Result get_physimos_root_dir();
std::string physimos_root_dir_or_die();

#endif