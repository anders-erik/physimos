
#include <iostream>
#include <cmath>

#include "vec3.hh"

namespace pmath {


    float* Vec3::get_fdata() {
        fdata[0] = (float)data[0];
        fdata[1] = (float)data[1];
        fdata[2] = (float)data[2];
        return fdata;
    }



    bool Vec3::operator==(Vec3 b){
        return (data[0] == b.data[0] && \
                data[1] == b.data[1] && \
                data[2] == b.data[2]) ? true : false;
    }

    Vec3& Vec3::operator+=(const Vec3& rhs) {
        this->data[0] += rhs.data[0];
        this->data[1] += rhs.data[1];
        this->data[2] += rhs.data[2];
        return *this;
    }

    Vec3& Vec3::operator-=(const Vec3& rhs) {
        this->data[0] -= rhs.data[0];
        this->data[1] -= rhs.data[1];
        this->data[2] -= rhs.data[2];
        return *this;
    }

    Vec3& Vec3::operator*=(double rhs) {
        this->data[0] *= rhs;
        this->data[1] *= rhs;
        this->data[2] *= rhs;
        return *this;
    }

    
    double Vec3::dot(const Vec3& rhs){
        return data[0] * rhs.data[0] + data[1] * rhs.data[1] + data[2] * rhs.data[2];
    }

    Vec3& Vec3::cross(const Vec3& rhs) {
        // Temp vec3 for intermediary values
        Vec3 cross = Vec3();
        
        cross.data[0] =  data[1] * rhs.data[2] - data[2] * rhs.data[1];
        cross.data[1] = -data[0] * rhs.data[2] + data[2] * rhs.data[0];
        cross.data[2] =  data[0] * rhs.data[1] - data[1] * rhs.data[0];
        
        return this->copy(cross);;
    }

    Vec3 Vec3::toCross(const Vec3& rhs) {
        Vec3 vec3 = Vec3(data[0], data[1], data[2]);
        return vec3.cross(rhs);
    }




    Vec3& Vec3::copy(const Vec3& rhs) {
        data[0] = rhs.data[0];
        data[1] = rhs.data[1];
        data[2] = rhs.data[2];
        return *this;
    }


    void Vec3::prettyPrint() {
        std::cout << "_____________________" << std::endl << std::endl;
        std::cout << "pmath::vec3 :" << std::endl;
        std::cout << "[0] = " << data[0] << std::endl;
        std::cout << "[1] = " << data[1] << std::endl;
        std::cout << "[2] = " << data[2] << std::endl;
        std::cout << "_____________________" << std::endl;
    }


}