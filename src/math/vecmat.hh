
#pragma once

#include <cmath>
#include <string>

struct s2;
struct ui2;
struct i2;
struct f2;
struct d2;


struct m4f4;
struct m3f3;

// #ifndef A
//     #define A true
//     #define A0 0.0f
//     #define A1 1.57f
//     #define A2 3.14f
//     #define A2 4.71f
// #endif





struct PointD {
    double x = 0.0;
    double y = 0.0;
};

enum class Axis {
    x,
    y,
    z,
    nx,
    ny,
    nz,
};


struct s2 {
    size_t x;
    size_t y;
};

struct ui2 {
    using uint = unsigned int;
    uint x = 0;
    uint y = 0;

    ui2(uint x, uint y) : x {x}, y {y} {}; 
    ui2() = default;

};

struct i2 {
    int x = 0;
    int y = 0;

    i2(int x, int y) : x {x}, y {y} {}; 
    i2(int xy) : x {xy}, y {xy} {}; 
    i2() = default;

    bool operator==(const i2& rhs) const;

    i2 operator/(const i2& rhs);

    f2 to_f2();
};

struct i3 {
    int x = 0;
    int y = 0;
    int z = 0;

    i3(int x, int y, int z) : x {x}, y {y}, z {z} {}; 
    i3(int xyz) : x {xyz}, y {xyz}, z {xyz} {}; 
    i3() = default;
};

struct f2 
{
    float x = 0.0f;
    float y = 0.0f;

    f2() = default;
    constexpr
    f2(float x, float y) : x {x}, y {y} {}; 
    f2(float xyz) : x {xyz}, y {xyz} {}; 

    f2(double x, double y) : x { (float)x}, y {(float)y} {}; 
    f2(int x, int y) : x { (float)x}, y {(float)y} {}; 

    f2& operator+=(const f2& rhs);
    f2& operator-=(const f2& rhs);
    f2& operator*=(const f2& rhs);
    f2& operator/=(const f2& rhs);

    f2& operator+=(float scalar);
    f2& operator-=(float scalar);

    f2 operator+(const f2& rhs) const;
    f2 operator-(const f2& rhs) const;
    f2 operator*(const f2& rhs) const;
    f2 operator/(const f2& rhs) const;

    f2 operator*(float scalar) const;

    f2 operator-();

    float norm();
    /** Radians measured ccw from the x-axis. */
    float angle();
    bool is_zero();
    void set_zero();
    f2& clamp_nonzero();

    i2 to_i2() const ;

    void print(std::string prefix);
};

struct d2
{
    float x;
    float y;

    constexpr d2() : x {0.0}, y { 0.0 } {};

    d2(f2 float_2) : x ((double)float_2.x), y {(float) float_2.y} {}

};

struct f3 
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    f3() = default;
    constexpr 
    f3(float x, float y, float z) : x {x}, y {y}, z {z} {}; 
    f3(const f2 _f2, float z) : x {_f2.x}, y {_f2.y}, z {z} {}; 
    f3(float x, const f2 _f2) : x {x}, y {_f2.x}, z {_f2.y} {}; 
    // f3(float xyz) : x {xyz}, y {xyz}, z {xyz} {}; 
    // f3() : x {0.0f}, y {0.0f}, z {0.0f} {}; 

    f3& operator=(const f3& rhs);
    f3& operator+=(const f3& rhs);
    f3& operator-=(const f3& rhs);
    f3& operator*=(const f3& rhs);
    f3& operator/=(const f3& rhs);

    f3 operator+(const f3& rhs) const;
    f3 operator-(const f3& rhs) const;
    f3 operator*(const f3& rhs);

    f3 operator-();

    f3& operator*=(float scalar);

    f3 operator+(float scalar);
    f3 operator-(float scalar);
    f3 operator*(float scalar) const;
    f3 operator/(float scalar) const;

    f2 to_xy() const;
    f2 yaw_pitch();
    
    /** x^2 + y^2 + z^2 */
    float norm_sq();
    /** sqrt ( x^2 + y^2 + z^2 ) */
    float norm() const;
    /** this / norm */
    f3 unit() const;
    f3 cross(const f3& rhs);
    float dot(const f3& rhs);
    float angle(const f3& rhs);

    f3& reflect_z();
    f3& reflect(Axis axis);

    void set_zero();
    bool is_zero();

    void matmul(m4f4& matrix);
    void matmul(m3f3& matrix);

    float* pointer();

    void print(std::string name);

    static constexpr f3 X() {return {1.0f, 0.0f, 0.0f}; }
    static constexpr f3 Y() {return {0.0f, 1.0f, 0.0f}; }
    static constexpr f3 Z() {return {0.0f, 0.0f, 1.0f}; }
};

inline f3 operator*(float scalar, const f3& vec)
{
    return vec * scalar;
}


