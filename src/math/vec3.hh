#ifndef MATH_VEC3_HH
#define MATH_VEC3_HH


namespace pmath {

    typedef struct Vec3f_base {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
    } Vec3f_base;

    typedef struct Vec4f_base {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;
    } Vec4f_base;

    typedef struct Vec3d_base {
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
    } Vec3d_base;

    typedef struct Mat4f_base {
        Vec4f_base x;
        Vec4f_base y;
        Vec4f_base z;
        Vec4f_base w;
    } Mat4f_base;


    /** Internally uses double. Can return float representation. */
    typedef struct Vec3
    {
        double data[3];
        // float  fdata[3] { 0.0f, 0.0f, 0.0f};
        void setData(double data0, double data1, double data2);
        /** Returns the underlying data as float data. */
        Vec3f_base get_f_base();
        
        Vec3&  copy(const Vec3& rhs);

        Vec3& operator=(const Vec3& rhs);
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
        Vec3(double a) : data{ a, a, a } {};
        
        ~Vec3() {};
    } Vec3;

    typedef struct Mat4 {

    } Mat4;
    
}




#endif
