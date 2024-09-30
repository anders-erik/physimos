
#include "logger.hh"

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>


namespace logger {


    void init(){
        // std::filesystem::remove_all("logs/");
        // std::filesystem::create_directory("logs/");

        std::ofstream outfile("logs/models.log");
        outfile << "";
        outfile.close();
    }

    void log(Logtype logtype, std::string logString){

        std::ofstream logFile;
        std::string logFileName;

        switch (logtype)
        {
        case Logtype::Models :
            logFileName = "logs/models.log";
            break;
        
        default:
            std::cerr << "NO MATCHING LOGGING MODE" << std::endl;
            break;
        }

        // std::ofstream logFile(logFileName); // Open the file for writing
        logFile.open(logFileName, std::ios_base::app);

        if (!logFile.is_open()) {
            std::cerr << "Unable to open file!" << std::endl;
        }
        else {
            logFile << logString << std::endl;
        }

    }


    

}