
#include <iostream>

#include "vec3.hh"

using namespace pmath;



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


int main(){
    std::cout << std::endl;
    std::cout << "-------------------------" << std::endl << std::endl;

    std::cout << "sizeof(Vec3) = " << sizeof(Vec3) << std::endl;
    std::cout << "sizeof(Mat4f_base) = " << sizeof(Mat4f_base) << std::endl;
    std::cout << "sizeof(Vec3[2]) = " << sizeof(Vec3[2]) << std::endl;
    std::cout << std::endl;
    

    Vec3 z = Vec3(0.0, 0.0, 0.0);
    Vec3 i = Vec3(1.0, 1.0, 1.0);
    Vec3 in = Vec3(1.0, 1.0, 1.0);

    Vec3 tmp = Vec3(0, 0, 0);

    
    Vec3 a = Vec3(1.0, 2.0, 3.0);
    Vec3 a0 = Vec3(1.0, 2.0, 3.0); // remains unchanged and used to reset a
    Vec3 an = Vec3(-1.0, -2.0, -3.0);

    Vec3 b = Vec3(4.0, 5.0, 6.0);
    Vec3 b0 = Vec3(4.0, 5.0, 6.0); // remains unchanged and used to reset b
    Vec3 bn = Vec3(-4.0, -5.0, -6.0);

    Vec3 c = Vec3(4554.033, 22.0, -12323.02222);
    Vec3 c0 = Vec3(4554.033, 22.0, -12323.02222);
    Vec3 cn = Vec3(-4554.033, -22.0, 12323.02222);


    /*
        =, .copy()
    */
    std::cout << "=, .copy()" << std::endl;
    (a.copy(b) == Vec3(4.0, 5.0, 6.0)) ? passed("a.copy(b) == Vec3(4.0, 5.0, 6.0)") : failed("a.copy(b) == Vec3(4.0, 5.0, 6.0)");
    (a.copy(a0) == a0) ? passed("a.copy(a0) == a0") : failed("a.copy(a0) == a0");
    (a = b) == Vec3(4.0, 5.0, 6.0) ? passed("(a = b) == Vec3(4.0, 5.0, 6.0)") : failed("(a = b) == Vec3(4.0, 5.0, 6.0)");
    (a = a0) == a0 ? passed("(a = a0) == a0") : failed("(a = a0) == a0");
    std::cout << std::endl;


    /* 
        ==
    */
   std::cout << "==" << std::endl;
    (a == a) ? passed("a == a") : failed("a == a");
    (a == a0) ? passed("a == a0") : failed("a == a0");
    (a0 == a) ? passed("a0 == a") : failed("a0 == a");

    !(a == an) ? passed("!(a == a3)") : failed("!(a == a3)");
    !(a == b) ? passed("!(a == a3)") : failed("!(a == a3)");
    std::cout<< std::endl;


    /*
        +=
        -=
        *=
    */
    std::cout << "+=, -=, *=" << std::endl;
    (a += z) == a0 ? passed("(a += z) == a0") : failed("(a += z) == a0");
    (a -= z) == a0 ? passed("(a -= z) == a0") : failed("(a -= z) == a0");
    (a += a0) == Vec3(2.0, 4.0, 6.0) ? passed("(a += a0) == Vec3(2.0, 4.0, 6.0)") : failed("(a += a0) == Vec3(2.0, 4.0, 6.0)");
    (a -= a0) == a0 ? passed("(a -= a0) == a0") : failed("(a -= a0) == a0");
    (a *= 0) == Vec3(0.0, 0.0, 0.0) ? passed("(a *= 0) == Vec3(0.0, 0.0, 0.0)") : failed("(a *= 0) == Vec3(0.0, 0.0, 0.0)");
    (a += a0) == a0 ? passed("(a += a0) == a0") : failed("(a += a0) == a0");
    std::cout << std::endl;



    /*
        .dot()
    */
    std::cout << ".dot()" << std::endl;
    (a.dot(z) == 0) ? passed("a.dot(z) == 0") : failed("a.dot(z) == 0");
    a.copy(a0); // reset
    (a.dot(a) == 14) ? passed("a.dot(a) == 14") : failed("a.dot(a) == 14");
    (a.dot(an) == -14.0) ? passed("a.dot(an) == -14.0") : failed("a.dot(an) == -14.0");
    std::cout << std::endl;



    /*
        .cross()
        .toCross()
    */
    std::cout << ".cross(), .toCross()" << std::endl;

    // https://www.wolframalpha.com/input?i=cross+product+calculator&assumption=%7B%22F%22%2C+%22CrossProduct%22%2C+%22crossVector1%22%7D+-%3E%22%7B1%2C+2%2C+3%7D%22&assumption=%7B%22F%22%2C+%22CrossProduct%22%2C+%22crossVector2%22%7D+-%3E%22%7B4%2C+5%2C+6%7D%22
    // https://www.emathhelp.net/en/calculators/linear-algebra/cross-product-calculator/?u=1%2C+2%2C+3&v=4%2C+5%2C+6
    a.toCross(b) == Vec3(-3.0, 6.0, -3.0) ? passed("a.toCross(b) == Vec3(-3.0, 6.0, -3.0)") : failed("a.toCross(b) == Vec3(-3.0, 6.0, -3.0)");
    a.cross(b) == Vec3(-3.0, 6.0, -3.0) ? passed("a.cross(b) == Vec3(-3.0, 6.0, -3.0)") : failed("a.cross(b) == Vec3(-3.0, 6.0, -3.0)");
    a.copy(a0); // reset because cross is in place

    // https://onlinemschool.com/math/assistance/vector/multiply1/
    c.cross(b) == Vec3(61747.1111, -76616.28688, 22682.165) ? passed("c.cross(b) == Vec3(61747.1111, -76616.28688, 22682.165)") : failed("c.cross(b) == Vec3(61747.1111, -76616.28688, 22682.165)");
    c.copy(c0);
    c.cross(bn) == Vec3(-61747.1111, 76616.28688, -22682.165) ? passed("c.cross(bn) == Vec3(-61747.1111, 76616.28688, -22682.165)") : failed("c.cross(bn) == Vec3(-61747.1111, 76616.28688, -22682.165)");
    c.copy(c0);




    // Memory investigation
    // std::cout << "a  = " << &a << std::endl;
    // a = { 1.5, 0.5, 0.5 };
    // std::cout << "a  = " << &a << std::endl;
    // a = *(new Vec3(1.5, 0.5, 0.5)); // old a-object is no longer accessible?
    // std::cout << "a  = " << &a << std::endl; // memory location is still the same!?!?
    // a = {1.5, 0.5, 0.5}; 
    // std::cout << "a  = " << &a << std::endl;

    // std::cout << "a0 = " << &a0 << std::endl;
    // std::cout << "an = " << &an << std::endl;

    // Vec3 vecvec = { 1.5, 0.5, 0.5 }; // convenient initialization!
    // std::cout << "vecvec = " << &vecvec << std::endl;

    // std::cout << "a.data[0] = " << a.data[0] << std::endl;
    // std::cout << "a.fdata[0] = " << a.fdata[0] << std::endl;
    
    // std::cout << "s" << a << std::endl;
    


    std::cout << std::endl << "-------------------------" << std::endl;
    
    return 0;
}
