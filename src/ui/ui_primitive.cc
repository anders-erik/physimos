
#include <glad/glad.h>
#include <iostream>

#include "ui_primitive.hh"

#include "ui_globals.hh"

#include "render/shader.hpp"


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


   
    
    
    
    Primitive::Primitive(PrimitiveInfo * _primitiveInfo_ptr) {

        std::cout << "new primitive!!  ! !" << std::endl;


        x = _primitiveInfo_ptr->x;
        y = _primitiveInfo_ptr->y;
        width = _primitiveInfo_ptr->width;
        height = _primitiveInfo_ptr->height;

        R = _primitiveInfo_ptr->R;
        G = _primitiveInfo_ptr->G;
        B = _primitiveInfo_ptr->B;
        A = _primitiveInfo_ptr->A;

        // this->primitiveInfo_ptr = 
        // this->primitiveInfo_ptr->A = _primitiveInfo_ptr->A;


        // this->primitiveInfo_ptr->x = _primitiveInfo_ptr->x;
        // this->primitiveInfo_ptr->y = _primitiveInfo_ptr->y;

        // primitiveInfo_ptr = _primitiveInfo_ptr;

        shader = getShader(Shaders::ui_primitive);

        // Transforms
        // uiViewportTransform16[0] = 2.0f / 1000.0;
        // uiViewportTransform16[5] = 2.0f / 750.0;

        // 
        if (parent == nullptr) {

            uiPrimitiveTransform16[0] = _primitiveInfo_ptr->width;
            uiPrimitiveTransform16[5] = _primitiveInfo_ptr->height;
            uiPrimitiveTransform16[3] = _primitiveInfo_ptr->x;
            uiPrimitiveTransform16[7] = _primitiveInfo_ptr->y;

        }
        else {
            uiPrimitiveTransform16[0] = _primitiveInfo_ptr->width;
            uiPrimitiveTransform16[5] = _primitiveInfo_ptr->height;
            uiPrimitiveTransform16[3] = _primitiveInfo_ptr->x;
            uiPrimitiveTransform16[7] = _primitiveInfo_ptr->y;
        }

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
        
        bool x_between_min_and_max = (_x > (double)x) && (_x < (double)(x + width));
        bool y_between_min_and_max = (_y > (double)y) && (_y < (double)(y + height));

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
