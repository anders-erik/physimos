
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "pso_loader.hh"

#include "bmp_loader.hpp"

#include "resources.hh"


namespace res {



// UTIL
std::string modelsDirectory = "resources/models/pso/";
std::string modelPath;


// PSO






ModelVertStucture vertStructure;

std::vector<float> vertexFloatBuffer;
int vertexCount = 0;


PsoLoader* loadPsoModel(std::string _modelname) {

    PsoLoader* psoLoader = new PsoLoader();

    modelPath = modelsDirectory + _modelname;


    std::cout << "Loading model: " << modelPath << ". ";
    // modelPath = _modelPath;

    vertexCount = 0;
    vertexFloatBuffer.clear();


    std::ifstream modelFile;
    std::stringstream modelStream;
    std::string modelString;

    modelFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // Load into Memory
        modelFile.open(modelPath);
        modelStream << modelFile.rdbuf();
        modelString = modelStream.str();
        modelFile.close();


        // GET META DATA ON FIRST LINE [example : vdf:p3c3]
        std::string firstLine; 
        std::getline(modelStream, firstLine, '\n'); // read the first meta-line
        std::stringstream metaStringStream(firstLine);

        std::string metaPair;
        while (std::getline(metaStringStream, metaPair, ';')){
            std::string key = metaPair.substr(0, metaPair.find(":"));
            std::string value = metaPair.substr(metaPair.find(":")+1, metaPair.size()-1);
            if(key == "vdf"){
                if(value == "p3c3"){
                    psoLoader->vertStructure = res::ModelVertStucture::p3c3;
                }
            }
        }
        


        std::string item;
        float number;


        while (std::getline(modelStream, item, ',')) {
            
            std::stringstream itemStream(item);
            itemStream >> number;

            psoLoader->vertexFloatBuffer.push_back(number);
            psoLoader->vertexCount++;
        }

        std::cout << "OK.  [" << psoLoader->vertexFloatBuffer.size() << " values]" << " (" << __FILE__ << "::" << __LINE__ << ")" << std::endl;

    }
    catch (std::ifstream::failure& e)
    {
        std::cout << " EROOR. [" << psoLoader->vertexFloatBuffer.size() << " values]" << std::endl;
        std::cout << "ERROR::READING_PSO_FILE" << e.what() << std::endl;
    }



    // std::cout << modelString;


    return psoLoader;


}


}