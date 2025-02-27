#include <iostream>
#include <vector>

#include <glad/glad.h>

#include "lib/string.hh"

#include "render/shader.hpp"
#include "res/bmp_loader.hpp"

#include "ui_primitive.hh"
#include "ui_globals.hh"
#include "ui/ui_texture.hh"
#include "ui/ui_texture_string.hh"


namespace UI {

    Primitive::Primitive() {
        
        texture_shader = &shader::texture_shader;
        color_shader = &shader::color_shader;

        color = active_pallete.base1;

        // renderedTexture = defaultTexture;
    }
    Primitive::Primitive(Primitive* _parent)
        : parent {_parent} 
    {
        _parent->children.push_back(this);
        z = _parent->z + 1;

        update_x_real_recursive();
        update_y_real_recursive();

        texture_shader = &shader::texture_shader;
        color_shader = &shader::color_shader;

        color = active_pallete.base1;
    }

    Primitive::Primitive(std::string _str) 
        :   str{ _str } 
    {
        texture_shader = &shader::texture_shader;
        color_shader = &shader::color_shader;

        primitiveType = PrimitiveType::String;
        
        str_setString(_str);
        
        set_color(transparent);
    }

    Primitive::Primitive(Primitive* _parent, std::string _str) 
        :   parent  { _parent },
            str     { _str }
    {
        z = _parent->z + 1;
        _parent->children.push_back(this);

        update_x_real_recursive();
        update_y_real_recursive();


        texture_shader = &shader::texture_shader;
        color_shader = &shader::color_shader;

        primitiveType = PrimitiveType::String;
        
        str_setString(_str);
        
        set_color(transparent);
    }




    void Primitive::str_setFontSize(FontSize _fontSize) {
        str_fontSize = _fontSize;
        str_setString(str);
    }
    void Primitive::str_setString(std::string _str) {

        set_h(std::to_string(str_fontSize) + "x");

        size_t charWidth = (8 * str_fontSize) / 15;

        set_w(std::to_string(charWidth * _str.length()) + "x");

        texture::new_texture(privateStringTexture);
        update_texture_with_string_row(privateStringTexture, _str);

        set_texture(privateStringTexture);

        updateTransformationMatrix();
    }


    void Primitive::update_str(std::string _str) {

        if (str != _str) {
            str = _str;
            str_setString(_str);
        }

    }
    void Primitive::update_str_int(int _int) {
        std::string _int_str = std::to_string(_int);
        update_str(_int_str);
    }
    void Primitive::update_str_double(double _double) {
        std::string _double_str = std::to_string(_double);
        update_str(_double_str);
    }









    UiResult Primitive::try_find_target_component(double x, double y) {
        return UiResult();
    }
    UiResult Primitive::click() {
        return UiResult();
    }
    HoverEvent Primitive::hover_enter() {
        return HoverEvent();
    }
    HoverEvent Primitive::hover_exit() {
        return HoverEvent();
    }
    
    GrabState Primitive::grab() { 
        return GrabState(); 
    }

    /** Is called on pointer change when ui is tracking current primitive as grabbed. */
    GrabState Primitive::grabbed(double dx, double dy) {
        return GrabState();
    }
    UiResult Primitive::scroll(double y_change) {
        return UiResult();
    }


    void Primitive::set_state(PrimitiveState _newState) {
        state = _newState;


        if (_newState == PrimitiveState::Default)
            darkness_shift = 0.0f;
        else if (_newState == PrimitiveState::Hover)
            darkness_shift = 0.1f;
        else if (_newState == PrimitiveState::Selected) 
            darkness_shift = 0.2f;

    }



