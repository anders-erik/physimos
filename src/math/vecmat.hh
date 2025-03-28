
#pragma once


struct m4f4;

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

struct i2 {
    int x = 0;
    int y = 0;

    i2(int x, int y) : x {x}, y {y} {}; 
    i2(int xy) : x {xy}, y {xy} {}; 
    i2() = default;
};

struct i3 {
    int x = 0;
    int y = 0;
    int z = 0;

    i3(int x, int y, int z) : x {x}, y {y}, z {z} {}; 
    i3(int xyz) : x {xyz}, y {xyz}, z {xyz} {}; 
    i3() = default;
};

struct f2 {
    float x = 0.0f;
    float y = 0.0f;

    f2(float x, float y) : x {x}, y {y} {}; 
    f2(float xyz) : x {xyz}, y {xyz} {}; 
    f2() = default;
};

struct f3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    void matmul(m4f4 matrix);

    f3(float x, float y, float z) : x {x}, y {y}, z {z} {}; 
    f3(float xyz) : x {xyz}, y {xyz}, z {xyz} {}; 
    // f3() : x {0.0f}, y {0.0f}, z {0.0f} {}; 
    f3() = default;
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
};


struct m4f4 {
    f4 x;
    f4 y;
    f4 z;
    f4 w;

    m4f4& operator=(m4f4& rhs);

    void translate(f3 transl);
    void rotate_x(float angle);
    void rotate_y(float angle);
    void rotate_z(float angle);

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

