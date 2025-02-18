
#include <glad/glad.h>
#include <iostream>

#include "render/shader.hpp"
#include "res/bmp_loader.hpp"

#include "ui_primitive.hh"
#include "ui_globals.hh"
#include "ui/font.hh"


namespace UI {
    PrimitiveString::PrimitiveString(std::string _str) : str{ _str } {
        initGraphics();
        str_setString(_str);
    }
    void PrimitiveString::str_setFontSize(FontSize _fontSize) {
        str_fontSize = _fontSize;
        str_setString(str);
    }
    void PrimitiveString::str_setString(std::string _str) {

        uiTransform.height = str_fontSize;

        size_t charWidth = 2 * (str_fontSize / 3);

        uiTransform.width = charWidth * _str.length();

        loadStringIntoGlTexture(defaultTexture, _str);

        updateTransformationMatrix();
    }


    void PrimitiveString::update_str(std::string _str){

        if (str != _str){
            str = _str;
            str_setString(_str);
        }

    }
    void PrimitiveString::update_str_int(int _int) {
        std::string _int_str = std::to_string(_int);
        update_str(_int_str);
    }
    void PrimitiveString::update_str_double(double _double) {
        std::string _double_str = std::to_string(_double);
        update_str(_double_str);
    }

    bool Primitive::try_hover_component(double x, double y) {
        return false;
    }


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


    // void Primitive::setUiTransform(::UI::Transform _uiTransform) {
    //     // uiTransform = _uiTransform;

    // }

    void Primitive::setDefaultColor(Color color){

        int imageBufferWidth = 1;
        int imageBufferHeight = 1;
        unsigned char colorBuffer[4];


        glBindTexture(GL_TEXTURE_2D, defaultTexture);


        colorBuffer[0] = color.R;
        colorBuffer[1] = color.G;
        colorBuffer[2] = color.B;
        colorBuffer[3] = color.A;


        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageBufferWidth, imageBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);

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

