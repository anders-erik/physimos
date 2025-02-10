
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "obj.hh"
#include "obj_types.hh"

#include "res/bmp_loader.hpp"

// returns a vector of the resulting string sections.
// e.g. "hello there" -> {"hello", "there"} with delimiter " ".
std::vector<std::string> split_str(std::string str, char delimiter) {

    std::stringstream str_stream(str);
    std::string section;

    std::vector<std::string> returnVector;

    int loop_index = 0;

    // Vertex #1
    // VertexI vertex1_indices = {0, 0, 0};
    while (std::getline(str_stream, section, delimiter))
    {
        returnVector.push_back(section);
    }
    loop_index = 0;


    return returnVector;
}


namespace pmodel {

float emptyTextureCoord = -2222.2;


// UTIL
std::string modelsDirectory = "resources/models/";

// MTL
bool hasTextureMap = false;
float Kd[3] = { 0.0f, 0.0f, 0.0f };
std::string texturePath = "";

std::vector<unsigned char> obj_textureDataBuffer;
unsigned int obj_imgWidth = 0;
unsigned int obj_imgHeight = 0;

// OBJ
ObjMesh objMesh;

std::vector<float> vertexBuffer;
std::vector<int> vertexIndices;


ObjLoadStatus Obj::loadObjFile() {
    if (!std::filesystem::exists(modelFilePath)){
        return ObjLoadStatus::PathError;
    }

    // Read Whole Obj File
    std::ifstream modelFile;
    std::stringstream modelStream;
    // std::string modelString;

    modelFile.open(modelFilePath);
    modelStream << modelFile.rdbuf();
    modelFile.close();

    // objFileContents = modelStream.str();


    // Parse file
    std::vector<std::string> objFileLines = split_str(modelStream.str(), '\n');

    for(std::string line : objFileLines){
        // std::cout << "" << line << std::endl;
        std::vector<std::string> line_segments = split_str(line, ' ');

        if (line_segments[0] == "v"){
            // std::cout << "v" << std::endl;
            processVertexCoordinateLine(line);
        }   
        else if (line_segments[0] == "vt") {
            // std::cout << "vt" << std::endl;
            processVertexTextureLine(line);
        }
        else if (line_segments[0] == "vn") {
            // std::cout << "vn" << std::endl;
            processVertexNormalLine(line);
        }
        else if (line_segments[0] == "f") {
            // std::cout << "f" << std::endl;
            processFaceLineDataSegments(line);
        }
    }



    return ObjLoadStatus::Ok;
}


ObjLoadStatus Obj::processVertexCoordinateLine(std::string line) {
    // std::cout << "" << (float)std::atof(lineDataSegments[1].data()) << std::endl;

    std::vector<std::string> line_segments = split_str(line, ' ');

    float x = (float)std::atof(line_segments[1].data());
    float y = (float)std::atof(line_segments[2].data());
    float z = (float)std::atof(line_segments[3].data());

    VertexCoord vertexCoord = { x, y, z, 1.0,};

    // The w value is optional
    if (line_segments.size() > 4)
        vertexCoord.w = (float)std::atof(line_segments[4].data());

    vertCoords.push_back(vertexCoord);

    return ObjLoadStatus::Ok;
}


ObjLoadStatus Obj::processVertexTextureLine(std::string line) {
    std::vector<std::string> line_segments = split_str(line, ' ');

    float u = (float)std::atof(line_segments[1].data());
    float v = (float)std::atof(line_segments[2].data());

    VertexTextureCoord vertexTexture = {u, v, emptyTextureCoord};

    if (line_segments.size() > 3)
        vertexTexture.w = (float)std::atof(line_segments[3].data());

    vertTextureCoords.push_back(vertexTexture);
    return ObjLoadStatus::Ok;
}

ObjLoadStatus Obj::processVertexNormalLine(std::string line) {
    std::vector<std::string> line_segments = split_str(line, ' ');

    float x = (float)std::atof(line_segments[1].data());
    float y = (float)std::atof(line_segments[2].data());
    float z = (float)std::atof(line_segments[3].data());

    VertexNormal vertexNormal = {x, y, z};

    vertNormals.push_back(vertexNormal);

    return ObjLoadStatus::Ok;
}

ObjLoadStatus Obj::processFaceLineDataSegments(std::string line) {
    std::vector<std::string> line_segments = split_str(line, ' ');
    
    std::vector <std::string> vert1_index_strings = split_str(line_segments[1], '/');
    std::vector <std::string> vert2_index_strings = split_str(line_segments[2], '/');
    std::vector <std::string> vert3_index_strings = split_str(line_segments[3], '/');

    TriangleFaceI triangleFaceI;
    
    // vertex_index/texture_index/normal_index
    VertexI vertexi_1;
    vertexi_1.vc_i = std::atoi(vert1_index_strings[0].data());
    vertexi_1.vt_i = std::atoi(vert1_index_strings[1].data());
    vertexi_1.vn_i = std::atoi(vert1_index_strings[2].data());

    VertexI vertexi_2;
    vertexi_2.vc_i = std::atoi(vert2_index_strings[0].data());
    vertexi_2.vt_i = std::atoi(vert2_index_strings[1].data());
    vertexi_2.vn_i = std::atoi(vert2_index_strings[2].data());

    VertexI vertexi_3;
    vertexi_3.vc_i = std::atoi(vert3_index_strings[0].data());
    vertexi_3.vt_i = std::atoi(vert3_index_strings[1].data());
    vertexi_3.vn_i = std::atoi(vert3_index_strings[2].data());

    triangleFaceI.v1 = vertexi_1;
    triangleFaceI.v2 = vertexi_2;
    triangleFaceI.v3 = vertexi_3;


    triangleFacesI.push_back(triangleFaceI);
    
    // std::stringstream vertex_stream(lineDataSegments[0]);
    // std::string index_string;
    // int loop_index = 0;

    // // Vertex #1
    // VertexI vertex1_indices = {0, 0, 0};
    // while (std::getline(vertex_stream, index_string, '/'))
    // {
    //     vertex1_indices. = std::atoi(index_string.data());
    //     loop_index++;
    //     // std::cout << index_string << " " << std::endl;
    // }
    // loop_index = 0;


    return ObjLoadStatus::Ok;
}



void obj_loadMtlFromFile(std::string modelName) {

    obj_textureDataBuffer.clear();
    hasTextureMap = false;

    // grab the path to file with mtl-extension
    std::string mtlPath = modelsDirectory + modelName + "/" + modelName + ".mtl";


    // std::cout << "Loading obj model: " << mtlPath << ". " << std::endl;
    // this->modelPath = objPath;

    std::ifstream modelFile;
    std::stringstream modelStream;
    std::string modelString;

    modelFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // std::cout << "000000000000" << std::endl;
        modelFile.open(mtlPath);



        modelStream << modelFile.rdbuf();

        modelString = modelStream.str();

        modelFile.close();

        std::string line;
        std::string item;
        // float number;

        // int vertexCounter = 0;
        // Stepping through each comma-separated value


        while (std::getline(modelStream, line)) {
            // Remove any leading or trailing whitespace from the item
            std::stringstream itemStream(line);
            // itemStream >> number;


            // Split line
            std::vector<std::string> lineSegments;
            std::string segment;


            while (std::getline(itemStream, segment, ' '))
            {
                lineSegments.push_back(segment);
                // std::cout << segment << " ";
            }
            // std::cout << std::endl;

            // Make sure the line ins't empty
            if (lineSegments.size() < 1)
                continue;

            if (lineSegments[0] == "Kd") {
                // std::cout << "" << lineSegments[1] << std::endl;
                Kd[0] = (float)std::atof(lineSegments[1].data());
                Kd[1] = (float)std::atof(lineSegments[2].data());
                Kd[2] = (float)std::atof(lineSegments[3].data());
                // VertexCoord vertexCoord = {
                //     (float)std::atof(lineSegments[1].data()),
                //     (float)std::atof(lineSegments[2].data()),
                //     (float)std::atof(lineSegments[3].data()),
                //     1.0,
                // };
                // objMesh.v.push_back(vertexCoord);

                hasTextureMap = false;
            }
            else if (lineSegments[0] == "map_Kd") {
                // std::cout << "MAP_KD ------------------" << std::endl;

                std::string modelDirectory;
                const size_t last_slash_idx = mtlPath.rfind('/');
                if (std::string::npos != last_slash_idx)
                {
                    modelDirectory = mtlPath.substr(0, last_slash_idx);
                }

                std::string texturePath = modelDirectory + "/" + lineSegments[1];

                bmp_loader_loadBMPFile(texturePath);
                obj_textureDataBuffer = bmp_getImageDataBuffer();
                obj_imgWidth = bmp_getWidth();
                obj_imgHeight = bmp_getHeight();

                hasTextureMap = true;
            }
        }


    }
    catch (std::ifstream::failure& e)
    {
        // std::cout << " ERROR. [" << this->vertices.size() << " values]" << std::endl;
        std::cout << "ERROR::READING_OBJ_FILE" << e.what() << std::endl;
    }



