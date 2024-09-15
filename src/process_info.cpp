
#include "unistd.h"

#include <iostream>
#include <fstream>
#include <string>
// #include "stdio.h"

#include "process_info.hpp"


Process process;
long pageSize;


// Inflate memory
#include <vector>
std::vector<double> doubleBloater = {};
std::string memBloater = "";


void process_info_init() {
    std::cout  << std::endl;

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


