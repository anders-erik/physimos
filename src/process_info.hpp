#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include <lib/result.hh>

typedef struct Process {
    int pid;
    int memory;
} Process;

void process_info_init();

plib::Result get_physimos_root_dir();

#endif