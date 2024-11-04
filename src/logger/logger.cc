
#include "logger.hh"

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>


namespace logger {

    // Naive implmeentation of clearing the file
    void init(){
        // std::filesystem::remove_all("logs/");
        std::filesystem::create_directory("logs/");

        std::ofstream modelLogFile("logs/models.log");
        modelLogFile << "";
        modelLogFile.close();

        std::ofstream textureLogFile("logs/textures.log");
        textureLogFile << "";
        textureLogFile.close();
    }

    void log(Logtype logtype, std::string logString){

        std::ofstream logFile;
        std::string logFileName;

        switch (logtype)
        {
        case Logtype::Models :
            logFileName = "logs/models.log";
            break;
        case Logtype::Textures:
            logFileName = "logs/textures.log";
            break;
        
        default:
            std::cerr << "NO MATCHING LOGGING MODE" << std::endl;
            break;
        }

        // std::ofstream logFile(logFileName); // Open the file for writing
        logFile.open(logFileName, std::ios_base::app);

        if (!logFile.is_open()) {
            std::cerr << "Unable to open file! [ " << logFileName << " ]" << std::endl;
        }
        else {
            logFile << logString << std::endl;
        }

    }


    

}