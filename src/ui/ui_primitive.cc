
#include <glad/glad.h>
#include <iostream>

#include "render/shader.hpp"
#include "res/bmp_loader.hpp"

#include "ui_primitive.hh"
#include "ui_globals.hh"
#include "ui/font.hh"


namespace UI {

    // float uiPrimitiveTransform16[16] = {
    //                             1, 0, 0, 100.0 / 1000.0,
    //                             0, 1, 0, 100.0 / 750.0,
    //                             0, 0, 1, 0,
    //                             0, 0, 0, 1,
    // };

    // float squareVertices[] = {
    //    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
    //    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top-right
    //    0.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // top-left
    //    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top-right
    //    0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom-left
    //    1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
    // };
    // float identity16[] = {
    //    1.0f, 0.0f, 0.0f, 0.0f,
    //    0.0f, 1.0f, 0.0f, 0.0f,
    //    0.0f, 0.0f, 1.0f, 0.0f,
    //    0.0f, 0.0f, 0.0f, 1.0f,
    // };



    // will load a texture contining the passed string into the default gl texture
    void Primitive::setString(std::string _str) {

        this->height = this->fontSize;

        size_t charWidth = 2 * (this->fontSize / 3);

        this->width = charWidth * _str.length();


        // loadCharIntoGlTexture(defaultTexture, _str[0]);
        loadStringIntoGlTexture(defaultTexture, _str);
        // loadStringIntoGlTexture(glTexture, _str);

        reloadHWXY();
    }

    bool Primitive::isTextPrimitive(){
        // return this->text
        bool stringLength = this->text.length();
        
        return stringLength == 0 ? false : true;
    }

    

    // Store x_input and make appropriate conversions to update x_real
    void Primitive::setX(int _x){
        x_input = _x;
        int _x_input_px = _x; // will change below if unit is set to percent

        // Make sure all calculations are made using pixel units
        if(x_unit == Unit::Percent){
            // Multiply _first_ to reduce compounding error from integer division rounding 
            _x_input_px = (viewport_width * x_input)/100;
        }


        if(horiRef == HoriRef::Right){
            // Subtract the input from the far right position
            int zero_point_right = viewport_width - width;
            x_real = zero_point_right - _x_input_px;
        }
        else {
            x_real = _x_input_px;
        }

        reloadHWXY();
    }
    // Store y_input and make appropriate conversions to update y_real
    void Primitive::setY(int _y){
        y_input = _y;
       
        int _y_input_px = _y; // will change below if unit is set to percent

        // Make sure all calculations are made using pixel units
        if (y_unit == Unit::Percent) {
            // Multiply _first_ to reduce compounding error from integer division rounding 
            _y_input_px = (viewport_height * y_input) / 100;
        }


        if (vertRef == VertRef::Top) {
            // Subtract the input from the far right position
            int zero_point_top = viewport_height - height;
            y_real = zero_point_top - _y_input_px;
        }
        else {
            y_real = _y_input_px;
        }

        reloadHWXY();
    }

    // Empty primitive 
    // Primitive::Primitive() {
    // }
    
    
    // Create Primitive element using a pre-populated primitiveInfo data object. 
    Primitive::Primitive(PrimitiveInfo * _primitiveInfo_ptr) {

        std::cout << "new primitive!!  ! !" << std::endl;

        x_real = _primitiveInfo_ptr->x;
        y_real = _primitiveInfo_ptr->y;
        width = _primitiveInfo_ptr->width;
        height = _primitiveInfo_ptr->height;

        R = _primitiveInfo_ptr->R;
        G = _primitiveInfo_ptr->G;
        B = _primitiveInfo_ptr->B;
        A = _primitiveInfo_ptr->A;

        parent = _primitiveInfo_ptr->parent;

        initGraphics();
    }

    // make sure the transform matrix is updated to current height, width, x, and y
    void Primitive::reloadHWXY(){

        if (parent == nullptr) {

            uiPrimitiveTransform16[0] = width;
            uiPrimitiveTransform16[5] = height;
            uiPrimitiveTransform16[3] = x_real;
            uiPrimitiveTransform16[7] = y_real;

        }
        else {
            uiPrimitiveTransform16[0] = width;
            uiPrimitiveTransform16[5] = height;
            uiPrimitiveTransform16[3] = x_real;
            uiPrimitiveTransform16[7] = y_real;
        }

    }

    void Primitive::initGraphics(){


        shader = getShader(Shaders::ui_primitive);

        // Transforms
        // uiViewportTransform16[0] = 2.0f / 1000.0;
        // uiViewportTransform16[5] = 2.0f / 750.0;


        reloadHWXY();


        shader_setUiPrimitiveUniforms_uniforms(viewportTransform16, uiPrimitiveTransform16);


        // SET VAO, VBO
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

        glBindVertexArray(vao);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);





        generateTextures();

        // Set current texture
        glTexture = defaultTexture;

    }


    /**
     * Binds the primitive's glTexture and generates a new color buffer for it.
     */
    void Primitive::generateTextures(){


        int imageBufferWidth = 1;
        int imageBufferHeight = 1;
        unsigned char colorBuffer[4]; // = { 255, 255, 255, 255 };


        //  DEFAULT
        glGenTextures(1, &defaultTexture);
        glBindTexture(GL_TEXTURE_2D, defaultTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, defaultTexture);

        colorBuffer[0] = R;
        colorBuffer[1] = G;
        colorBuffer[2] = B;
        colorBuffer[3] = A;


        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);


        //  HOVER
        glGenTextures(1, &hoverTexture);
        glBindTexture(GL_TEXTURE_2D, hoverTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, hoverTexture);

        colorBuffer[0] = R;
        colorBuffer[1] = G;
        colorBuffer[2] = B;
        colorBuffer[3] = 255;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);


        //  SELECTED
        glGenTextures(1, &selectedTexture);
        glBindTexture(GL_TEXTURE_2D, selectedTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, selectedTexture);

        colorBuffer[0] = R;
        colorBuffer[1] = G;
        colorBuffer[2] = B;
        colorBuffer[3] = A;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);


        
    }



    bool Primitive::containsPoint(double _x, double _y) {
        
        bool x_between_min_and_max = (_x > (double)x_real) && (_x < (double)(x_real + width));
        bool y_between_min_and_max = (_y > (double)y_real) && (_y < (double)(y_real + height));

        if (x_between_min_and_max && y_between_min_and_max) {
            // std::cout << "IN PRIMITIVE\n";
            glTexture = hoverTexture;
            return true;
        }
        else {
            // make sure default texture is being used
            if (glTexture != defaultTexture) {
                glTexture = defaultTexture;
            }
            return false;
        }

    }


    
    void Primitive::update() {
        // std::cout << "UPDATING UI PRIMITIVE" << std::endl;

        shader_setUiPrimitiveUniforms_uniforms(viewportTransform16, uiPrimitiveTransform16);

    }


    void Primitive::render() {
        // std::cout << "RENDERING UI PRIMITIVE" << std::endl;
        glUseProgram(shader->ID);

        // Transform
        shader_setUiPrimitiveUniforms_uniforms(viewportTransform16, uiPrimitiveTransform16);


        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, glTexture);
        // glDrawArrays(GL_TRIANGLES,0, 30);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_TRIANGLES,0, 6);
        glDisable(GL_BLEND);
        
    }




   
}
