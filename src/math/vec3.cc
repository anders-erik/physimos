
#include <iostream>
#include <cmath>

#include "vec3.hh"

namespace pmath {

    bool Vec3::operator==(Vec3 b){
        return (data[0] == b.data[0]) ? true : false;
    }


    float* Vec3::get_fdata() {
        fdata[0] = (float) data[0];
        fdata[1] = (float) data[1];
        fdata[2] = (float) data[2];
        return fdata;
    }



}