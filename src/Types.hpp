
#ifndef TYPES_H
#define TYPES_H


typedef unsigned int uint;

typedef struct Size_UINT_2D {
    int w;
    int h;
} Size_UINT_2D;

/** Holds the real dimensions in Pixels */
typedef struct AABB_INT_2D {
    int w;
    int h;
    int x;
    int y;

    bool contains(double x, double y);
} AABB_INT_2D;

// OLD BELOW

#define pso_white   { 1.0f, 1.0f, 1.0f }
#define pso_black   { 0.0f, 0.0f, 0.0f }
#define pso_red     { 1.0f, 0.0f, 0.0f }
#define pso_green   { 0.0f, 1.0f, 0.0f }
#define pso_blue    { 0.0f, 0.0f, 1.0f }

struct Vec3 {
    float x;
	float y;
	float z;
};

struct Vec4 {
    float x;
    float y;
    float z;
    float w;
};


struct Point3 : Vec3 {
};

struct EulerAnglesRad {
    float a;
    float b;
    float c;
};

#endif /* TYPES_H */