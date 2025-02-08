#ifndef PMATH_HPP
#define PMATH_HPP

namespace pmath {

    // Internally always uses double. Can return float representation. 
    struct Vec3
    {
        double data[3];
        float  fdata[3] { 0.0f, 0.0f, 0.0f};
        void setData(double data0, double data1, double data2);
        float* get_fdata();
        
        Vec3&  copy(const Vec3& rhs);

        bool   operator==(Vec3);
        Vec3&  operator+=(const Vec3& rhs);
        Vec3&  operator-=(const Vec3& rhs);
        Vec3&  operator*=(double rhs);

        double dot(const Vec3& rhs);
        Vec3&  cross(const Vec3& rhs);
        Vec3   toCross(const Vec3& rhs);
        
        void   prettyPrint();

        Vec3() : data{ 0.0, 0.0, 0.0 } {};
        Vec3(double a, double b, double c) : data{ a, b, c } {};
        // Vec3(double a, double b, double c) {
        //     data[0] = a;
        //     data[1] = b;
        //     data[2] = c;
        // };
        ~Vec3() {};
    };
    
    

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
