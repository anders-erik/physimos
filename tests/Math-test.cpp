
#include <iostream>
#include <cassert>

#include "Math.hpp"

// Enable easy additional messaging ?
// https://en.cppreference.com/w/cpp/error/assert
#define assertm(exp, msg) assert(((void)msg, exp))



// The manually confirmed value(s) that we want to equal
math::pvec3 goalVector = {0};
double goalDouble = 0.0;
// The vector whose value we change and compare to the goal
math::pvec3 testVector = { 0 };
double testDouble = 0.0;


void resetVectors(){
    goalVector[0] = 0.0;
    goalVector[1] = 0.0;
    goalVector[2] = 0.0;

    testVector[0] = 0.0;
    testVector[1] = 0.0;
    testVector[2] = 0.0;
}

void setTestVector(math::pvec3 newVector){
    testVector[0] = newVector[0];
    testVector[1] = newVector[1];
    testVector[2] = newVector[2];
}
void setGoalVector(double x, double y, double z) {
    goalVector[0] = x;
    goalVector[1] = y;
    goalVector[2] = z;
}


int main(){
    std::cout << "-------------------------" << std::endl << std::endl;
    
    std::cout << "Physimos Math Tests started:" << std::endl << std::endl;


    math::pvec3 a1 = { 1.0, 2.0, 3.0 };
    math::pvec3 a2 = { 1.0, 2.0, 3.0 };
    math::pvec3 an = { -1.0, -2.0, -3.0 };

    math::pvec3 b1 = { 4.0, 5.0, 6.0 };
    math::pvec3 b2 = { 4.0, 5.0, 6.0 };

    math::pvec3 c = { 4554.033, 22.0, -12323.02222 };

    math::pvec3 z = { 0, 0, 0 };
    




    /* 
        areEqual_v3
    */
    assert(("NOT EQUAL!", math::areEqual_v3(a1, a2)));
    assert(("NOT EQUAL!", math::areEqual_v3(b1, b2)));
    assert(("NOT EQUAL!", !math::areEqual_v3(a1, b1)));
    assert(("NOT EQUAL!", !math::areEqual_v3(a2, b2)));

    std::cout << "passed : math::areEqual_v3" << std::endl;




    /*
        add_v3
    */
    setTestVector(a1);
    math::add_v3(testVector, a2);
    setGoalVector(2.0, 4.0, 6.0);
    assert(("FAILED ADD!", math::areEqual_v3(testVector, goalVector)));
    

    setTestVector(a1);
    math::add_v3(testVector, an);
    setGoalVector(0.0, 0.0, 0.0);
    assert(("FAILED ADD!", math::areEqual_v3(testVector, goalVector)));

    // c = 4554.033, 22.0, -12323.02222 
    setTestVector(a1);
    math::add_v3(testVector, c);
    setGoalVector(4555.033, 24.0, -12320.02222);
    assert(("FAILED ADD!", math::areEqual_v3(testVector, goalVector)));


    setTestVector(a1);
    math::add_v3(testVector, z);
    setGoalVector(1.0, 2.0, 3.0);
    assert(("FAILED ADD!", math::areEqual_v3(testVector, goalVector)));

    std::cout << "passed : math::add_v3" << std::endl;



    /*
        sub_v3
    */
    setTestVector(a1);
    math::sub_v3(testVector, a1);
    setGoalVector(0.0, 0.0, 0.0);
    assert(("FAILED ADD!", math::areEqual_v3(testVector, goalVector)));

    setTestVector(a1);
    math::sub_v3(testVector, an);
    setGoalVector(2.0, 4.0, 6.0);
    assert(("FAILED ADD!", math::areEqual_v3(testVector, goalVector)));

    setTestVector(a1);
    math::sub_v3(testVector, a1);
    setGoalVector(0.0, 0.0, 0.0);
    assert(("FAILED ADD!", math::areEqual_v3(testVector, goalVector)));

    std::cout << "passed : math::sub_v3" << std::endl;




    /*
           dot_v3
    */
    setTestVector(a1);
    testDouble = math::dot_v3(testVector, a1);
    goalDouble = 14.0;
    assert(("FAILED ADD!", testDouble == goalDouble));

    setTestVector(a1);
    testDouble = math::dot_v3(testVector, an);
    goalDouble = -14.0;
    assert(("FAILED ADD!", testDouble == goalDouble));

    setTestVector(a1);
    testDouble = math::dot_v3(testVector, a1);
    goalDouble = 14.00000001;
    assert(("FAILED ADD!", testDouble != goalDouble));

    std::cout << "passed : math::dot_v3" << std::endl;



    /*
        cross_v3
    */

    setTestVector(z);
    math::cross_v3(a1, b1, testVector);
    // https://www.wolframalpha.com/input?i=cross+product+calculator&assumption=%7B%22F%22%2C+%22CrossProduct%22%2C+%22crossVector1%22%7D+-%3E%22%7B1%2C+2%2C+3%7D%22&assumption=%7B%22F%22%2C+%22CrossProduct%22%2C+%22crossVector2%22%7D+-%3E%22%7B4%2C+5%2C+6%7D%22
    // https://www.emathhelp.net/en/calculators/linear-algebra/cross-product-calculator/?u=1%2C+2%2C+3&v=4%2C+5%2C+6
    setGoalVector(-3.0, 6.0, -3.0);
    assert(("FAILED ADD!", math::areEqual_v3(testVector, goalVector)) );

    setTestVector(z);
    math::cross_v3(c, b1, testVector);
    // https://onlinemschool.com/math/assistance/vector/multiply1/
    // { 61747.1111; -76616.28688; 22682.165 }
    setGoalVector(61747.1111, -76616.28688, 22682.165);
    assert(("FAILED ADD!", math::areEqual_v3(testVector, goalVector)));

    std::cout << "passed : math::cross_v3" << std::endl;



    /*
        div_v3
    */
    setTestVector(a1);
    math::div_v3(testVector, a1);
    setGoalVector(1.0, 1.0, 1.0);
    assert(("FAILED ADD!", math::areEqual_v3(testVector, goalVector)));

    setTestVector(b1);
    math::div_v3(testVector, a1);
    setGoalVector(4.0, 2.5, 2.0);
    assert(("FAILED ADD!", math::areEqual_v3(testVector, goalVector)));

    setTestVector(a1);
    math::div_v3(testVector, a1);
    setGoalVector(1.0, 1.0, 1.000001);
    assert(("FAILED ADD!", !math::areEqual_v3(testVector, goalVector)));

    std::cout << "passed : math::div_v3" << std::endl;







    std::cout << std::endl << "Physimos Math Tests Done!" << std::endl << std::endl;
    std::cout << "-------------------------" << std::endl;
    
    return 0;
}