    // std::cout << modelString;
    // std::cout << "DONE READING MTL FILE " << mtlPath << std::endl;

    // std::cout << "" << std::endl;

}






float* obj_loadKdFromFile(std::string mtlPath) {
    // std::cout << "Loading obj model: " << mtlPath << ". " << std::endl;
    // this->modelPath = objPath;

    std::ifstream modelFile;
    std::stringstream modelStream;
    std::string modelString;

    modelFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // std::cout << "000000000000" << std::endl;
        modelFile.open(mtlPath);



        modelStream << modelFile.rdbuf();

        modelString = modelStream.str();

        modelFile.close();

        std::string line;
        std::string item;
        // float number;

        // int vertexCounter = 0;
        // Stepping through each comma-separated value


        while (std::getline(modelStream, line)) {
            // Remove any leading or trailing whitespace from the item
            std::stringstream itemStream(line);
            // itemStream >> number;


            // Split line
            std::vector<std::string> lineSegments;
            std::string segment;


            while (std::getline(itemStream, segment, ' '))
            {
                lineSegments.push_back(segment);
                // std::cout << segment << " ";
            }
            // std::cout << std::endl;

            if (lineSegments.size() > 0 && lineSegments[0] == "Kd") {
                std::cout << "" << lineSegments[1] << std::endl;
                Kd[0] = (float)std::atof(lineSegments[1].data());
                Kd[1] = (float)std::atof(lineSegments[2].data());
                Kd[2] = (float)std::atof(lineSegments[3].data());
                // VertexCoord vertexCoord = {
                //     (float)std::atof(lineSegments[1].data()),
                //     (float)std::atof(lineSegments[2].data()),
                //     (float)std::atof(lineSegments[3].data()),
                //     1.0,
                // };
                // objMesh.v.push_back(vertexCoord);
            }
        }
    }
    catch (std::ifstream::failure& e)
    {
        // std::cout << " ERROR. [" << this->vertices.size() << " values]" << std::endl;
        std::cout << "ERROR::READING_OBJ_FILE" << e.what() << std::endl;
    }



    // std::cout << modelString;
    // std::cout << "DONE READING MTL FILE " << mtlPath << std::endl;

    // std::cout << "" << std::endl;

    return Kd;
}


