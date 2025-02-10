#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <filesystem>

#include "obj.hh"
#include "obj_types.hh"

using namespace pmodel;
using namespace pmodel::pobj;


// Log all passed asserts
bool PTEST_VERBOSE_LOG = false;
bool PTEST_EXIT_ON_FAILED_ASSERT = true;


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

// Primary ptest assert
void assertTrue(bool expression, std::string message){

    // on ok asserts we always return
    if (expression) {
        if (PTEST_VERBOSE_LOG)
            passed(message);
        return;
    }

    // Always log failed asserts
    failed(message);

    if (PTEST_EXIT_ON_FAILED_ASSERT){
        std::cout << "Failed Assert with 'PTEST_EXIT_ON_FAILED_ASSERT = true'." << std::endl;
        std::cout << "Exiting." << std::endl;
        exit(0);
    }
    
    return;
}


// has to be run from within the physimos project!
std::string getGitRepoRootDir(){
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


    // Two different ways of getting the physimos root directory
    char* PHYSIMOS_ROOT_DIR = std::getenv("PHYSIMOS_ROOT_DIR");
    std::string physimosRepoDir = getGitRepoRootDir();


    // Test Objects
    Obj dummyObj = Obj();
    pmodel::pobj::Obj triangle = Obj();
    

    // triangle.
    // FILESYSTEM
    std::cout << "Starting: filesystem" << std::endl;
    std::filesystem::path dummyFilePath("/dev/null/nonexistent");
    std::filesystem::path triangleModelFilePath = physimosRepoDir + "/resources/models/triangle/triangle.obj";
    std::filesystem::path triangleModelFileDir = physimosRepoDir + "/resources/models/triangle";
    std::filesystem::path triangleModelFileName = "triangle.obj";
    std::string           triangleModelName = "triangle";

    assertTrue(dummyObj.setModelPaths(dummyFilePath) == ObjLoadStatus::PathError, "dummyObj.setModelPaths(dummyFilePath) == ObjLoadStatus::PathError");
    assertTrue(triangle.setModelPaths(triangleModelFilePath) == ObjLoadStatus::Ok, "triangle.setModelPaths(triangleModelFilePath) == ObjLoadStatus::Ok");
    assertTrue(triangleModelFilePath.compare(triangle.modelFilePath) == 0, "triangleModelFilePath.compare(triangle.modelFilePath) == 0");
    assertTrue(triangleModelFileDir.compare(triangle.modelFileDir) == 0, "triangleModelFileDir.compare(triangle.modelFileDir) == 0");
    assertTrue(triangleModelFileName.compare(triangle.modelFileName) == 0, "triangleModelFileName.compare(triangle.modelFileName) == 0");
    std::cout << "Done    : filesystem" << std::endl << std::endl;



    // VERTEX VERTEX
    std::cout << "Starting: Vertex Coordinate Line (v)" << std::endl;
    triangle.processVertexCoordinateLine("v -1.000000 -0.666667 0.000000");
    assertTrue(triangle.vertCoords[0].x == -1.000000f, "triangle.vertCoords[0].x == -1.000000f");
    assertTrue(triangle.vertCoords[0].y == -0.666667f, "triangle.vertCoords[0].y == -0.666667f");
    assertTrue(triangle.vertCoords[0].z == 0.000000f, "triangle.vertCoords[0].z ==  0.000000f");
    assertTrue(triangle.vertCoords.size() == 1, "triangle.vertCoords.size() == 1");
    triangle.vertCoords.pop_back();
    std::cout << "Done    : Vertex Coordinate Line (v)" << std::endl << std::endl;



    // VERTEX TEXTURE
    std::cout << "Starting: Vertex Texture Line (vt)" << std::endl;
    triangle.processVertexTextureLine("vt 0.511719 0.992188");
    assertTrue(triangle.vertTextureCoords[0].u == 0.511719f, "triangle.vertTextureCoords[0].u == 0.511719f");
    assertTrue(triangle.vertTextureCoords[0].v == 0.992188f, "triangle.vertTextureCoords[0].v == 0.992188f");
    assertTrue(triangle.vertTextureCoords[0].w == emptyTextureCoord, "triangle.vertTextureCoords[0].w == emptyTextureCoord");
    assertTrue(triangle.vertTextureCoords.size() == 1, "triangle.vertTextureCoords.size() == 1");
    triangle.vertTextureCoords.pop_back();
    std::cout << "Done    : Vertex Texture Line (vt)" << std::endl << std::endl;



    // NORMAL NORMAL
    std::cout << "Starting: Vertex Normal Line (vn)" << std::endl;
    triangle.processVertexNormalLine("vn -0.0000 -0.0000 1.0000");
    assertTrue(triangle.vertNormals[0].x == -0.0000f, "triangle.vertNormals[0].x == -0.0000f");
    assertTrue(triangle.vertNormals[0].y == -0.0000f, "triangle.vertNormals[0].y == -0.0000f");
    assertTrue(triangle.vertNormals[0].z ==  1.0000f, "triangle.vertNormals[0].z ==  1.0000f");
    assertTrue(triangle.vertNormals.size() == 1, "triangle.vertNormals.size() == 1");
    triangle.vertNormals.pop_back();
    std::cout << "Done    : Vertex Normal Line (vn)" << std::endl << std::endl;



    // TRIANGLE FACE
    std::cout << "Starting: Face Line (f)" << std::endl;
    // vertex_index/texture_index/normal_index
    triangle.processFaceLineDataSegments("f 1/1/1 2/2/1 3/3/1");
    assertTrue(triangle.triangleFacesI[0].v1.vc_i, "triangle.triangleFacesI[0].v1.vc_i");
    assertTrue(triangle.triangleFacesI[0].v1.vc_i, "triangle.triangleFacesI[0].v1.vc_i");
    assertTrue(triangle.triangleFacesI[0].v1.vc_i, "triangle.triangleFacesI[0].v1.vc_i");
    assertTrue(triangle.triangleFacesI[0].v2.vc_i, "triangle.triangleFacesI[0].v2.vc_i");
    assertTrue(triangle.triangleFacesI[0].v2.vc_i, "triangle.triangleFacesI[0].v2.vc_i");
    assertTrue(triangle.triangleFacesI[0].v2.vc_i, "triangle.triangleFacesI[0].v2.vc_i");
    assertTrue(triangle.triangleFacesI[0].v3.vc_i, "triangle.triangleFacesI[0].v3.vc_i");
    assertTrue(triangle.triangleFacesI[0].v3.vc_i, "triangle.triangleFacesI[0].v3.vc_i");
    assertTrue(triangle.triangleFacesI[0].v3.vc_i, "triangle.triangleFacesI[0].v3.vc_i");
    triangle.triangleFacesI.pop_back();
    std::cout << "Done    : Face Line (f)" << std::endl << std::endl;



    // LOAD WHOLE OBJ FILE
    std::cout << "Starting: Full Obj File" << std::endl;
    assertTrue(triangle.loadObjFile() == ObjLoadStatus::Ok, "triangle.loadObjFile() == ObjLoadStatus::Ok");
    assertTrue(triangle.vertCoords.size() == 3, "triangle.vertCoords.size() == 3");
    assertTrue(triangle.vertTextureCoords.size() == 3, "triangle.vertTextureCoords.size() == 3");
    assertTrue(triangle.vertNormals.size() == 1, "triangle.vertNormals.size() == 1");
    assertTrue(triangle.triangleFacesI.size() == 1, "triangle.triangleFacesI.size() == 1");

    std::filesystem::path triangleMtlFilePath = physimosRepoDir + "/resources/models/triangle/triangle.mtl";
    assertTrue(triangleMtlFilePath == triangle.mtlFilePath, "triangleMtlFilePath == triangle.mtlFilePath");
    std::cout << "Done    : Full Obj File" << std::endl << std::endl;


    std::cout << std::endl << "-------------------------" << std::endl;
    return 0;
}
