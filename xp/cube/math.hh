
#pragma once

namespace xpcube {


struct f3 {
    float x;
    float y;
    float z;


    f3(float x, float y, float z) : x {z}, y {y}, z {z} {}; 
    f3(float xyz) : x {xyz}, y {xyz}, z {xyz} {}; 
    f3() : x {0.0f}, y {0.0f}, z {0.0f} {}; 
};


}