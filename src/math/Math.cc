
#include <iostream>
#include <cmath>

#include "Math.hh"

namespace math {


    bool areEqual_v3(pvec3 a, pvec3 b){
        if (a[0] == b[0] && a[1] == b[1] && a[2] == b[2])
            return true;
        else
            return false;
    }

    /**
     * @brief  Add second argument to the first.  [element-wise]
     * @param math::pvec3 a
     * @param math::pvec3 b
     */
    void add_v3(pvec3 a, pvec3 b) {

        a[0] = a[0] + b[0];
        a[1] = a[1] + b[1];
        a[2] = a[2] + b[2];
    }

    void sub_v3(pvec3 a, pvec3 b) {

        a[0] = a[0] - b[0];
        a[1] = a[1] - b[1];
        a[2] = a[2] - b[2];
    }

    double dot_v3(pvec3 a, pvec3 b) {
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    }

    double* cross_v3(pvec3 a, pvec3 b, pvec3 _cross) {
        // pvec3 _cross = {0};
        
        _cross[0] =   a[1] * b[2] - a[2] * b[1];
        _cross[1] = - a[0] * b[2] + a[2] * b[0];
        _cross[2] =   a[0] * b[1] - a[1] * b[0];

        return _cross;
    }

    /**
     * @brief  Divides the second argument by the first. [element-wise]
     * @param math::pvec3 a
     * @param math::pvec3 b
     */
    void div_v3(pvec3 a, pvec3 b) {

        a[0] = a[0] / b[0];
        a[1] = a[1] / b[1];
        a[2] = a[2] / b[2];
    }
   



    void print_v3(pvec3 _vec3){
        std::cout << "_____________________" << std::endl << std::endl;
        std::cout << "print_v3 :" << std::endl;
        std::cout << "[0] = " << _vec3[0] << std::endl;
        std::cout << "[1] = " << _vec3[1] << std::endl;
        std::cout << "[2] = " << _vec3[2] << std::endl;
        std::cout << "_____________________" << std::endl;
    }



}