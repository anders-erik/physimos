#include <cmath>
#include <iostream>

#include "vecmat.hh"



f2& f2::operator+=(const f2& rhs){
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
}
f2& f2::operator+=(float scalar){
    this->x += scalar;
    this->y += scalar;
    return *this;
}
f2& f2::operator-=(const f2& rhs){
    this->x -= rhs.x;
    this->y -= rhs.y;
    return *this;
}
f2& f2::operator-=(float scalar){
    this->x -= scalar;
    this->y -= scalar;
    return *this;
}
f2& f2::operator*=(const f2& rhs)
{
    this->x *= rhs.x;
    this->y *= rhs.y;
    return *this;
}
f2& f2::operator/=(const f2 & rhs)
{
    this->x /= rhs.x;
    this->y /= rhs.y;
    return *this;
}
f2 f2::operator+(const f2& rhs) const
{
    return {this->x + rhs.x, this->y + rhs.y};
}

f2 f2::
operator-(const f2& rhs) const
{
    return f2 {this->x - rhs.x, this->y - rhs.y};
}

f2 f2::
operator*(const f2 & rhs) const
{
    return f2(this->x * rhs.x, this->y * rhs.y);
}
f2 f2::operator/(const f2 & rhs) const
{
    return f2(this->x / rhs.x, this->y / rhs.y);
}

f2 f2::
operator*(float scalar) const
{
    return f2(this->x * scalar, this->y * scalar);
}

f2 f2::
operator-()
{
    return {-x, -y};
}

float f2::norm()
{
    return sqrtf(x*x + y*y);
}

float f2::angle()
{
    float ang = atanf( y / x );

    if(x < 0.0f)
        ang += 3.141593;

    return ang;
}

bool f2::is_zero()
{
    return (x == 0.0f && y == 0.0f) ? true : false;
}

f2& f2::clamp_nonzero()
{
    if(is_zero())
        x = 0.00001f;
    return *this;
}


i2 f2::to_i2()
{
    return i2{(int)x, (int)y};
}

void f2::print(std::string prefix){
    std::cout << prefix <<  ": x = " << x << ", y = " << y << std::endl;
}


m2f2& m2f2::operator=(m2f2& rhs){
    x = rhs.x;
    y = rhs.y;

    return *this;
}

f2 m2f2::mult_vec(f2 vec){
    f2 _f2;

    _f2.x = x.x * vec.x + x.y * vec.y;
    _f2.y = y.x * vec.x + y.y * vec.y;

    return _f2;
}


void m2f2::set_to_identity(){
    x.x = 1.0f;
    x.y = 0.0f;

    y.x = 0.0f;
    y.y = 1.0f;
}

void m2f2::print(){
    std::cout << x.x << " " << x.y << " " << std::endl;
    std::cout << y.x << " " << y.y << " " << std::endl;
}


m3f3& m3f3::operator=(const m3f3& rhs){
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;

    return *this;
}


m3f3 m3f3::mult_right(const m3f3& rhs){

    m3f3 copy;
    copy = *this;
    
    copy.x.x = x.x * rhs.x.x + x.y * rhs.y.x + x.z * rhs.z.x;
    copy.x.y = x.x * rhs.x.y + x.y * rhs.y.y + x.z * rhs.z.y;
    copy.x.z = x.x * rhs.x.z + x.y * rhs.y.z + x.z * rhs.z.z;

    copy.y.x = y.x * rhs.x.x + y.y * rhs.y.x + y.z * rhs.z.x;
    copy.y.y = y.x * rhs.x.y + y.y * rhs.y.y + y.z * rhs.z.y;
    copy.y.z = y.x * rhs.x.z + y.y * rhs.y.z + y.z * rhs.z.z;

    copy.z.x = z.x * rhs.x.x + z.y * rhs.y.x + z.z * rhs.z.x;
    copy.z.y = z.x * rhs.x.y + z.y * rhs.y.y + z.z * rhs.z.y;
    copy.z.z = z.x * rhs.x.z + z.y * rhs.y.z + z.z * rhs.z.z;

    // *this = copy;
    return copy;
}

