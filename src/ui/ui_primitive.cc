
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


    void Primitive::setState(PrimitiveState _newState){
        state = _newState;

        if(_newState == PrimitiveState::Default){
            glTexture = defaultTexture;
        }
        else if (_newState == PrimitiveState::Hover) {
            glTexture = hoverTexture;
        }
        else if (_newState == PrimitiveState::Selected) {
            glTexture = selectedTexture;
        }

    }

    // Default click method
    void Primitive::click(){
        std::cout << id << " clicked!" << std::endl;
        
        // if(clickCallback == nullptr)
        //     return;
        
        // (*clickCallback)();
    }


    // No children primitives
    bool Primitive::isLeaf() {
        return children.size() == 0 ? true : false;
    }

    void Primitive::printId(){
        std::cout << "primitive id = " << id << std::endl;
    }


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

    void Primitive::setHeight(int _height){
        height = _height;

        reloadHWXY();
    }
    void Primitive::setWidth(int _width) {
        width = _width;

        reloadHWXY();
    }


    // Store x_input and make appropriate conversions to update x_real.
    // x_input is relative to parent. If no parent then relative to ui origin.
    void Primitive::setX(int _x){
        x_input = _x;
        int _x_input_px = _x; // will change below if unit is set to percent

        // real x coordinate offset. If a root ui primitive then this is viewport/ui coordinates
        int _x_real_offset;



        // Make sure all calculations are made using pixel units
        if(x_unit == Unit::Percent){
            // Multiply _first_ to reduce compounding error from integer division rounding 
            _x_input_px = (viewport_width * x_input)/100;
        }


        if(horiRef == HoriRef::Right){
            // Subtract the input from the far right position
            int zero_point_right = viewport_width - width;
            _x_real_offset = zero_point_right - _x_input_px;
        }
        else {
            _x_real_offset = _x_input_px;
        }


        if (parent == nullptr) {
            x_real = _x_real_offset;
        }
        else {
            x_real = parent->x_real + _x_real_offset;
        }


        reloadHWXY();
    }



    // Store y_input and make appropriate conversions to update y_real
    // y_input is relative to parent. If no parent then relative to ui origin.
    void Primitive::setY(int _y){
        y_input = _y;
       
        // y input in number of PIXELS
        int _y_input_px = _y;
        if (y_unit == Unit::Percent) {
            // Multiply _first_ to reduce compounding error from integer division rounding 
            _y_input_px = (viewport_height * y_input) / 100;
        }

        // real y coordinate offset from reference
        // If primitive is a root element, then this is viewport/ui coordinates
        int _y_real_offset;

        // Draw root primitives directly
        if (vertRef == VertRef::Bottom) {

            _y_real_offset = _y_input_px;

            if (parent == nullptr) {
                y_real = _y_input_px;
            }
            else {
                y_real = parent->y_real + _y_input_px;
            }

            reloadHWXY();
            return;

        }
        else if (vertRef == VertRef::Top) {

            // The real offset is in the negative direction when using top as vertical reference
            _y_real_offset = -_y_input_px;

            if (parent == nullptr) {
                int zero_point_top = viewport_height - height;
                // _y_real_offset = zero_point_top - _y_input_px;
                y_real = zero_point_top + _y_real_offset;
            }
            else {
                // move child to align with parent top, the subtract top offset (_y_real_offset)
                y_real = parent->y_real + parent->height - this->height + _y_real_offset;
            }

            reloadHWXY();
            return;

        }
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

        uiPrimitiveTransform16[0] = width;
        uiPrimitiveTransform16[5] = height;

        uiPrimitiveTransform16[3] = x_real;
        uiPrimitiveTransform16[7] = y_real;

        // if (parent == nullptr) {
        //     // set pure ui coordinates
        //     uiPrimitiveTransform16[3] = x_real;
        //     uiPrimitiveTransform16[7] = y_real;

        // }
        // else {
        //     // 
        //     uiPrimitiveTransform16[3] = x_real;
        //     uiPrimitiveTransform16[7] = y_real;
        // }

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


        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &colorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color_default);
        glGenerateMipmap(GL_TEXTURE_2D);


        //  HOVER
        glGenTextures(1, &hoverTexture);
        glBindTexture(GL_TEXTURE_2D, hoverTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, hoverTexture);

        colorBuffer[0] = R + 30;
        colorBuffer[1] = G + 30;
        colorBuffer[2] = B + 30;
        colorBuffer[3] = 255;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color_hover);
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


    bool Primitive::childrenContainPoint(double _x, double _y) {

        bool _childrenContainPoint = false;

        for (Primitive* child : children){
            if (child->containsPoint(_x, _y))
                _childrenContainPoint = true;
        }

        return _childrenContainPoint;
    }

    // 
    bool Primitive::containsPoint(double _x, double _y) {
        
        bool x_between_min_and_max = (_x > (double)x_real) && (_x < (double)(x_real + width));
        bool y_between_min_and_max = (_y > (double)y_real) && (_y < (double)(y_real + height));

        if (x_between_min_and_max && y_between_min_and_max) {
            // std::cout << "IN PRIMITIVE\n";
            // glTexture = hoverTexture;
            return true;
        }
        else {
            // make sure default texture is being used
            // if (glTexture != defaultTexture) {
            //     glTexture = defaultTexture;
            // }
            return false;
        }

    }

    // Set both parent and child relations and update where necesary
    void Primitive::appendChild(Primitive* childPrimitive){
        children.push_back(childPrimitive);

        childPrimitive->parent = this;

        // TODO: better automatic detection on new parent?
        childPrimitive->setX(childPrimitive->x_input);
        childPrimitive->setY(childPrimitive->y_input);
    }



    std::vector<Primitive*> tempFlatTree = {};

    std::vector<Primitive*> Primitive::flattenTree(){
        // std::cout << "tempFlatTree.size() = " << tempFlatTree.size() << std::endl;
        
        while(tempFlatTree.size() != 0)
            tempFlatTree.pop_back();
        
        appendtoFlatTreeNested(this);

        return tempFlatTree;
    }

    void Primitive::appendtoFlatTreeNested(Primitive* _primitive){
        tempFlatTree.push_back(_primitive);
        // appendChildrenNested()
        for (Primitive* child : _primitive->children){
            appendtoFlatTreeNested(child);
        }
    }

    // will set the shader transforms for the ui primitive
    void Primitive::updateTransformsRecursive() {
        // std::cout << "UPDATING UI PRIMITIVE" << std::endl;
        for (Primitive* child : children){
            child->updateTransformsRecursive();
        }

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
        

        // Make sure that children are rendered after parent as depth test is turned of!
        for (Primitive* child : children) {
            child->render();
        }
    }




   
}
