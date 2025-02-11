
#include <glad/glad.h>
#include <iostream>

#include "render/shader.hpp"
#include "res/bmp_loader.hpp"

#include "ui_primitive.hh"
#include "ui_globals.hh"
#include "ui/font.hh"


namespace UI {




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

    // Virtual click method.
    // Implmements behavior within current container scope and returns action(s) to be executed globally.
    UI::Action Primitive::click(){
        std::cout << id << " clicked!" << std::endl;
        return UI::Action::None;
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

        this->uiTransform.height = this->fontSize;

        size_t charWidth = 2 * (this->fontSize / 3);

        this->uiTransform.width = charWidth * _str.length();


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
        uiTransform.height = _height;

        reloadHWXY();
    }
    void Primitive::setWidth(int _width) {
        uiTransform.width = _width;

        reloadHWXY();
    }


    // Store x_input and make appropriate conversions to update x_real.
    // x_input is relative to parent. If no parent then relative to ui origin.
    void Primitive::setX(int _x){
        uiTransform.x_input = _x;
        // x_input = _x;
        int _x_input_px = _x; // will change below if unit is set to percent

        // real x coordinate offset. If a root ui primitive then this is viewport/ui coordinates
        int _x_real_offset;



        // Make sure all calculations are made using pixel units
        if (uiTransform.x_unit == Unit::Percent) {
            // Multiply _first_ to reduce compounding error from integer division rounding 
            _x_input_px = (viewport_width * uiTransform.x_input) / 100;
        }


        if (uiTransform.horiRef == HoriRef::Right) {
            // Subtract the input from the far right position
            int zero_point_right = viewport_width - uiTransform.width;
            _x_real_offset = zero_point_right - _x_input_px;
        }
        else {
            _x_real_offset = _x_input_px;
        }


        if (parent == nullptr) {
            uiTransform.x_real = _x_real_offset;
        }
        else {
            uiTransform.x_real = parent->uiTransform.x_real + _x_real_offset;
        }


        reloadHWXY();
    }



    // Store y_input and make appropriate conversions to update y_real
    // y_input is relative to parent. If no parent then relative to ui origin.
    void Primitive::setY(int _y){
        uiTransform.y_input = _y;
       
        // y input in number of PIXELS
        int _y_input_px = _y;
        if (uiTransform.y_unit == Unit::Percent) {
            // Multiply _first_ to reduce compounding error from integer division rounding 
            _y_input_px = (viewport_height * uiTransform.y_input) / 100;
        }

        // real y coordinate offset from reference
        // If primitive is a root element, then this is viewport/ui coordinates
        int _y_real_offset;

        // Draw root primitives directly
        if (uiTransform.vertRef == VertRef::Bottom) {

            _y_real_offset = _y_input_px;

            if (parent == nullptr) {
                uiTransform.y_real = _y_input_px;
            }
            else {
                uiTransform.y_real = parent->uiTransform.y_real + _y_input_px;
            }

            reloadHWXY();
            return;

        }
        else if (uiTransform.vertRef == VertRef::Top) {

            // The real offset is in the negative direction when using top as vertical reference
            _y_real_offset = -_y_input_px;

            if (parent == nullptr) {
                int zero_point_top = viewport_height - uiTransform.height;
                // _y_real_offset = zero_point_top - _y_input_px;
                uiTransform.y_real = zero_point_top + _y_real_offset;
            }
            else {
                // move child to align with parent top, the subtract top offset (_y_real_offset)
                uiTransform.y_real = parent->uiTransform.y_real + parent->uiTransform.height - this->uiTransform.height + _y_real_offset;
            }

            reloadHWXY();
            return;

        }
    }


    // make sure the transform matrix is updated to current height, width, x, and y
    void Primitive::reloadHWXY(){

        uiTransform.uiPrimitiveTransform16[0] = uiTransform.width;
        uiTransform.uiPrimitiveTransform16[5] = uiTransform.height;

        uiTransform.uiPrimitiveTransform16[3] = uiTransform.x_real;
        uiTransform.uiPrimitiveTransform16[7] = uiTransform.y_real;

    }

    void Primitive::initGraphics(){


        shader = getShader(Shaders::ui_primitive);


        reloadHWXY();


        shader_setUiPrimitiveUniforms_uniforms(viewportTransform16, uiTransform.uiPrimitiveTransform16);


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

        colorBuffer[0] = color_default[0];
        colorBuffer[1] = color_default[1];
        colorBuffer[2] = color_default[2];
        colorBuffer[3] = color_default[3];


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

        colorBuffer[0] = color_hover[0];
        colorBuffer[1] = color_hover[1];
        colorBuffer[2] = color_hover[2];
        colorBuffer[3] = color_hover[3];

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

        colorBuffer[0] = color_selected[0];
        colorBuffer[1] = color_selected[1];
        colorBuffer[2] = color_selected[2];
        colorBuffer[3] = color_selected[3];

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
        
        bool x_between_min_and_max = (_x > (double)uiTransform.x_real) && (_x < (double)(uiTransform.x_real + uiTransform.width));
        bool y_between_min_and_max = (_y > (double)uiTransform.y_real) && (_y < (double)(uiTransform.y_real + uiTransform.height));

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
        childPrimitive->setX(childPrimitive->uiTransform.x_input);
        childPrimitive->setY(childPrimitive->uiTransform.y_input);
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
    void Primitive::updateShaderMatrixesRecursively() {
        // std::cout << "UPDATING UI PRIMITIVE" << std::endl;
        for (Primitive* child : children){
            child->updateShaderMatrixesRecursively();
        }

        shader_setUiPrimitiveUniforms_uniforms(viewportTransform16, uiTransform.uiPrimitiveTransform16);

    }


    void Primitive::renderRecursive() {
        // std::cout << "RENDERING UI PRIMITIVE" << std::endl;
        glUseProgram(shader->ID);

        // Transform
        shader_setUiPrimitiveUniforms_uniforms(viewportTransform16, uiTransform.uiPrimitiveTransform16);


        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, glTexture);
        // glDrawArrays(GL_TRIANGLES,0, 30);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_TRIANGLES,0, 6);
        glDisable(GL_BLEND);
        

        // Make sure that children are rendered after parent as depth test is turned of!
        for (Primitive* child : children) {
            child->renderRecursive();
        }
    }




   
}