m3f3 m3f3::mult_left(const m3f3& lhs){

    m3f3 copy;

    m3f3 rhs = *this;
    
    copy.x.x = lhs.x.x * rhs.x.x + lhs.x.y * rhs.y.x + lhs.x.z * rhs.z.x;
    copy.x.y = lhs.x.x * rhs.x.y + lhs.x.y * rhs.y.y + lhs.x.z * rhs.z.y;
    copy.x.z = lhs.x.x * rhs.x.z + lhs.x.y * rhs.y.z + lhs.x.z * rhs.z.z;

    copy.y.x = lhs.y.x * rhs.x.x + lhs.y.y * rhs.y.x + lhs.y.z * rhs.z.x;
    copy.y.y = lhs.y.x * rhs.x.y + lhs.y.y * rhs.y.y + lhs.y.z * rhs.z.y;
    copy.y.z = lhs.y.x * rhs.x.z + lhs.y.y * rhs.y.z + lhs.y.z * rhs.z.z;

    copy.z.x = lhs.z.x * rhs.x.x + lhs.z.y * rhs.y.x + lhs.z.z * rhs.z.x;
    copy.z.y = lhs.z.x * rhs.x.y + lhs.z.y * rhs.y.y + lhs.z.z * rhs.z.y;
    copy.z.z = lhs.z.x * rhs.x.z + lhs.z.y * rhs.y.z + lhs.z.z * rhs.z.z;

    // *this = copy;
    return copy;
}

m3f3 m3f3::operator*(const m3f3& rhs)
{
    return this->mult_right(rhs);
}

f2 m3f3::mult_vec(f2 _f2){
    f3 _f3 (_f2.x,_f2.y, 1.0f);

    _f3.matmul(*this);

    _f2.x = _f3.x;
    _f2.y = _f3.y;

    return _f2;
}

void m3f3::translate(f2 transl){
    x.z = transl.x;
    y.z = transl.y;
}
void m3f3::scale(f2 scale){
    x.x = scale.x;
    y.y = scale.y;
}
void m3f3::rotate(float angle){

    m3f3 matrix;
    matrix.x.x = cosf(angle);
    matrix.x.y = sinf(angle);
    matrix.y.x = -sinf(angle);
    matrix.y.y = cosf(angle);



    // return matrix;
}

float* m3f3::pointer(){
    return (float*) this;
}

const float* m3f3::pointer_const() const
{
    return (const float*) this;
}

void m3f3::set_to_identity(){
    x.x = 1.0f;
    x.y = 0.0f;
    x.z = 0.0f;

    y.x = 0.0f;
    y.y = 1.0f;
    y.z = 0.0f;

    z.x = 0.0f;
    z.y = 0.0f;
    z.z = 1.0f;
}

void m3f3::print(){
    std::cout << x.x << " " << x.y << " " << x.z << " " << std::endl;
    std::cout << y.x << " " << y.y << " " << y.z << " " << std::endl;
    std::cout << z.x << " " << z.y << " " << z.z << " " << std::endl;
}

f3 f3::operator+(const f3& rhs)
{
    return {    this->x + rhs.x,
                this->y + rhs.y,
                this->z + rhs.z};
}

f3& f3::operator=(const f3 & rhs)
{
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    return *this;
}

f3 f3::operator-()
{
    return { -x, -y, -z };
}

f2 f3::to_xy() const
{
    return f2(x, y);
}

f2 f3::yaw_pitch()
{
    f2 xy = to_xy();

    if(xy.is_zero())
        return {INFINITY, INFINITY};

    float yaw = xy.angle();
    float pitch = atanf( z / xy.norm());

    return {yaw, pitch};
}


float f3::norm()
{
    return sqrtf(x*x + y*y + z*z);
}

void f3::matmul(m4f4 matrix){
    f3 tmp;
    tmp.x = x*matrix.x.x + y*matrix.x.y + z*matrix.x.z;
    tmp.y = x*matrix.y.x + y*matrix.y.y + z*matrix.y.z;
    tmp.z = x*matrix.z.x + y*matrix.z.y + z*matrix.z.z;

    *this = tmp;
}
void f3::matmul(m3f3 matrix){
    f3 tmp;
    tmp.x = matrix.x.x * x  + matrix.x.y * y    + matrix.x.z * z;
    tmp.y = matrix.y.x * x  + matrix.y.y * y    + matrix.y.z * z;
    tmp.z = matrix.z.x * x  + matrix.z.y * y    + matrix.z.z * z;

    *this = tmp;
}