    void Primitive::set_h(std::string h_str) {
        std::vector<std::string> h_str_offset_split = plib::std_string::split(h_str, 'o');
        std::string h_base = h_str_offset_split[0];

        if(h_str_offset_split.size() > 1)
            uiTransform.h_offset_input = std::stoi(h_str_offset_split[1]);

        char unit_char = h_base[h_base.size() - 1];
        unsigned int num_value = atoi(h_base.substr(0, h_base.size() - 1).data());

        uiTransform.h_input = num_value;

        if (unit_char == 'x')
            uiTransform.h_unit = Unit::Pixel;
        else if (unit_char == '%') 
            uiTransform.h_unit = Unit::Percent;

        uiTransform.size_has_been_changed = true;

    }
    void Primitive::set_w(std::string w_str) {
        std::vector<std::string> w_str_offset_split = plib::std_string::split(w_str, 'o');
        std::string w_base = w_str_offset_split[0];

        if(w_str_offset_split.size() > 1)
            uiTransform.w_offset_input = std::stoi(w_str_offset_split[1]);

        char unit_char = w_base[w_base.size() - 1];
        unsigned int num_value = atoi(w_base.substr(0, w_base.size() - 1).data());

        uiTransform.w_input = num_value;

        if (unit_char == 'x')
            uiTransform.w_unit = Unit::Pixel;
        else if (unit_char == '%') 
            uiTransform.w_unit = Unit::Percent;

        uiTransform.size_has_been_changed = true;
    }
    void Primitive::dec_w(){

        if(uiTransform.w_unit == Unit::Pixel)
            uiTransform.w_input -= 10;
        if(uiTransform.w_unit == Unit::Percent)
            uiTransform.w_input -= 1;
        
        uiTransform.size_has_been_changed = true;
    }
    void Primitive::inc_w(){
        if(uiTransform.w_unit == Unit::Pixel)
            uiTransform.w_input += 10;
        if(uiTransform.w_unit == Unit::Percent)
            uiTransform.w_input += 1;
        
        uiTransform.size_has_been_changed = true;
    }
    void Primitive::dec_h(){
        if(uiTransform.h_unit == Unit::Pixel)
            uiTransform.h_input -= 10;
        if(uiTransform.h_unit == Unit::Percent)
            uiTransform.h_input -= 1;
        
        uiTransform.size_has_been_changed = true;
    }
    void Primitive::inc_h(){
        if(uiTransform.h_unit == Unit::Pixel)
            uiTransform.h_input += 10;
        if(uiTransform.h_unit == Unit::Percent)
            uiTransform.h_input += 1;
        
        uiTransform.size_has_been_changed = true;
    }
    void Primitive::dec_x(){
        if(uiTransform.x_unit == Unit::Pixel)
            uiTransform.x_input -= 10;
        if(uiTransform.x_unit == Unit::Percent)
            uiTransform.x_input -= 1;
        
        uiTransform.x_has_been_changed = true;
    }
    void Primitive::inc_x(){
        if(uiTransform.x_unit == Unit::Pixel)
            uiTransform.x_input += 10;
        if(uiTransform.x_unit == Unit::Percent)
            uiTransform.x_input += 1;
        
        uiTransform.x_has_been_changed = true;
    }
    void Primitive::dec_y(){
        if(uiTransform.y_unit == Unit::Pixel)
            uiTransform.y_input -= 10;
        if(uiTransform.y_unit == Unit::Percent)
            uiTransform.y_input -= 1;
        
        uiTransform.y_has_been_changed = true;
    }
    void Primitive::inc_y(){
        if(uiTransform.y_unit == Unit::Pixel)
            uiTransform.y_input += 10;
        if(uiTransform.y_unit == Unit::Percent)
            uiTransform.y_input += 1;
        
        uiTransform.y_has_been_changed = true;
    }

    void Primitive::update_h_real_recursive() {

        if (uiTransform.h_unit == Unit::Pixel){

            uiTransform.h_real = uiTransform.h_input;

        }
        else if (uiTransform.h_unit == Unit::Percent) {

            if (parent == nullptr)
                uiTransform.h_real = (viewport_height * uiTransform.h_input) / 100;
            else 
                uiTransform.h_real = (parent->uiTransform.h_real * uiTransform.h_input) / 100;

        }

        uiTransform.h_real += uiTransform.h_offset_input;

        if(uiTransform.h_real > uiTransform.h_max)
            uiTransform.h_real = uiTransform.h_max;

        if(uiTransform.h_real < uiTransform.h_min)
            uiTransform.h_real = uiTransform.h_min;

        // Reload real x locations
        int i = 0;
        for (Primitive* child : children){
            i++;
            child->update_h_real_recursive();
        }

        updateTransformationMatrix();
        // Maybe move to caller of method to prevent every descendant trying to reset when only the triggering primitive should have its flag reset
        uiTransform.size_has_been_changed = false;
    }

