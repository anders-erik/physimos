#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <filesystem>

#include "obj.hh"
#include "obj_types.hh"

using namespace pmodel;



void passed(std::string msg){
    std::cout << "pass  : " << msg << std::endl;
    
}

void failed(std::string msg) {
    std::cout << "_________________________________________________________________________________________" << std::endl;
    std::cout << "*" << std::endl;
    std::cout << "*     FAILED TEST:           " << std::endl;
    std::cout << "*" << std::endl;
    std::cout << "*     " << msg << std::endl;
    std::cout << "*" << std::endl;
    std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
}


// has to be run from within the physimos project!
std::string getGitRepoDir(){
    char getRepoRootDirCommand[] = "git rev-parse --show-toplevel";

    // int returnCode = system(getRepoRootDirCommand);
    // std::cout << "returnCode = " << returnCode << std::endl;

    FILE* pipe = popen(getRepoRootDirCommand, "r");
    char buffer[128];
    if (pipe) {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != nullptr) {
                // printf("%s", buffer);
            }
        }
        pclose(pipe);
    }
    
    std::string repoDir = std::string(buffer);
    // Clear trailing whitespace
    repoDir = repoDir.substr(0, repoDir.size()-1);
    return repoDir;
}

int main(){
    std::cout << std::endl;
    std::cout << "-------------------------" << std::endl << std::endl;


    char* PHYSIMOS_ROOT_DIR = std::getenv("PHYSIMOS_ROOT_DIR");
    
    std::string physimosRepoDir = getGitRepoDir();

    Obj dummyObj = Obj();
    dummyObj.modelName = "dummyObj";
    dummyObj.modelFilePath = std::string("/dev/null/nonexistent");

    Obj triangle = Obj();
    triangle.modelName = "triangle";
    triangle.modelFilePath = physimosRepoDir + std::string("/resources/models/triangle/triangle.obj");
    

    // FILESYSTEM
    std::cout << "Filesystem" << std::endl;
    dummyObj.loadObjFile() == ObjLoadStatus::PathError ? passed("dummyObj.loadObjFile() == ObjLoadStatus::PathError") : failed("dummyObj.loadObjFile() == ObjLoadStatus::PathError");
    std::cout << std::endl;
    


    // PROCESS OBJ LINE DATA
    std::cout << "Vertex Lines" << std::endl;

    std::cout << "v" << std::endl;
    std::cout << "triangle.processVertexCoordinateLine(\"v - 1.000000 - 0.666667 0.000000\");" << std::endl;
    triangle.processVertexCoordinateLine("v -1.000000 -0.666667 0.000000");
    // Make sure to explicitly define float, and not the implicit double precision!
    triangle.vertCoords[0].x == -1.000000f ? passed("triangle.vertCoords[0].x == -1.000000") : failed("triangle.vertCoords[0].x == -1.000000");
    triangle.vertCoords[0].y == -0.666667f ? passed("triangle.vertCoords[0].y == -0.666667") : failed("triangle.vertCoords[0].y == -0.666667");
    triangle.vertCoords[0].z ==  0.000000f ? passed("triangle.vertCoords[0].z == 0.000000") : failed("triangle.vertCoords[0].z == 0.000000");
    triangle.vertCoords.size() == 1 ? passed("vertCoords size check") : failed("vertCoords size check");
    triangle.vertCoords.pop_back();
    std::cout << std::endl;

    std::cout << "vt" << std::endl;
    std::cout << "triangle.processVertexTextureLine(\"vt 0.511719 0.992188\");" << std::endl;
    triangle.processVertexTextureLine("vt 0.511719 0.992188");
    // Make sure to explicitly define float, and not the implicit double precision!
    triangle.vertTextureCoords[0].u == 0.511719f ? passed("triangle.vertTextureCoords[0].u == 0.511719f") : failed("triangle.vertTextureCoords[0].u == 0.511719f");
    triangle.vertTextureCoords[0].v == 0.992188f ? passed("triangle.vertTextureCoords[0].v == 0.992188f") : failed("triangle.vertTextureCoords[0].v == 0.992188f");
    // std::cout << "emptyTextureCoord = " << emptyTextureCoord << std::endl;
    triangle.vertTextureCoords[0].w == emptyTextureCoord ? passed("triangle.vertTextureCoords[0].w == 0.0f") : failed("triangle.vertTextureCoords[0].w == 0.0f ");
    triangle.vertTextureCoords.size() == 1 ? passed("vertTextureCoords size check") : failed("vertTextureCoords size check");
    triangle.vertTextureCoords.pop_back();
    std::cout << std::endl;

    std::cout << "vn" << std::endl;
    std::cout << "triangle.processVertexNormalLine(\"vn - 0.0000 - 0.0000 1.0000\");" << std::endl;
    triangle.processVertexNormalLine("vn -0.0000 -0.0000 1.0000");
    // Make sure to explicitly define float, and not the implicit double precision!
    triangle.vertNormals[0].x == -0.0000f ? passed("triangle.vertNormals[0].x == -0.0000f") : failed("triangle.vertNormals[0].x == -0.0000f");
    triangle.vertNormals[0].y == -0.0000f ? passed("triangle.vertNormals[0].y == -0.0000f") : failed("triangle.vertNormals[0].y == -0.0000f");
    triangle.vertNormals[0].z == 1.0000f ? passed("triangle.vertNormals[0].z == 1.0000f") : failed("triangle.vertNormals[0].z == 1.0000f");
    triangle.vertNormals.size() == 1 ? passed("vertNormals size check") : failed("vertNormals size check");
    triangle.vertNormals.pop_back();
    std::cout << std::endl;


    std::cout << "f" << std::endl;
    std::cout << "triangle.processFaceLineDataSegments(\"f 1/1/1 2/2/1 3/3/1\");" << std::endl;
    // vertex_index/texture_index/normal_index
    triangle.processFaceLineDataSegments("f 1/1/1 2/2/1 3/3/1");
    triangle.triangleFacesI[0].v1.vc_i == 1 ? passed("triangle.triangleFaceI[0].v1.vc_i == 1") : failed("triangle.triangleFaceI[0].v1.vc_i == 1");
    triangle.triangleFacesI[0].v1.vt_i == 1 ? passed("triangle.triangleFaceI[0].v1.vt_i == 1") : failed("triangle.triangleFaceI[0].v1.vt_i == 1");
    triangle.triangleFacesI[0].v1.vn_i == 1 ? passed("triangle.triangleFaceI[0].v1.vn_i == 1") : failed("triangle.triangleFaceI[0].v1.vn_i == 1");
    triangle.triangleFacesI[0].v2.vc_i == 2 ? passed("triangle.triangleFaceI[0].v2.vc_i == 2") : failed("triangle.triangleFaceI[0].v2.vc_i == 2");
    triangle.triangleFacesI[0].v2.vt_i == 2 ? passed("triangle.triangleFaceI[0].v2.vt_i == 2") : failed("triangle.triangleFaceI[0].v2.vt_i == 1");
    triangle.triangleFacesI[0].v2.vn_i == 1 ? passed("triangle.triangleFaceI[0].v2.vn_i == 1") : failed("triangle.triangleFaceI[0].v2.vn_i == 2");
    triangle.triangleFacesI[0].v3.vc_i == 3 ? passed("triangle.triangleFaceI[0].v3.vc_i == 3") : failed("triangle.triangleFaceI[0].v3.vc_i == 3");
    triangle.triangleFacesI[0].v3.vt_i == 3 ? passed("triangle.triangleFaceI[0].v3.vt_i == 3") : failed("triangle.triangleFaceI[0].v3.vt_i == 1");
    triangle.triangleFacesI[0].v3.vn_i == 1 ? passed("triangle.triangleFaceI[0].v3.vn_i == 1") : failed("triangle.triangleFaceI[0].v3.vn_i == 3");
    triangle.triangleFacesI.pop_back();
    std::cout << std::endl;
    
    std::cout << std::endl;




    // LOAD WHOLE OBJ FILE
    std::cout << "Full Obj File" << std::endl;
    triangle.loadObjFile() == ObjLoadStatus::Ok ? passed("triangle.loadObjFile() == ObjLoadStatus::Ok") : failed("triangle.loadObjFile() == ObjLoadStatus::Ok");
    triangle.vertCoords.size() == 3 ? passed("triangle.vertCoords.size() == 3") : failed("triangle.vertCoords.size() == 3");
    triangle.vertTextureCoords.size() == 3 ? passed("triangle.vertTextureCoords.size() == 3") : failed("triangle.vertTextureCoords.size() == 3");
    triangle.vertNormals.size() == 1 ? passed("triangle.vertNormals.size() == 1") : failed("triangle.vertNormals.size() == 1");
    triangle.triangleFacesI.size() == 1 ? passed("triangle.triangleFacesI.size() == 1") : failed("triangle.triangleFacesI.size() == 1");
    std::cout << std::endl;


    std::cout << std::endl << "-------------------------" << std::endl;
    return 0;
}