        updateTransformationMatrix();
    }

    bool Primitive::isTextPrimitive(){
        // return this->text
        bool stringLength = this->text.length();
        
        return stringLength == 0 ? false : true;
    }

    void Primitive::setHeight(int _height){
        uiTransform.height = _height;

        updateTransformationMatrix();
    }
    void Primitive::setWidth(int _width) {
        uiTransform.width = _width;

        updateTransformationMatrix();
    }


    void Primitive::update_x_real_recursive() {

        if (uiTransform.horiRef == HoriRef::Left) {

            if (parent == nullptr) {
                uiTransform.x_real = uiTransform.x_input_px;
            }
            else {
                uiTransform.x_real = parent->uiTransform.x_real + uiTransform.x_input_px;
            }


        }
        else if (uiTransform.horiRef == HoriRef::Right) {

            // The x value when primitives right edge would be flush with parents right edge
            int right_reference_x;

            if (parent == nullptr) {
                right_reference_x = viewport_width - uiTransform.width;
                uiTransform.x_real = right_reference_x - uiTransform.x_input_px;
            }
            else {
                right_reference_x = parent->uiTransform.x_real + parent->uiTransform.width - uiTransform.width;
                uiTransform.x_real = right_reference_x - uiTransform.x_input_px;
            }
        }


        // Reload real x locations
        for (Primitive* child : children)
            child->update_x_real_recursive();
        

        updateTransformationMatrix();
        uiTransform.hasBeenChangedFlag = false;
    }

    void Primitive::update_y_real_recursive() {


        // real y coordinate offset from reference
        // If primitive is a root element, then this is viewport/ui coordinates
        int y_real_offset;

        // Draw root primitives directly
        if (uiTransform.vertRef == VertRef::Bottom) {

            y_real_offset = uiTransform.y_input_px;

            if (parent == nullptr) {
                uiTransform.y_real = uiTransform.y_input_px;
            }
            else {
                uiTransform.y_real = parent->uiTransform.y_real + uiTransform.y_input_px;
            }

        }
        else if (uiTransform.vertRef == VertRef::Top) {

            // The real offset is in the negative direction when using top as vertical reference
            y_real_offset = -uiTransform.y_input_px;

            if (parent == nullptr) {
                int zero_point_top = viewport_height - uiTransform.height;
                // _y_real_offset = zero_point_top - _y_input_px;
                uiTransform.y_real = zero_point_top + y_real_offset;
            }
            else {
                // move child to align with parent top, the subtract top offset (_y_real_offset)
                uiTransform.y_real = parent->uiTransform.y_real + parent->uiTransform.height - this->uiTransform.height + y_real_offset;
            }

        }

        // Reload real y locations
        for (Primitive* child : children)
            child->update_y_real_recursive();

        updateTransformationMatrix();
        uiTransform.hasBeenChangedFlag = false;
    }

    void Primitive::set_x(std::string x_str) {

        char hori_char = x_str[0];
        char unit_char = x_str[x_str.size() - 1];
        unsigned int num_value = atoi(x_str.substr(1, x_str.size() - 2).data());

        uiTransform.x_input = num_value;
        
        if (hori_char == '<')
            uiTransform.horiRef = HoriRef::Left;
        else if (hori_char == '>')
            uiTransform.horiRef = HoriRef::Right;

        if (unit_char == 'x'){
            uiTransform.x_unit = Unit::Pixel;
            uiTransform.x_input_px = uiTransform.x_input;
        }
        else if (unit_char == '%'){
            uiTransform.x_unit = Unit::Percent;
            uiTransform.x_input_px = (viewport_width * uiTransform.x_input) / 100;  // Multiply _first_ to reduce compounding error from integer division rounding 
        }
        
        uiTransform.hasBeenChangedFlag = true;
    }

    void Primitive::set_y(std::string y_str) {

        char vert_char = y_str[0];
        char unit_char = y_str[y_str.size() - 1];
        unsigned int num_value = atoi(y_str.substr(1, y_str.size() - 2).data());

        uiTransform.y_input = num_value;

        if (vert_char == '^')
            uiTransform.vertRef = VertRef::Top;
        else if (vert_char == '_')
            uiTransform.vertRef = VertRef::Bottom;

        if (unit_char == 'x'){
            uiTransform.y_unit = Unit::Pixel;
            uiTransform.y_input_px = uiTransform.y_input;
        }
        else if (unit_char == '%'){
            uiTransform.y_unit = Unit::Percent;
            uiTransform.y_input_px = (viewport_height * uiTransform.y_input) / 100; // Multiply _first_ to reduce compounding error from integer division rounding 
        }

        uiTransform.hasBeenChangedFlag = true;
    }


    // Store x_input and make appropriate conversions to update x_real.
    // x_input is relative to parent. If no parent then relative to ui origin.
    // Recursivity is performed to make sure all x_real values are correct for rendering.
    void Primitive::setXrecursive(int x_input){
        uiTransform.x_input = x_input;
        
        // x_input in pixel-units
        int _x_input_px = x_input;
        // Make sure all calculations are made using pixel units
        if (uiTransform.x_unit == Unit::Percent) {
            // Multiply _first_ to reduce compounding error from integer division rounding 
            _x_input_px = (viewport_width * uiTransform.x_input) / 100;
        }


        if (uiTransform.horiRef == HoriRef::Left) {

            if (parent == nullptr) {
                uiTransform.x_real = _x_input_px;
            }
            else {
                uiTransform.x_real = parent->uiTransform.x_real + _x_input_px;
            }


        }
        else if (uiTransform.horiRef == HoriRef::Right) {

            // The x value when primitives right edge would be flush with parents right edge
            int right_reference_x;

            if (parent == nullptr) {
                right_reference_x = viewport_width - uiTransform.width;
                uiTransform.x_real = right_reference_x - _x_input_px;
            }
            else {
                right_reference_x = parent->uiTransform.x_real + parent->uiTransform.width - uiTransform.width;
                uiTransform.x_real = right_reference_x - _x_input_px;
            }
        }
        

        // Reload real x locations
        for(Primitive* child : children)
            child->setXrecursive(child->uiTransform.x_input);

        updateTransformationMatrix();
    }



    // Store y_input and make appropriate conversions to update y_real
    // y_input is relative to parent. If no parent then relative to ui origin.
    // Recursivity is performed to make sure all y_real values are correct for rendering.
    void Primitive::setYrecursive(int y_input){
        uiTransform.y_input = y_input;
       
        // y input in number of PIXELS
        int _y_input_px = y_input;
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

        }


        // Reload real y locations
        for (Primitive* child : children)
            child->setYrecursive(child->uiTransform.y_input);

        updateTransformationMatrix();
        return;
    }


    // make sure the transform matrix is updated to current height, width, x, and y
    void Primitive::updateTransformationMatrix(){

        uiTransform.uiPrimitiveTransform16[0] = uiTransform.width;
        uiTransform.uiPrimitiveTransform16[5] = uiTransform.height;

        uiTransform.uiPrimitiveTransform16[3] = uiTransform.x_real;
        uiTransform.uiPrimitiveTransform16[7] = uiTransform.y_real;

    }

    void Primitive::initGraphics(){


        shader = getShader(Shaders::ui_primitive);


        updateTransformationMatrix();


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
        childPrimitive->z = this->z + 1;

        // TODO: better automatic detection on new parent?
        childPrimitive->setXrecursive(childPrimitive->uiTransform.x_input);
        childPrimitive->setYrecursive(childPrimitive->uiTransform.y_input);
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

    void Primitive::render_recursive() {
        render();
        // Make sure children are rendered after parent as currently no depth testing or sorting is being done!
        for (Primitive* child : children) {
            child->render_recursive();
        }
    }

    void Primitive::render(){


        if (uiTransform.hasBeenChangedFlag) {
            update_x_real_recursive();
            update_y_real_recursive();
        }

        // std::cout << "RENDERING UI PRIMITIVE" << std::endl;
        glUseProgram(shader->ID);

        // Transform
        shader_setUiPrimitiveUniforms_uniforms(viewportTransform16, uiTransform.uiPrimitiveTransform16);


        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, glTexture);
        // glDrawArrays(GL_TRIANGLES,0, 30);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisable(GL_BLEND);

    }



   
}