    void Primitive::update_w_real_recursive() {
        
        if (uiTransform.w_unit == Unit::Pixel) {
            
                uiTransform.w_real = uiTransform.w_input;
            
        }
        else if (uiTransform.w_unit == Unit::Percent) {

            if (parent == nullptr) {
                uiTransform.w_real = (viewport_width * uiTransform.w_input) / 100;
            }
            else {
                uiTransform.w_real = (parent->uiTransform.w_real * uiTransform.w_input) / 100;
            }
            
        }


        uiTransform.w_real += uiTransform.w_offset_input;

        if(uiTransform.w_real > uiTransform.w_max)
            uiTransform.w_real = uiTransform.w_max;

        if(uiTransform.w_real < uiTransform.w_min)
            uiTransform.w_real = uiTransform.w_min;


        // Reload real x locations
        for (Primitive* child : children)
            child->update_w_real_recursive();


        updateTransformationMatrix();
        // Maybe move to caller of method to prevent every descendant trying to reset when only the triggering primitive should have its flag reset
        uiTransform.size_has_been_changed = false;
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
        else if (hori_char == '|')
            uiTransform.horiRef = HoriRef::Center;

        if (unit_char == 'x')
            uiTransform.x_unit = Unit::Pixel;
        else if (unit_char == '%')
            uiTransform.x_unit = Unit::Percent;


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
        else if (vert_char == '~')
            uiTransform.vertRef = VertRef::Center;


        if (unit_char == 'x')
            uiTransform.y_unit = Unit::Pixel;
        else if (unit_char == '%') 
            uiTransform.y_unit = Unit::Percent;

        uiTransform.y_has_been_changed = true;
    }

    void Primitive::update_x_real_recursive() {

        if (uiTransform.horiRef == HoriRef::Left) {

            if(uiTransform.x_unit == Unit::Pixel){

                if(parent == nullptr)
                    uiTransform.x_real = uiTransform.x_input;
                else 
                    uiTransform.x_real = parent->uiTransform.x_real + uiTransform.x_input;
            }
            else if(uiTransform.x_unit == Unit::Percent){

                if (parent == nullptr) 
                    uiTransform.x_real = viewport_width * uiTransform.x_input / 100;
                else
                    uiTransform.x_real = parent->uiTransform.x_real + parent->uiTransform.w_real * uiTransform.x_input / 100;
            }


        }
        else if (uiTransform.horiRef == HoriRef::Right) {

            // The x value when primitives right edge would be flush with parents right edge
            // If no parent then the viewport is taken as parent
            int right_reference_x;

            

            if(uiTransform.x_unit == Unit::Pixel){

                if (parent == nullptr) {
                    right_reference_x = viewport_width - uiTransform.w_real;
                    uiTransform.x_real = right_reference_x - uiTransform.x_input;
                }
                else {
                    right_reference_x = parent->uiTransform.x_real + parent->uiTransform.w_real - uiTransform.w_real;
                    uiTransform.x_real = right_reference_x - uiTransform.x_input;
                }

            }
            else if(uiTransform.x_unit == Unit::Percent){

                if (parent == nullptr) {
                    right_reference_x = viewport_width - uiTransform.w_real;
                    uiTransform.x_real = right_reference_x - viewport_width * uiTransform.x_input / 100;
                }
                else {
                    right_reference_x = parent->uiTransform.x_real + parent->uiTransform.w_real - uiTransform.w_real;
                    uiTransform.x_real = right_reference_x - parent->uiTransform.w_real * uiTransform.x_input / 100;
                }

            }
        }
        else if (uiTransform.horiRef == HoriRef::Center) {

            if (parent == nullptr) {
                uiTransform.x_real = viewport_width / 2 - uiTransform.w_real / 2;
            }
            else {
                uiTransform.x_real = parent->uiTransform.x_real + parent->uiTransform.w_real / 2 - uiTransform.w_real / 2;
            }

        }


        // Reload real x locations
        for (Primitive* child : children)
            child->update_x_real_recursive();


        updateTransformationMatrix();
    }

