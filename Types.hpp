
#ifndef TYPES_H
#define TYPES_H

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