void f3::print(std::string name)
{
    std::cout << name <<  ": x = " << x << ", y = " << y << ", z = " << z << std::endl;
}


float* f4::pointer()
{
    return (float*) this;
}




float * m4f4::pointer() const
{
    return (float*) this;
}
const float * m4f4::pointer_const() const{
    return (float const*) this;
}



m4f4& m4f4::operator=(const m4f4& rhs){
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

m4f4 m4f4::operator*(const m4f4 & rhs)
{
    m4f4& lhs = *this;

    m4f4 copy;
    copy = lhs; // copy current matrix
    
    copy.x.x = lhs.x.x * rhs.x.x + lhs.x.y * rhs.y.x + lhs.x.z * rhs.z.x + lhs.x.w * rhs.w.x; 
    copy.x.y = lhs.x.x * rhs.x.y + lhs.x.y * rhs.y.y + lhs.x.z * rhs.z.y + lhs.x.w * rhs.w.y; 
    copy.x.z = lhs.x.x * rhs.x.z + lhs.x.y * rhs.y.z + lhs.x.z * rhs.z.z + lhs.x.w * rhs.w.z; 
    copy.x.w = lhs.x.x * rhs.x.w + lhs.x.y * rhs.y.w + lhs.x.z * rhs.z.w + lhs.x.w * rhs.w.w; 

    copy.y.x = lhs.y.x * rhs.x.x + lhs.y.y * rhs.y.x + lhs.y.z * rhs.z.x + lhs.y.w * rhs.w.x; 
    copy.y.y = lhs.y.x * rhs.x.y + lhs.y.y * rhs.y.y + lhs.y.z * rhs.z.y + lhs.y.w * rhs.w.y; 
    copy.y.z = lhs.y.x * rhs.x.z + lhs.y.y * rhs.y.z + lhs.y.z * rhs.z.z + lhs.y.w * rhs.w.z; 
    copy.y.w = lhs.y.x * rhs.x.w + lhs.y.y * rhs.y.w + lhs.y.z * rhs.z.w + lhs.y.w * rhs.w.w; 

    copy.z.x = lhs.z.x * rhs.x.x + lhs.z.y * rhs.y.x + lhs.z.z * rhs.z.x + lhs.z.w * rhs.w.x; 
    copy.z.y = lhs.z.x * rhs.x.y + lhs.z.y * rhs.y.y + lhs.z.z * rhs.z.y + lhs.z.w * rhs.w.y; 
    copy.z.z = lhs.z.x * rhs.x.z + lhs.z.y * rhs.y.z + lhs.z.z * rhs.z.z + lhs.z.w * rhs.w.z; 
    copy.z.w = lhs.z.x * rhs.x.w + lhs.z.y * rhs.y.w + lhs.z.z * rhs.z.w + lhs.z.w * rhs.w.w; 

    copy.w.x = lhs.w.x * rhs.x.x + lhs.w.y * rhs.y.x + lhs.w.z * rhs.z.x + lhs.w.w * rhs.w.x; 
    copy.w.y = lhs.w.x * rhs.x.y + lhs.w.y * rhs.y.y + lhs.w.z * rhs.z.y + lhs.w.w * rhs.w.y; 
    copy.w.z = lhs.w.x * rhs.x.z + lhs.w.y * rhs.y.z + lhs.w.z * rhs.z.z + lhs.w.w * rhs.w.z; 
    copy.w.w = lhs.w.x * rhs.x.w + lhs.w.y * rhs.y.w + lhs.w.z * rhs.z.w + lhs.w.w * rhs.w.w; 

    return copy;
}


void m4f4::mult_in_place(const m4f4 & rhs)
{
    m4f4& lhs = *this;

    m4f4 tmp;

    tmp.x.x = lhs.x.x * rhs.x.x + lhs.x.y * rhs.y.x + lhs.x.z * rhs.z.x + lhs.x.w * rhs.w.x; 
    tmp.x.y = lhs.x.x * rhs.x.y + lhs.x.y * rhs.y.y + lhs.x.z * rhs.z.y + lhs.x.w * rhs.w.y; 
    tmp.x.z = lhs.x.x * rhs.x.z + lhs.x.y * rhs.y.z + lhs.x.z * rhs.z.z + lhs.x.w * rhs.w.z; 
    tmp.x.w = lhs.x.x * rhs.x.w + lhs.x.y * rhs.y.w + lhs.x.z * rhs.z.w + lhs.x.w * rhs.w.w; 

    tmp.y.x = lhs.y.x * rhs.x.x + lhs.y.y * rhs.y.x + lhs.y.z * rhs.z.x + lhs.y.w * rhs.w.x; 
    tmp.y.y = lhs.y.x * rhs.x.y + lhs.y.y * rhs.y.y + lhs.y.z * rhs.z.y + lhs.y.w * rhs.w.y; 
    tmp.y.z = lhs.y.x * rhs.x.z + lhs.y.y * rhs.y.z + lhs.y.z * rhs.z.z + lhs.y.w * rhs.w.z; 
    tmp.y.w = lhs.y.x * rhs.x.w + lhs.y.y * rhs.y.w + lhs.y.z * rhs.z.w + lhs.y.w * rhs.w.w; 

    tmp.z.x = lhs.z.x * rhs.x.x + lhs.z.y * rhs.y.x + lhs.z.z * rhs.z.x + lhs.z.w * rhs.w.x; 
    tmp.z.y = lhs.z.x * rhs.x.y + lhs.z.y * rhs.y.y + lhs.z.z * rhs.z.y + lhs.z.w * rhs.w.y; 
    tmp.z.z = lhs.z.x * rhs.x.z + lhs.z.y * rhs.y.z + lhs.z.z * rhs.z.z + lhs.z.w * rhs.w.z; 
    tmp.z.w = lhs.z.x * rhs.x.w + lhs.z.y * rhs.y.w + lhs.z.z * rhs.z.w + lhs.z.w * rhs.w.w; 

    tmp.w.x = lhs.w.x * rhs.x.x + lhs.w.y * rhs.y.x + lhs.w.z * rhs.z.x + lhs.w.w * rhs.w.x; 
    tmp.w.y = lhs.w.x * rhs.x.y + lhs.w.y * rhs.y.y + lhs.w.z * rhs.z.y + lhs.w.w * rhs.w.y; 
    tmp.w.z = lhs.w.x * rhs.x.z + lhs.w.y * rhs.y.z + lhs.w.z * rhs.z.z + lhs.w.w * rhs.w.z; 
    tmp.w.w = lhs.w.x * rhs.x.w + lhs.w.y * rhs.y.w + lhs.w.z * rhs.z.w + lhs.w.w * rhs.w.w; 

    lhs = tmp;

}

void m4f4::set_to_I()
{
    x = {1.0f, 0.0f, 0.0f, 0.0f};
    y = {0.0f, 1.0f, 0.0f, 0.0f};
    z = {0.0f, 0.0f, 1.0f, 0.0f};
    w = {0.0f, 0.0f, 0.0f, 1.0f};
}

void m4f4::print(){
    std::cout << x.x << " " << x.y << " " << x.z << " " << x.w << std::endl;
    std::cout << y.x << " " << y.y << " " << y.z << " " << y.w << std::endl;
    std::cout << z.x << " " << z.y << " " << z.z << " " << z.w << std::endl;
    std::cout << w.x << " " << w.y << " " << w.z << " " << w.w << std::endl;
}



m4f4 m4f4::
translation(f3 transl)
{
    return {{1, 0, 0, transl.x},
            {0, 1, 0, transl.y},
            {0, 0, 1, transl.z},
            {0, 0, 0, 1       }};
}


m4f4 m4f4::
rotation_x(float angle)
{
    return {{1, 0,           0,             0},
            {0, cosf(angle), -sinf(angle),  0},
            {0, sinf(angle), cosf(angle),   0},
            {0, 0,           0,             1}};
}


m4f4 m4f4::
rotation_y(float angle)
{
    return {{cosf(angle),   0,  sinf(angle),    0},
            {0,             1,  0,              0},
            {-sinf(angle),  0,  cosf(angle),    0},
            {0,             0,  0,              1}};
}


m4f4 m4f4::
rotation_z(float angle)
{
    return {{cosf(angle),  sinf(angle),    0,  0},
            {-sinf(angle),   cosf(angle),    0,  0},
            {0,             0,              1,  0},
            {0,             0,              0,  1}};
}


i2 i2::operator/(const i2 & rhs)
{
    return {this->x / rhs.x, this->y / rhs.y};
}
