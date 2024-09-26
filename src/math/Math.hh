#ifndef MATH_HPP
#define MATH_HPP

namespace math {

    typedef double pvec3[3];
    

    bool areEqual_v3(pvec3 a, pvec3 b);

    void add_v3(pvec3 a, pvec3 b);
    void sub_v3(pvec3 a, pvec3 b);
    double dot_v3(pvec3 a, pvec3 b);
    double* cross_v3(pvec3 a, pvec3 b, pvec3 _cross);
    void div_v3(pvec3 a, pvec3 b);


    void print_v3(pvec3 _vec3);
}




#endif