std::vector<float> obj_getVertexBuffer_v_vt_vn() {
    return vertexBuffer;
}


void obj_loadFromFile(std::string modelName) {

    // grab the path to file with obj-extension
    std::string objPath = modelsDirectory + modelName + "/" + modelName + ".obj";

    // std::cout << "Loading obj model: " << modelName << ". " << std::endl;
    // this->modelPath = objPath;

    vertexBuffer.clear();
    // objMesh.clear();
    objMesh.v.clear();
    objMesh.vn.clear();
    objMesh.vt.clear();
    objMesh.fi.clear();
    objMesh.f.clear();
    // std::vector<VertexCoord> v;
    // std::vector<VertexTextureCoord> vt;
    // std::vector<VertexNormal> vn;
    // std::vector<ObjFaceIndex> fi;
    // std::vector<ObjFace> f;


    std::ifstream modelFile;
    std::stringstream modelStream;
    std::string modelString;

    modelFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // std::cout << "000000000000" << std::endl;
        modelFile.open(objPath);



        modelStream << modelFile.rdbuf();

        modelString = modelStream.str();

        modelFile.close();

        std::string line;
        std::string item;
        // float number;

        int vertexCounter = 0;
        // Stepping through each comma-separated value
        while (std::getline(modelStream, line)) {
            // Remove any leading or trailing whitespace from the item
            std::stringstream itemStream(line);
            // itemStream >> number;


            // Split line
            std::vector<std::string> lineSegments;
            std::string segment;


            while (std::getline(itemStream, segment, ' '))
            {
                lineSegments.push_back(segment);

            }

            if (lineSegments[0] == "v") {
                // std::cout << "" << lineSegments[0] << std::endl;
                VertexCoord vertexCoord = {
                    (float)std::atof(lineSegments[1].data()),
                    (float)std::atof(lineSegments[2].data()),
                    (float)std::atof(lineSegments[3].data()),
                    1.0,
                };
                objMesh.v.push_back(vertexCoord);
            }
            if (lineSegments[0] == "vn") {
                // std::cout << "" << lineSegments[0] << std::endl;
                VertexNormal vertexNorm = {
                    (float)std::atof(lineSegments[1].data()),
                    (float)std::atof(lineSegments[2].data()),
                    (float)std::atof(lineSegments[3].data()),
                };
                objMesh.vn.push_back(vertexNorm);
            }
            if (lineSegments[0] == "vt") {
                // std::cout << "" << lineSegments[0] << std::endl;
                VertexTextureCoord vertexTextCoord = {
                    (float)std::atof(lineSegments[1].data()),
                    (float)std::atof(lineSegments[2].data()),
                    0.0,
                };
                objMesh.vt.push_back(vertexTextCoord);
            }



            // FACE INDECES
            if (lineSegments[0] == "f") {
                // std::cout << "" << lineSegments[0] << std::endl;
                ObjFaceIndex objFaceIndex;

                // Split FACE indices



                // FACE INDEX 1
                std::stringstream faceSegmentStream1(lineSegments[1]);

                std::vector<std::string> faceIndexStrings;
                std::string faceIndexString;

                while (std::getline(faceSegmentStream1, faceIndexString, '/'))
                {
                    faceIndexStrings.push_back(faceIndexString);

                }

                ObjFaceVertexIndex objFaceVertexIndex1 = {
                    std::atoi(faceIndexStrings[0].data()),
                    std::atoi(faceIndexStrings[1].data()),
                    std::atoi(faceIndexStrings[2].data()),
                };
                objFaceIndex.vert1 = objFaceVertexIndex1;



                // FACE INDEX 2
                std::stringstream faceSegmentStream2(lineSegments[2]);

                faceIndexStrings.clear();

                while (std::getline(faceSegmentStream2, faceIndexString, '/'))
                {
                    faceIndexStrings.push_back(faceIndexString);

                }

                ObjFaceVertexIndex objFaceVertexIndex2 = {
                    std::atoi(faceIndexStrings[0].data()),
                    std::atoi(faceIndexStrings[1].data()),
                    std::atoi(faceIndexStrings[2].data()),
                };
                objFaceIndex.vert2 = objFaceVertexIndex2;



                // FACE INDEX 3
                std::stringstream faceSegmentStream3(lineSegments[3]);

                faceIndexStrings.clear();

                while (std::getline(faceSegmentStream3, faceIndexString, '/'))
                {
                    faceIndexStrings.push_back(faceIndexString);

                }

                ObjFaceVertexIndex objFaceVertexIndex3 = {
                    std::atoi(faceIndexStrings[0].data()),
                    std::atoi(faceIndexStrings[1].data()),
                    std::atoi(faceIndexStrings[2].data()),
                };
                objFaceIndex.vert3 = objFaceVertexIndex3;

                objMesh.fi.push_back(objFaceIndex);


                vertexCounter += 3;
            }

            // std::cout << "segment = ";
            // for(std::string lineSegment : lineSegments){
            //     std::cout << lineSegment << " ";
            //     if(lineSegment)
            // }
            // std::cout << std::endl;



            // vertexCounter++;


            // Add the parsed float to the vector
            // this->vertices.push_back(number);

        }

        // this->vertexCount = vertexCounter;

        // std::cout << "OK.  [" << objMesh.v.size() << " VERTEX COORD ROWS]" << " (" << __FILE__ << "::" << __LINE__ << ")" << std::endl;
        // std::cout << "OK.  [" << objMesh.vt.size() << " TEXTURE COORD ROWS]" << " (" << __FILE__ << "::" << __LINE__ << ")" << std::endl;
        // std::cout << "OK.  [" << objMesh.vn.size() << " VERTEX NORMAL ROWS]" << " (" << __FILE__ << "::" << __LINE__ << ")" << std::endl;
        // std::cout << "OK.  [" << objMesh.fi.size() << " FACE ROWS]" << " (" << __FILE__ << "::" << __LINE__ << ")" << std::endl;

        // ADD ACTUAL FACE VERTEX DATA
        ObjFace objFace;
        for (ObjFaceIndex objFaceIndex : objMesh.fi) {
            // std::cout << "fi ";
            // std::cout << objFaceIndex.vert1.v << "/" << objFaceIndex.vert1.vt << "/" << objFaceIndex.vert1.vn << " ";
            // std::cout << objFaceIndex.vert2.v << "/" << objFaceIndex.vert2.vt << "/" << objFaceIndex.vert2.vn << " ";
            // std::cout << objFaceIndex.vert3.v << "/" << objFaceIndex.vert3.vt << "/" << objFaceIndex.vert3.vn << std::endl;

            objFace.vert1.v = objMesh.v[objFaceIndex.vert1.v - 1];
            objFace.vert1.vt = objMesh.vt[objFaceIndex.vert1.vt - 1];
            objFace.vert1.vn = objMesh.vn[objFaceIndex.vert1.vn - 1];

            objFace.vert2.v = objMesh.v[objFaceIndex.vert2.v - 1];
            objFace.vert2.vt = objMesh.vt[objFaceIndex.vert2.vt - 1];
            objFace.vert2.vn = objMesh.vn[objFaceIndex.vert2.vn - 1];

            objFace.vert3.v = objMesh.v[objFaceIndex.vert3.v - 1];
            objFace.vert3.vt = objMesh.vt[objFaceIndex.vert3.vt - 1];
            objFace.vert3.vn = objMesh.vn[objFaceIndex.vert3.vn - 1];

            objMesh.f.push_back(objFace);




            // ADD ALL FACES TO THE VERTEX BUFFER
            // for (ObjFace objFace : objMesh.f) {

                // VERT 1
            vertexBuffer.push_back(objFace.vert1.v.x);
            vertexBuffer.push_back(objFace.vert1.v.y);
            vertexBuffer.push_back(objFace.vert1.v.z);

            vertexBuffer.push_back(objFace.vert1.vn.x);
            vertexBuffer.push_back(objFace.vert1.vn.y);
            vertexBuffer.push_back(objFace.vert1.vn.z);

            vertexBuffer.push_back(objFace.vert1.vt.u);
            vertexBuffer.push_back(objFace.vert1.vt.v);

            // VERT 2
            vertexBuffer.push_back(objFace.vert2.v.x);
            vertexBuffer.push_back(objFace.vert2.v.y);
            vertexBuffer.push_back(objFace.vert2.v.z);

            vertexBuffer.push_back(objFace.vert2.vn.x);
            vertexBuffer.push_back(objFace.vert2.vn.y);
            vertexBuffer.push_back(objFace.vert2.vn.z);

            vertexBuffer.push_back(objFace.vert2.vt.u);
            vertexBuffer.push_back(objFace.vert2.vt.v);


            // VERT 3
            vertexBuffer.push_back(objFace.vert3.v.x);
            vertexBuffer.push_back(objFace.vert3.v.y);
            vertexBuffer.push_back(objFace.vert3.v.z);

            vertexBuffer.push_back(objFace.vert3.vn.x);
            vertexBuffer.push_back(objFace.vert3.vn.y);
            vertexBuffer.push_back(objFace.vert3.vn.z);

            vertexBuffer.push_back(objFace.vert3.vt.u);
            vertexBuffer.push_back(objFace.vert3.vt.v);


            // }

            // std::cout << "f ";


            // std::cout << objFace.vert1.v << "/" << objFace.vert1.vt << "/" << objFace.vert1.vn << " ";
            // std::cout << objFace.vert2.v << "/" << objFace.vert2.vt << "/" << objFace.vert2.vn << " ";
            // std::cout << objFace.vert3.v << "/" << objFace.vert3.vt << "/" << objFace.vert3.vn << std::endl;
        }
    }
    catch (std::ifstream::failure& e)
    {
        // std::cout << " ERROR. [" << this->vertices.size() << " values]" << std::endl;
        std::cout << "ERROR::READING_OBJ_FILE" << e.what() << std::endl;
    }



    // std::cout << modelString;
    // std::cout << "DONE READING OBJ FILE " << objPath  << std::endl;

    // std::cout << "vertexBuffer.size() = " << vertexBuffer.size() << std::endl;


    // std::cout << ""<< std::endl;


}


}