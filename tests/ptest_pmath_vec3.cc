
#include <iostream>

#include "vec3.hh"

using namespace pmath;



void passed(std::string msg){
    std::cout << "PASSED: " << msg << std::endl;
    
}

void failed(std::string msg) {
    std::cout << "FAILED: " << msg << std::endl;
}


int main(){
    std::cout << std::endl;
    std::cout << "-------------------------" << std::endl << std::endl;
    
    // std::cout << "Ptest:  pmath::vec3  started:" << std::endl << std::endl;

    
    Vec3 a1 = Vec3(1.0, 2.0, 3.0);
    Vec3 a2 = Vec3(1.0, 2.0, 3.0);
    Vec3 a3 = Vec3(-1.0, -2.0, -3.0);

    Vec3 b1 = Vec3(4.0, 5.0, 6.0);
    Vec3 b2 = Vec3(4.0, 5.0, 6.0);

    Vec3 c = Vec3(4554.033, 22.0, -12323.02222);

    Vec3 z = Vec3(0, 0, 0);

    


    /* 
        ==
    */
    (a1 == a1) ? passed("asdf") : failed("asdf");




    // std::cout << std::endl << "Physimos Math Tests Done!" << std::endl << std::endl;
    std::cout << std::endl << "-------------------------" << std::endl;
    
    return 0;
}