    void Primitive::update_y_real_recursive() {


        // Draw root primitives directly
        if (uiTransform.vertRef == VertRef::Bottom) {


            if(uiTransform.y_unit == Unit::Pixel){

                if (parent == nullptr) {
                    uiTransform.y_real = uiTransform.y_input;
                }
                else {
                    uiTransform.y_real = parent->uiTransform.y_real + uiTransform.y_input;
                }

            }
            else if(uiTransform.y_unit == Unit::Percent){

                if (parent == nullptr) {
                    uiTransform.y_real = viewport_height * uiTransform.y_input / 100;
                }
                else {
                    uiTransform.y_real = parent->uiTransform.y_real + parent->uiTransform.h_real * uiTransform.y_input / 100;
                }

            }

        }
        else if (uiTransform.vertRef == VertRef::Top) {

            if(uiTransform.y_unit == Unit::Pixel){

                if (parent == nullptr) {
                    int zero_point_top = viewport_height - uiTransform.h_real;
                    // _y_real_offset = zero_point_top - _y_input_px;
                    uiTransform.y_real = zero_point_top - uiTransform.y_input;
                }
                else {
                    // move child to align with parent top, the subtract top offset (_y_real_offset)
                    uiTransform.y_real = parent->uiTransform.y_real + parent->uiTransform.h_real - this->uiTransform.h_real - uiTransform.y_input;
                }

            }
            else if(uiTransform.y_unit == Unit::Percent){

                if (parent == nullptr) {
                    int zero_point_top = viewport_height - uiTransform.h_real;
                    // _y_real_offset = zero_point_top - _y_input_px;
                    uiTransform.y_real = zero_point_top - viewport_height * uiTransform.y_input / 100;
                }
                else {
                    int zero_point_top = parent->uiTransform.y_real + parent->uiTransform.h_real - uiTransform.h_real;
                    // move child to align with parent top, the subtract top offset (_y_real_offset)
                    uiTransform.y_real = zero_point_top - parent->uiTransform.h_real * uiTransform.y_input / 100;
                }
            }

        }
        else if (uiTransform.vertRef == VertRef::Center) {

            if (parent == nullptr) {
                uiTransform.y_real = viewport_height / 2 - uiTransform.h_real / 2;
            }
            else {
                uiTransform.y_real = parent->uiTransform.y_real + parent->uiTransform.h_real / 2 - this->uiTransform.h_real / 2;
            }

        }

        // Reload real y locations
        for (Primitive* child : children)
            child->update_y_real_recursive();

        updateTransformationMatrix();
        // uiTransform.y_has_been_changed = false;
    }


    // make sure the transform matrix is updated to current height, width, x, and y
    void Primitive::updateTransformationMatrix() {

        uiTransform.uiPrimitiveTransform16[0] = uiTransform.w_real;
        uiTransform.uiPrimitiveTransform16[5] = uiTransform.h_real;

        uiTransform.uiPrimitiveTransform16[3] = uiTransform.x_real;
        uiTransform.uiPrimitiveTransform16[7] = uiTransform.y_real;

    }

    void Primitive::set_texture(unsigned int glTextureName){
        has_texture = true;
        renderedTexture = glTextureName;
    }

    void Primitive::set_color_texture(ColorTexture _colorTexture){
        renderedTexture = UI::texture::get_static_color_texture(_colorTexture);
    }

    void Primitive::set_darkness_shift(float shift_value){
        darkness_shift = shift_value;
    }
    void Primitive::reset_darkness_shift(){
        darkness_shift = 0.0f;
    }

    void Primitive::set_color(Color _color) {
        color = _color;
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
        update_z_recursive();
        // childPrimitive->z = this->z + 1;

        childPrimitive->update_x_real_recursive();
        childPrimitive->update_y_real_recursive();
    }

    void Primitive::update_z_recursive(){
        // Root primitives are identified by nullptr parent
        if(parent != nullptr)
            z = parent->z + 1;

        for(Primitive* primitive : children)
            primitive->update_z_recursive();
    }

    void Primitive::render_recursive() {
        if(!render_enabled)
            return;
        
        render();
        // Make sure children are rendered after parent as currently no depth testing or sorting is being done!
        for (Primitive* child : children) {
            child->render_recursive();
        }
    }

    void Primitive::render() {

        // WARN: Does update happen when rendereding turned on?
        // YES? sinze the change flags will remain untouched and then all updates will take place prior to next render!
        if(!render_enabled) 
            return;

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

        // RENDER BASE COLOR
        color_shader->set(uiTransform.uiPrimitiveTransform16, darkness_shift, color);
        color_shader->draw();
    


        if(has_texture){
            texture_shader->set(uiTransform.uiPrimitiveTransform16, renderedTexture);
            texture_shader->draw();
        }

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
