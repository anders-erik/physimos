#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H


typedef struct Process {
    int pid;
    int memory;
} Process;

void process_info_init();


#endif