struct f4 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;

    constexpr
    f4(float x, float y, float z, float w) : x {x}, y {y}, z {z}, w {w} {}; 
    // f4(float x, float y, float z) : x {x}, y {y}, z {z}, w {1.0f} {}; 
    f4(const f3 _f3, float w) : x {_f3.x}, y {_f3.y}, z {_f3.z}, w {w} {}; 
    f4(float xyz) : x {xyz}, y {xyz}, z {xyz}, w {1.0f} {}; 
    f4(f3 _f3) : x {_f3.x}, y {_f3.y}, z {_f3.z}, w {1.0f} {}; 
    // f4() : x {0.0f}, y {0.0f}, z {0.0f}, w {1.0f} {}; 
    f4() = default;

    float* pointer();
};


struct m2f2 
{
    f2 x;
    f2 y;

    m2f2& operator=(m2f2& rhs);

    f2 mult_vec(f2 _f2); /** Multiply the matrix by a f2 vector e.g. f2 = m2f2 x f2 * */

    void set_to_identity();

    void print();

    m2f2() : 
        x { f2(1.0f, 0.0f) }, 
        y { f2(0.0f, 1.0f) } {};
};


struct m3f3 
{
    f3 x;
    f3 y;
    f3 z;

    m3f3& operator=(const m3f3& rhs);
    /** copy = this x rhs 
        this = copy
    */
    m3f3 mult_right(const m3f3& rhs);
    /** copy = lhs x this 
        this = copy
    */
    m3f3 mult_left(const m3f3& lhs);
    /** copy = this * rhs */
    m3f3 operator*(const m3f3& rhs);

    f2 mult_vec(f2 _f2); /** Translate, rotate, and scale f2 using the set m3f3 values. */
    void translate(f2 transl);
    void scale(f2 scale);
    void rotate(float angle);

    float* pointer();
    const float* pointer_const() const;

    void set_to_identity();

    void print();

    m3f3()
        : x { f3(1.0f, 0.0f, 0.0f) }
        , y { f3(0.0f, 1.0f, 0.0f) }
        , z { f3(0.0f, 0.0f, 1.0f) } 
    {};
    m3f3(f3 x, f3 y, f3 z)
        : x { x }
        , y { y }
        , z { z } 
    {}; 
    

    static m3f3
    rotation_x(float angle)
    {
        return {{ 1.0f,  0.0f,         0.0f        },
                { 0.0f,  cosf(angle), -sinf(angle) },
                { 0.0f,  sinf(angle),  cosf(angle) } };
    }

    static m3f3
    rotation_y(float angle)
    {
        return {{  cosf(angle),  0.0f,  sinf(angle) },
                {  0.0f,         1.0f,  0.0f        },
                { -sinf(angle),  0.0f,  cosf(angle) } };
    }

    static m3f3
    rotation_z(float angle)
    {
        return {{ cosf(angle),  sinf(angle),    0.0f  },
                {-sinf(angle),  cosf(angle),    0.0f  },
                { 0.0f,         0.0f,           1.0f  }  };
    }
};

inline f3 operator*(const m3f3& M, const f3& V)
{
    return { M.x.x * V.x + M.x.y * V.y + M.x.z * V.z,
             M.y.x * V.x + M.y.y * V.y + M.y.z * V.z,
             M.z.x * V.x + M.z.y * V.y + M.z.z * V.z    };
}


struct m4f4 
{
    f4 x;
    f4 y;
    f4 z;
    f4 w;

    constexpr
    m4f4() : 
        x { f4(1.0f, 0.0f, 0.0f, 0.0f) }, 
        y { f4(0.0f, 1.0f, 0.0f, 0.0f) }, 
        z { f4(0.0f, 0.0f, 1.0f, 0.0f) }, 
        w { f4(0.0f, 0.0f, 0.0f, 1.0f) } {};

    m4f4(f4 x, f4 y, f4 z, f4 w) : 
        x { x }, 
        y { y }, 
        z { z }, 
        w { w } {};

    m4f4& operator=(const m4f4& rhs);
    /** copy = this * rhs */
    m4f4 operator*(const m4f4& rhs);

    /** this = this X rhs */
    void mult_in_place(const m4f4& rhs);

    void set_to_I();

    float* pointer() const;
    const float * pointer_const() const;

    void print();

    /** Create a pure scale matrix */
    static m4f4 scale(float scale_factor);
    /** Create a pure scale matrix */
    static m4f4 scale(f3 scale);
    /** Create a pure translation matrix */
    static m4f4 translation(f3 transl);
    /** Create a pure rotation matrix about x axis. 
        angle > 0 : +Y -> +Z */
    static m4f4 rotation_x(float angle);
    /** Create a pure rotation matrix about y axis. 
        angle > 0 : +Z -> +X */
    static m4f4 rotation_y(float angle);
    /** Create a pure rotation matrix about z axis. 
        angle > 0 : +X -> +Y */
    static m4f4 rotation_z(float angle);
};