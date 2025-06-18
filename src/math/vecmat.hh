
#pragma once

#include <string>

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
    ui2(uint xy) : x {xy}, y {xy} {}; 
    ui2() = default;

};

struct i2 {
    int x = 0;
    int y = 0;

    i2(int x, int y) : x {x}, y {y} {}; 
    i2(int xy) : x {xy}, y {xy} {}; 
    i2() = default;

    i2 operator/(const i2& rhs);
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
    constexpr f2(float x, float y) : x {x}, y {y} {}; 
    f2(float xyz) : x {xyz}, y {xyz} {}; 

    f2(double x, double y) : x { (float)x}, y {(float)y} {}; 
    f2(int x, int y) : x { (float)x}, y {(float)y} {}; 

    f2& operator+=(const f2& rhs);
    f2& operator+=(float scalar);
    f2& operator-=(const f2& rhs);
    f2& operator*=(const f2& rhs);
    f2& operator/=(const f2& rhs);
    f2& operator-=(float scalar);
    // f2&& operator+(const f2& rhs);
    f2 operator+(const f2& rhs);
    f2 operator-(const f2& rhs) const;
    f2 operator*(float scalar);
    f2 operator*(const f2& rhs) const;
    f2 operator/(const f2& rhs) const;
    f2 operator-();

    i2 to_i2();

    void print(std::string prefix);
};

struct f3 
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    f3() = default;
    constexpr f3(float x, float y, float z) : x {x}, y {y}, z {z} {}; 
    f3(float xyz) : x {xyz}, y {xyz}, z {xyz} {}; 
    // f3() : x {0.0f}, y {0.0f}, z {0.0f} {}; 

    f3& operator=(const f3& rhs);

    void matmul(m4f4 matrix);
    void matmul(m3f3 matrix);
};


struct f4 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;


    f4(float x, float y, float z, float w) : x {x}, y {y}, z {z}, w {w} {}; 
    f4(float x, float y, float z) : x {x}, y {y}, z {z}, w {1.0f} {}; 
    f4(float xyz) : x {xyz}, y {xyz}, z {xyz}, w {1.0f} {}; 
    f4(f3 _f3) : x {_f3.x}, y {_f3.y}, z {_f3.z}, w {1.0f} {}; 
    // f4() : x {0.0f}, y {0.0f}, z {0.0f}, w {1.0f} {}; 
    f4() = default;

    float* pointer();
};

struct m2f2 {
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

struct m3f3 {
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

    m3f3() : 
        x { f3(1.0f, 0.0f, 0.0f) }, 
        y { f3(0.0f, 1.0f, 0.0f) }, 
        z { f3(0.0f, 0.0f, 1.0f) } {};
};

struct m4f4 {
    f4 x;
    f4 y;
    f4 z;
    f4 w;

    m4f4& operator=(const m4f4& rhs);

    void translate(f3 transl);
    void rotate_x(float angle);
    void rotate_y(float angle);
    void rotate_z(float angle);

    float* pointer();
    const float * pointer_const() const;

    void perspective(float fov, float width, float height, float zn, float zf);

    void print();

    m4f4() : 
        x { f4(1.0f, 0.0f, 0.0f, 0.0f) }, 
        y { f4(0.0f, 1.0f, 0.0f, 0.0f) }, 
        z { f4(0.0f, 0.0f, 1.0f, 0.0f) }, 
        w { f4(0.0f, 0.0f, 0.0f, 1.0f) } {};
    
};

// Create
m4f4 m4f4_create_translation(f3 transl);
m4f4 m4f4_create_rotation_x(float angle);
m4f4 m4f4_create_rotation_y(float angle);
m4f4 m4f4_create_rotation_z(float angle);
// m4f4 rotation_mat(float angle, Axis axis);

// Operations
void mat_mul(m4f4& lmat, m4f4& rmat);

