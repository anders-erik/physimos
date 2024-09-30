
#include "resources.hh"

// #include <map>
#include <string>

#include <iostream>
#include <filesystem>
#include <fstream>

// #include "object_types.hh"
#include "logger.hh"

namespace res {

    std::vector<ModelFormat> modelFormatList;



    ModelFormat* getModelFormat(std::string modelName){
        for(ModelFormat& _mf : modelFormatList){
            // std::cout << "_mf.name = " << _mf.name << std::endl;
            
            if(_mf.name == modelName)
                return &_mf;
        }
        
        return nullptr;
    }

    // Creates a list of available models and their formats
    void createInventory(){

        /* 
            MODELS
        */
        std::string modelPath = "./resources/models"; 
        std::string psoPath = "./resources/models/pso"; // path for the smaller pso-models


        logger::log(logger::Logtype::Models, "MODELS: \n");   

        logger::log(logger::Logtype::Models, "OBJ: ");
        for (const auto& entry : std::filesystem::directory_iterator(modelPath)) {
            if (entry.is_directory()) {
                // std::cout << entry.path().string() << std::endl;
                if (entry.path().string() == psoPath)
                    continue;
                
                std::string dirname = entry.path().filename().string();
                logger::log(logger::Logtype::Models, " - " + dirname);

                ModelFormat& _mf =  modelFormatList.emplace_back();
                _mf.name = dirname;
                _mf.fileType = ModelFileType::obj;
                // _mf.vertStructure = ModelVertStucture::p3n3t2; // NOT ALWAYS!
            }
        }

        logger::log(logger::Logtype::Models, "\nPSO: ");
        for (const auto& entry : std::filesystem::directory_iterator(psoPath)) {
            if (entry.is_regular_file()) {
                
                std::string filename = entry.path().filename().string();

                logger::log(logger::Logtype::Models, " - " + filename);

                ModelFormat& _mf = modelFormatList.emplace_back();
                _mf.name = filename;
                _mf.fileType = ModelFileType::pso;
                // _mf.vertStructure = ModelVertStucture::p3n3t2; // NOT ALWAYS!
            }
        }

    }

    // LOADED TEXTURES
    // std::map<objects::MODELNAME, unsigned int> textureMap;

    





    // extern std::vector<unsigned char> obj_textureDataBuffer;
    // extern unsigned int obj_imgWidth;
    // extern unsigned int obj_imgHeight;
    // void createGlTexture(objects::MODELNAME _modelname, std::vector<unsigned char> _obj_textureDataBuffer, unsigned int& obj_imgWidth, unsigned int& obj_imgHeight) {

    //     if (_modelname == MODELNAME::blend_cube_texture_1){

    //     }
    // }

    // CHECKS IF THE MODEL HAS BEEN LOADED


}