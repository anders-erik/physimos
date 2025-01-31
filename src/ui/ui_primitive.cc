
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




        //  TEXTURE

        // RANDOM TEXTURE
        glGenTextures(1, &glTexture);
        glBindTexture(GL_TEXTURE_2D, glTexture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // bmp_loader_loadBMPFile("media/mountain.bmp");
        // bmp_loader.prettyPrint();

        // RANDOM DATA GENERATION
        // Old Black and white generated texture
        // Generate a black and white test 'image'
        int blackWhiteWidth = 1;
        int blackWhiteHeight = 1;
        // Generate black and white texture : 3 * blackWhiteWidth * blackWhiteHeight
        unsigned char blackWhiteImageBuffer[4]; // = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, };
        blackWhiteImageBuffer[0] = _primitiveInfo_ptr->R;
        blackWhiteImageBuffer[1] = _primitiveInfo_ptr->G;
        blackWhiteImageBuffer[2] = _primitiveInfo_ptr->B;
        blackWhiteImageBuffer[3] = _primitiveInfo_ptr->A;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, blackWhiteWidth, blackWhiteHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &blackWhiteImageBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);

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
