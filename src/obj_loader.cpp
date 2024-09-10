
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "obj_loader.hpp"


// MTL

float Kd[3] = {0.0f, 0.0f, 0.0f};
std::string texturePath = "";




// OBJ
ObjMesh objMesh;

std::vector<float> vertexBuffer;
std::vector<int> vertexIndices;



float * obj_loadKdFromFile(std::string mtlPath){
    std::cout << "Loading obj model: " << mtlPath << ". " << std::endl;
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
                std::cout << segment << " ";
            }
            std::cout << std::endl;

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
    std::cout << "DONE READING MTL FILE " << mtlPath << std::endl;

    std::cout << "" << std::endl;

    return Kd;
}


std::vector<float> obj_getVertexBuffer_v_vt_vn(){
    return vertexBuffer;
}


void obj_loadFromFile(std::string objPath) {
    
    std::cout << "Loading obj model: " << objPath << ". " << std::endl;
    // this->modelPath = objPath;

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
            
            if(lineSegments[0] == "v"){
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
                VertexNorm vertexNorm = {
                    (float)std::atof(lineSegments[1].data()),
                    (float)std::atof(lineSegments[2].data()),
                    (float)std::atof(lineSegments[3].data()),
                };
                objMesh.vn.push_back(vertexNorm);
            }
            if (lineSegments[0] == "vt") {
                // std::cout << "" << lineSegments[0] << std::endl;
                VertexTextCoord vertexTextCoord = {
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

            }

            // std::cout << "segment = ";
            // for(std::string lineSegment : lineSegments){
            //     std::cout << lineSegment << " ";
            //     if(lineSegment)
            // }
            // std::cout << std::endl;
            
            

            vertexCounter++;


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
        for (ObjFaceIndex objFaceIndex : objMesh.fi){
            // std::cout << "fi ";
            // std::cout << objFaceIndex.vert1.v << "/" << objFaceIndex.vert1.vt << "/" << objFaceIndex.vert1.vn << " ";
            // std::cout << objFaceIndex.vert2.v << "/" << objFaceIndex.vert2.vt << "/" << objFaceIndex.vert2.vn << " ";
            // std::cout << objFaceIndex.vert3.v << "/" << objFaceIndex.vert3.vt << "/" << objFaceIndex.vert3.vn << std::endl;
            
            objFace.vert1.v = objMesh.v[objFaceIndex.vert1.v - 1];
            objFace.vert1.vt = objMesh.vt[objFaceIndex.vert1.vt - 1];
            objFace.vert1.vn = objMesh.vn[objFaceIndex.vert1.vn - 1];

            objFace.vert2.v =    objMesh.v[objFaceIndex.vert2.v - 1];
            objFace.vert2.vt = objMesh.vt[objFaceIndex.vert2.vt - 1];
            objFace.vert2.vn = objMesh.vn[objFaceIndex.vert2.vn - 1];

            objFace.vert3.v =   objMesh.v[objFaceIndex.vert3.v - 1];
            objFace.vert3.vt = objMesh.vt[objFaceIndex.vert3.vt - 1];
            objFace.vert3.vn = objMesh.vn[objFaceIndex.vert3.vn - 1];

            objMesh.f.push_back(objFace);




            // ADD ALL FACES TO THE VERTEX BUFFER
            for (ObjFace objFace : objMesh.f) {

                // VERT 1
                vertexBuffer.push_back(objFace.vert1.v.x);
                vertexBuffer.push_back(objFace.vert1.v.y);
                vertexBuffer.push_back(objFace.vert1.v.z);

                vertexBuffer.push_back(objFace.vert1.vt.u);
                vertexBuffer.push_back(objFace.vert1.vt.v);

                vertexBuffer.push_back(objFace.vert1.vn.x);
                vertexBuffer.push_back(objFace.vert1.vn.y);
                vertexBuffer.push_back(objFace.vert1.vn.z);

                // VERT 2
                vertexBuffer.push_back(objFace.vert2.v.x);
                vertexBuffer.push_back(objFace.vert2.v.y);
                vertexBuffer.push_back(objFace.vert2.v.z);

                vertexBuffer.push_back(objFace.vert2.vt.u);
                vertexBuffer.push_back(objFace.vert2.vt.v);

                vertexBuffer.push_back(objFace.vert2.vn.x);
                vertexBuffer.push_back(objFace.vert2.vn.y);
                vertexBuffer.push_back(objFace.vert2.vn.z);

                // VERT 3
                vertexBuffer.push_back(objFace.vert3.v.x);
                vertexBuffer.push_back(objFace.vert3.v.y);
                vertexBuffer.push_back(objFace.vert3.v.z);

                vertexBuffer.push_back(objFace.vert3.vt.u);
                vertexBuffer.push_back(objFace.vert3.vt.v);

                vertexBuffer.push_back(objFace.vert3.vn.x);
                vertexBuffer.push_back(objFace.vert3.vn.y);
                vertexBuffer.push_back(objFace.vert3.vn.z);

            }

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
    std::cout << "DONE READING OBJ FILE " << objPath  << std::endl;

    std::cout << "vertexBuffer.size() = " << vertexBuffer.size() << std::endl;
    

    std::cout << ""<< std::endl;
    

}