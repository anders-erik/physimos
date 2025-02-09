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
    char* getRepoRootDirCommand = "git rev-parse --show-toplevel";

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
    
    std::string physimosRepoDir = getGitRepoDir();

    Obj obj1 = Obj();
    obj1.modelName = "obj1";
    obj1.modelFilePath = physimosRepoDir + std::string("/resources/models/cube/cube.obj");

    Obj obj2 = Obj();
    obj2.modelName = "obj2";
    obj2.modelFilePath = std::string("/dev/null/nonexistent");


    // FILESYSTEM
    std::cout << "filesystem" << std::endl << std::endl;

    std::filesystem::exists(obj1.modelFilePath) ? passed(" std::filesystem::exists(obj1.modelFilePath)") : failed(" std::filesystem::exists(obj1.modelFilePath)");
    !std::filesystem::exists(obj2.modelFilePath) ? passed("!std::filesystem::exists(obj2.modelFilePath)") : failed("!std::filesystem::exists(obj2.modelFilePath)");
    
    std::cout << std::endl;





    std::cout << std::endl << "-------------------------" << std::endl;
    
    return 0;
}
