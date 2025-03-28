
#include "vecmat.hh"

#include <cmath>
#include <iostream>


void f3::matmul(m4f4 matrix){
    f3 tmp;
    tmp.x = x*matrix.x.x + y*matrix.x.y + z*matrix.x.z;
    tmp.y = x*matrix.y.x + y*matrix.y.y + z*matrix.y.z;
    tmp.z = x*matrix.z.x + y*matrix.z.y + z*matrix.z.z;

    this->x = tmp.x;
    this->y = tmp.y;
    this->z = tmp.z;
}

m4f4 m4f4_create_translation(f3 transl){
    m4f4 matrix;

    matrix.x.w = transl.x;
    matrix.y.w = transl.y;
    matrix.z.w = transl.z;

    return matrix;
}


m4f4 m4f4_create_rotation_x(float angle){
    m4f4 matrix;

    matrix.y.y = cosf(angle);
    matrix.y.z = -sinf(angle);
    matrix.z.y = sinf(angle);
    matrix.z.z = cosf(angle);

    return matrix;
}
m4f4 m4f4_create_rotation_y(float angle){
    m4f4 matrix;

    matrix.x.x = cosf(angle);
    matrix.x.z = sinf(angle);
    matrix.z.x = -sinf(angle);
    matrix.z.z = cosf(angle);

    return matrix;
}
m4f4 m4f4_create_rotation_z(float angle){
    m4f4 matrix;

    matrix.x.x = cosf(angle);
    matrix.x.y = sinf(angle);
    matrix.y.x = -sinf(angle);
    matrix.y.y = cosf(angle);

    return matrix;
}

void mat_mul(m4f4& lmat, m4f4& rmat){

    m4f4 tmp;

    tmp.x.x = lmat.x.x * rmat.x.x + lmat.x.y * rmat.y.x + lmat.x.z * rmat.z.x + lmat.x.w * rmat.w.x; 
    tmp.x.y = lmat.x.x * rmat.x.y + lmat.x.y * rmat.y.y + lmat.x.z * rmat.z.y + lmat.x.w * rmat.w.y; 
    tmp.x.z = lmat.x.x * rmat.x.z + lmat.x.y * rmat.y.z + lmat.x.z * rmat.z.z + lmat.x.w * rmat.w.z; 
    tmp.x.w = lmat.x.x * rmat.x.w + lmat.x.y * rmat.y.w + lmat.x.z * rmat.z.w + lmat.x.w * rmat.w.w; 

    tmp.y.x = lmat.y.x * rmat.x.x + lmat.y.y * rmat.y.x + lmat.y.z * rmat.z.x + lmat.y.w * rmat.w.x; 
    tmp.y.y = lmat.y.x * rmat.x.y + lmat.y.y * rmat.y.y + lmat.y.z * rmat.z.y + lmat.y.w * rmat.w.y; 
    tmp.y.z = lmat.y.x * rmat.x.z + lmat.y.y * rmat.y.z + lmat.y.z * rmat.z.z + lmat.y.w * rmat.w.z; 
    tmp.y.w = lmat.y.x * rmat.x.w + lmat.y.y * rmat.y.w + lmat.y.z * rmat.z.w + lmat.y.w * rmat.w.w; 

    tmp.z.x = lmat.z.x * rmat.x.x + lmat.z.y * rmat.y.x + lmat.z.z * rmat.z.x + lmat.z.w * rmat.w.x; 
    tmp.z.y = lmat.z.x * rmat.x.y + lmat.z.y * rmat.y.y + lmat.z.z * rmat.z.y + lmat.z.w * rmat.w.y; 
    tmp.z.z = lmat.z.x * rmat.x.z + lmat.z.y * rmat.y.z + lmat.z.z * rmat.z.z + lmat.z.w * rmat.w.z; 
    tmp.z.w = lmat.z.x * rmat.x.w + lmat.z.y * rmat.y.w + lmat.z.z * rmat.z.w + lmat.z.w * rmat.w.w; 

    tmp.w.x = lmat.w.x * rmat.x.x + lmat.w.y * rmat.y.x + lmat.w.z * rmat.z.x + lmat.w.w * rmat.w.x; 
    tmp.w.y = lmat.w.x * rmat.x.y + lmat.w.y * rmat.y.y + lmat.w.z * rmat.z.y + lmat.w.w * rmat.w.y; 
    tmp.w.z = lmat.w.x * rmat.x.z + lmat.w.y * rmat.y.z + lmat.w.z * rmat.z.z + lmat.w.w * rmat.w.z; 
    tmp.w.w = lmat.w.x * rmat.x.w + lmat.w.y * rmat.y.w + lmat.w.z * rmat.z.w + lmat.w.w * rmat.w.w; 

    lmat = tmp;

}


void m4f4::translate(f3 transl){

    m4f4 tranls_matrix = m4f4_create_translation(transl);

    mat_mul(*this, tranls_matrix);

}


void m4f4::rotate_x(float angle){
    m4f4 rot_x_matrix = m4f4_create_rotation_x(angle);

    mat_mul(*this, rot_x_matrix);
}
void m4f4::rotate_y(float angle){
    m4f4 rot_y_matrix = m4f4_create_rotation_y(angle);

    mat_mul(*this, rot_y_matrix);
}
void m4f4::rotate_z(float angle){
    m4f4 rot_z_matrix = m4f4_create_rotation_z(angle);

    mat_mul(*this, rot_z_matrix);
}


void m4f4::perspective(float fov, float width, float height, float zn, float zf){
    float aspect = width / height;
    float tanHalfFov = tanf(fov / 2.0f);

    x.x = 1 / (aspect * tanHalfFov);
    y.y = 1 / tanHalfFov;
    z.z = - (zf + zn) / (zf - zn);
    w.w = 0.0f;

    z.w = - (2.0f * zf * zn) / (zf - zn);
    w.z = - 1.0f;

}

m4f4& m4f4::operator=(m4f4& rhs){
    x.x = rhs.x.x;
    x.y = rhs.x.y;
    x.z = rhs.x.z;
    x.w = rhs.x.w;

    y.x = rhs.y.x;
    y.y = rhs.y.y;
    y.z = rhs.y.z;
    y.w = rhs.y.w;

    z.x = rhs.z.x;
    z.y = rhs.z.y;
    z.z = rhs.z.z;
    z.w = rhs.z.w;

    w.x = rhs.w.x;
    w.y = rhs.w.y;
    w.z = rhs.w.z;
    w.w = rhs.w.w;

    return *this;
}

void m4f4::print(){
    std::cout << x.x << " " << x.y << " " << x.z << " " << x.w << std::endl;
    std::cout << y.x << " " << y.y << " " << y.z << " " << y.w << std::endl;
    std::cout << z.x << " " << z.y << " " << z.z << " " << z.w << std::endl;
    std::cout << w.x << " " << w.y << " " << w.z << " " << w.w << std::endl;
}
