
#include <glad/glad.h>
#include <iostream>

#include "render/shader.hpp"
#include "res/bmp_loader.hpp"

#include "ui_primitive.hh"
#include "ui_globals.hh"
#include "ui/ui_texture.hh"
#include "ui/font.hh"


namespace UI {

    PrimitiveString::PrimitiveString(std::string _str) : str{ _str } {
        primitiveType = PrimitiveType::String;

        str_setString(_str);
    }
    void PrimitiveString::str_setFontSize(FontSize _fontSize) {
        str_fontSize = _fontSize;
        str_setString(str);
    }
    void PrimitiveString::str_setString(std::string _str) {

        set_h(std::to_string(str_fontSize) + "x");

        size_t charWidth = 2 * (str_fontSize / 3);

        set_w(std::to_string(charWidth * _str.length()) + "x");

        texture::new_texture(privateStringTexture);
        loadStringIntoGlTexture(privateStringTexture, _str);
        renderedTexture = privateStringTexture;
        defaultTexture = privateStringTexture;
        hoverTexture = privateStringTexture;
        selectedTexture = privateStringTexture;

        updateTransformationMatrix();
    }


    void PrimitiveString::update_str(std::string _str) {

        if (str != _str) {
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

    UiResult Primitive::try_find_target_component(double x, double y) {
        return UiResult();
    }
    UiResult Primitive::click() {
        return UiResult();
    }
    UiResult Primitive::hover_enter() {
        return UiResult();
    }
    UiResult Primitive::hover_exit() {
        return UiResult();
    }
    
    /** Is called on pointer change when ui is tracking current primitive as grabbed. */
    UiResult Primitive::grabbed(double dx, double dy) {
        return UiResult();
    }
    UiResult Primitive::scroll(double y_change) {
        return UiResult();
    }


    void Primitive::set_state(PrimitiveState _newState) {
        state = _newState;

        if (_newState == PrimitiveState::Default)
            renderedTexture = defaultTexture;
        else if (_newState == PrimitiveState::Hover)
            renderedTexture = hoverTexture;
        else if (_newState == PrimitiveState::Selected) 
            renderedTexture = selectedTexture;

    }



    void Primitive::set_h(std::string h_str) {
        char unit_char = h_str[h_str.size() - 1];
        unsigned int num_value = atoi(h_str.substr(0, h_str.size() - 1).data());

        uiTransform.h_input = num_value;

        if (unit_char == 'x')
            uiTransform.h_unit = Unit::Pixel;
        else if (unit_char == '%') 
            uiTransform.h_unit = Unit::Percent;

        uiTransform.size_has_been_changed = true;

    }
    void Primitive::set_w(std::string w_str) {

        char unit_char = w_str[w_str.size() - 1];
        unsigned int num_value = atoi(w_str.substr(0, w_str.size() - 1).data());

        uiTransform.w_input = num_value;

        if (unit_char == 'x')
            uiTransform.w_unit = Unit::Pixel;
        else if (unit_char == '%') 
            uiTransform.w_unit = Unit::Percent;

        uiTransform.size_has_been_changed = true;
    }

    void Primitive::update_h_real_recursive() {
        if (parent == nullptr) {
            uiTransform.h_real = uiTransform.h_input;
        }
        else if (uiTransform.h_unit == Unit::Pixel){
            uiTransform.h_real = uiTransform.h_input;
        }
        else if (uiTransform.h_unit == Unit::Percent) {
            uiTransform.h_real = (parent->uiTransform.h_real * uiTransform.h_input) / 100;
        }

        // Reload real x locations
        for (Primitive* child : children)
            child->update_h_real_recursive();


        updateTransformationMatrix();
        // Maybe move to caller of method to prevent every descendant trying to reset when only the triggering primitive should have its flag reset
        uiTransform.size_has_been_changed = false;
    }

    void Primitive::update_w_real_recursive() {
        if (parent == nullptr) {
            uiTransform.w_real = uiTransform.w_input;
        }
        else if (uiTransform.h_unit == Unit::Pixel) {
            uiTransform.w_real = uiTransform.w_input;
        }
        else if (uiTransform.h_unit == Unit::Percent) {
            uiTransform.w_real = (parent->uiTransform.w_real * uiTransform.w_input) / 100;
        }

        // Reload real x locations
        for (Primitive* child : children)
            child->update_w_real_recursive();


        updateTransformationMatrix();
        // Maybe move to caller of method to prevent every descendant trying to reset when only the triggering primitive should have its flag reset
        uiTransform.size_has_been_changed = false;
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
                right_reference_x = viewport_width - uiTransform.w_real;
                uiTransform.x_real = right_reference_x - uiTransform.x_input_px;
            }
            else {
                right_reference_x = parent->uiTransform.x_real + parent->uiTransform.w_real - uiTransform.w_real;
                uiTransform.x_real = right_reference_x - uiTransform.x_input_px;
            }
        }


        // Reload real x locations
        for (Primitive* child : children)
            child->update_x_real_recursive();


        updateTransformationMatrix();
        // uiTransform.x_has_been_changed = false;
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
                int zero_point_top = viewport_height - uiTransform.h_real;
                // _y_real_offset = zero_point_top - _y_input_px;
                uiTransform.y_real = zero_point_top + y_real_offset;
            }
            else {
                // move child to align with parent top, the subtract top offset (_y_real_offset)
                uiTransform.y_real = parent->uiTransform.y_real + parent->uiTransform.h_real - this->uiTransform.h_real + y_real_offset;
            }

        }

        // Reload real y locations
        for (Primitive* child : children)
            child->update_y_real_recursive();

        updateTransformationMatrix();
        // uiTransform.y_has_been_changed = false;
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

        if (unit_char == 'x') {
            uiTransform.x_unit = Unit::Pixel;
            uiTransform.x_input_px = uiTransform.x_input;
        }
        else if (unit_char == '%') {
            uiTransform.x_unit = Unit::Percent;
            uiTransform.x_input_px = (viewport_width * uiTransform.x_input) / 100;  // Multiply _first_ to reduce compounding error from integer division rounding 
        }

        uiTransform.x_has_been_changed = true;
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

        if (unit_char == 'x') {
            uiTransform.y_unit = Unit::Pixel;
            uiTransform.y_input_px = uiTransform.y_input;
        }
        else if (unit_char == '%') {
            uiTransform.y_unit = Unit::Percent;
            uiTransform.y_input_px = (viewport_height * uiTransform.y_input) / 100; // Multiply _first_ to reduce compounding error from integer division rounding 
        }

        uiTransform.y_has_been_changed = true;
    }



    // make sure the transform matrix is updated to current height, width, x, and y
    void Primitive::updateTransformationMatrix() {

        uiTransform.uiPrimitiveTransform16[0] = uiTransform.w_real;
        uiTransform.uiPrimitiveTransform16[5] = uiTransform.h_real;

        uiTransform.uiPrimitiveTransform16[3] = uiTransform.x_real;
        uiTransform.uiPrimitiveTransform16[7] = uiTransform.y_real;

    }


    void Primitive::set_color(Colors _color) {
        // color = _color;
        defaultTexture = UI::texture::get_static_color_texture(_color);
        if(state == PrimitiveState::Default)
            renderedTexture = defaultTexture;
    }
    void Primitive::set_color_hover(Colors _colorHover) {
        // colorHover = _color;
        hoverTexture = UI::texture::get_static_color_texture(_colorHover);
        if (state == PrimitiveState::Hover)
            renderedTexture = defaultTexture;
    }
    void Primitive::set_color_active(Colors _colorActive) {
        selectedTexture = UI::texture::get_static_color_texture(_colorActive);
        if (state == PrimitiveState::Selected)
            renderedTexture = selectedTexture;
    }



    Primitive::Primitive() {
        
        texture_shader = &shader::texture_shader;

        defaultTexture = texture::get_static_color_texture(Colors::LightGray);
        hoverTexture = texture::get_static_color_texture(Colors::Gray);
        selectedTexture = texture::get_static_color_texture(Colors::DarkGray);

        renderedTexture = defaultTexture;
    }


    // 
    bool Primitive::containsPoint(double _x, double _y) {

        bool x_between_min_and_max = (_x > (double)uiTransform.x_real) && (_x < (double)(uiTransform.x_real + uiTransform.w_real));
        bool y_between_min_and_max = (_y > (double)uiTransform.y_real) && (_y < (double)(uiTransform.y_real + uiTransform.h_real));

        if (x_between_min_and_max && y_between_min_and_max) {
            return true;
        }
        else {
            return false;
        }

    }

    // Set both parent and child relations and update where necesary
    void Primitive::appendChild(Primitive* childPrimitive) {
        children.push_back(childPrimitive);

        childPrimitive->parent = this;
        childPrimitive->z = this->z + 1;

        childPrimitive->update_x_real_recursive();
        childPrimitive->update_y_real_recursive();
    }



    void Primitive::render_recursive() {
        render();
        // Make sure children are rendered after parent as currently no depth testing or sorting is being done!
        for (Primitive* child : children) {
            child->render_recursive();
        }
    }

    void Primitive::render() {

        if (uiTransform.size_has_been_changed) {
            update_h_real_recursive();
            update_w_real_recursive();
            uiTransform.size_has_been_changed = false;

            // When changing size, all positions need to be updated
            uiTransform.x_has_been_changed = true;
            uiTransform.y_has_been_changed = true;
        }

        if (uiTransform.x_has_been_changed) {
            update_x_real_recursive();
            uiTransform.x_has_been_changed = false;
        }
        if (uiTransform.y_has_been_changed) {
            update_y_real_recursive();
            uiTransform.y_has_been_changed = false;
        }


        texture_shader->set(uiTransform.uiPrimitiveTransform16, renderedTexture);

        texture_shader->draw();

    }




    std::vector<Primitive*> tempFlatTree = {};

    std::vector<Primitive*> Primitive::flattenTree() {
        // std::cout << "tempFlatTree.size() = " << tempFlatTree.size() << std::endl;

        while (tempFlatTree.size() != 0)
            tempFlatTree.pop_back();

        appendtoFlatTreeNested(this);

        return tempFlatTree;
    }

    void Primitive::appendtoFlatTreeNested(Primitive* _primitive) {
        tempFlatTree.push_back(_primitive);
        // appendChildrenNested()
        for (Primitive* child : _primitive->children) {
            appendtoFlatTreeNested(child);
        }
    }

    // No children primitives
    bool Primitive::isLeaf() {
        return children.size() == 0 ? true : false;
    }

}
