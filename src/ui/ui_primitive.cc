#include <iostream>
#include <vector>

#include <glad/glad.h>

#include "lib/string.hh"

#include "render/shader.hpp"
#include "file_format/bmp_loader.hpp"

#include "ui_primitive.hh"
#include "ui_globals.hh"
#include "ui/ui_texture.hh"
#include "ui/ui_font.hh"

#include "ui/render/renderer_primitive.hh"

namespace UI {

    Primitive::Primitive() {
        
        texture_shader = &shader::texture_shader;
        color_shader = &shader::color_shader;
        string_shader = &shader::string_shader;

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
        string_shader = &shader::string_shader;

        color = active_pallete.base1;
    }

    Primitive::Primitive(std::string _str) 
        :   str{ _str } 
    {
        texture_shader = &shader::texture_shader;
        color_shader = &shader::color_shader;
        string_shader = &shader::string_shader;

        primitiveType = PrimitiveType::String;
        
        str_setString(_str);
        
        set_color(transparent);
    }

    // Primitive::Primitive(Primitive* _parent, std::string _str) 
    //     :   parent  { _parent },
    //         str     { _str }
    // {
    //     z = _parent->z + 1;
    //     _parent->children.push_back(this);

    //     update_real_pos.x_recursive();
    //     update_y_real_recursive();


    //     texture_shader = &shader::texture_shader;
    //     color_shader = &shader::color_shader;

    //     primitiveType = PrimitiveType::String;
        
    //     str_setString(_str);
        
    //     set_color(transparent);
    // }




    void Primitive::str_setFontSize(FontSize _fontSize) {
        str_fontSize = _fontSize;
        str_setString(str);
    }
    void Primitive::str_setString(std::string _str) {

        str = _str;

        // Primitive size
        set_h(std::to_string(str_fontSize) + "x");
        size_t charWidth = (8 * str_fontSize) / 15;
        set_w(std::to_string(charWidth * _str.length()) + "x");

        // // Old String Texture generation
        // texture::new_texture(privateStringTexture);
        // font::update_texture_with_string_row(privateStringTexture, _str);
        // set_texture(privateStringTexture);
        updateTransformationMatrix();

        // New string generation

    }


    void Primitive::update_str(std::string _str) {

        if (str != _str) {
            str = _str;
            // str_setString(_str);
        }

    }
    void Primitive::update_str_int(int _int) {
        std::string _int_str = std::to_string(_int);
        update_str(_int_str);
    }
    void Primitive::update_str_double(double _double, int chars) {
        std::string _double_str = std::to_string(_double);
        update_str(_double_str.substr(0, chars));
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
            darkness_shift = -0.2f;
        else if (_newState == PrimitiveState::Selected) 
            darkness_shift = -0.4f;

    }



    void Primitive::set_h(std::string h_str) {
        std::vector<std::string> h_str_offset_split = plib::std_string::split(h_str, 'o');
        std::string h_base = h_str_offset_split[0];

        if(h_str_offset_split.size() > 1)
            uiTransform.input_offset.y = std::stoi(h_str_offset_split[1]);

        char unit_char = h_base[h_base.size() - 1];
        unsigned int num_value = atoi(h_base.substr(0, h_base.size() - 1).data());

        uiTransform.input_size.y = num_value;

        if (unit_char == 'x')
            uiTransform.unit_size.y = Unit::Pixel;
        else if (unit_char == '%') 
            uiTransform.unit_size.y = Unit::Percent;

        uiTransform.size_changed = true;

    }
    void Primitive::set_w(std::string w_str) {
        std::vector<std::string> w_str_offset_split = plib::std_string::split(w_str, 'o');
        std::string w_base = w_str_offset_split[0];

        if(w_str_offset_split.size() > 1)
            uiTransform.input_offset.x = std::stoi(w_str_offset_split[1]);

        char unit_char = w_base[w_base.size() - 1];
        unsigned int num_value = atoi(w_base.substr(0, w_base.size() - 1).data());

        uiTransform.input_size.x = num_value;

        if (unit_char == 'x')
            uiTransform.unit_size.x = Unit::Pixel;
        else if (unit_char == '%') 
            uiTransform.unit_size.x = Unit::Percent;

        uiTransform.size_changed = true;
    }
    void Primitive::dec_w(){

        if(uiTransform.unit_size.x == Unit::Pixel)
            uiTransform.input_size.x -= 10;
        if(uiTransform.unit_size.x == Unit::Percent)
            uiTransform.input_size.x -= 1;
        
        uiTransform.size_changed = true;
    }
    void Primitive::inc_w(){
        if(uiTransform.unit_size.x == Unit::Pixel)
            uiTransform.input_size.x += 10;
        if(uiTransform.unit_size.x == Unit::Percent)
            uiTransform.input_size.x += 1;
        
        uiTransform.size_changed = true;
    }
    void Primitive::dec_h(){
        if(uiTransform.unit_size.y == Unit::Pixel)
            uiTransform.input_size.y -= 10;
        if(uiTransform.unit_size.y == Unit::Percent)
            uiTransform.input_size.y -= 1;
        
        uiTransform.size_changed = true;
    }
    void Primitive::inc_h(){
        if(uiTransform.unit_size.y == Unit::Pixel)
            uiTransform.input_size.y += 10;
        if(uiTransform.unit_size.y == Unit::Percent)
            uiTransform.input_size.y += 1;
        
        uiTransform.size_changed = true;
    }
    void Primitive::dec_x(){
        if(uiTransform.unit_pos.x == Unit::Pixel)
            uiTransform.input_pos.x -= 10;
        if(uiTransform.unit_pos.x == Unit::Percent)
            uiTransform.input_pos.x -= 1;
        
        uiTransform.x_pos_changed = true;
    }
    void Primitive::inc_x(){
        if(uiTransform.unit_pos.x == Unit::Pixel)
            uiTransform.input_pos.x += 10;
        if(uiTransform.unit_pos.x == Unit::Percent)
            uiTransform.input_pos.x += 1;
        
        uiTransform.x_pos_changed = true;
    }
    void Primitive::dec_y(){
        if(uiTransform.unit_pos.y == Unit::Pixel)
            uiTransform.input_pos.y -= 10;
        if(uiTransform.unit_pos.y == Unit::Percent)
            uiTransform.input_pos.y -= 1;
        
        uiTransform.y_pos_changed = true;
    }
    void Primitive::inc_y(){
        if(uiTransform.unit_pos.y == Unit::Pixel)
            uiTransform.input_pos.y += 10;
        if(uiTransform.unit_pos.y == Unit::Percent)
            uiTransform.input_pos.y += 1;
        
        uiTransform.y_pos_changed = true;
    }

    void Primitive::update_h_real_recursive() {

        if (uiTransform.unit_size.y == Unit::Pixel){

            uiTransform.real_size.y = uiTransform.input_size.y;

        }
        else if (uiTransform.unit_size.y == Unit::Percent) {

            if (parent == nullptr)
                uiTransform.real_size.y = (viewport_height * uiTransform.input_size.y) / 100;
            else 
                uiTransform.real_size.y = (parent->uiTransform.real_size.y * uiTransform.input_size.y) / 100;

        }

        uiTransform.real_size.y += uiTransform.input_offset.y;

        if(uiTransform.real_size.y > uiTransform.max_size.y)
            uiTransform.real_size.y = uiTransform.max_size.y;

        if(uiTransform.real_size.y < uiTransform.min_size.y)
            uiTransform.real_size.y = uiTransform.min_size.y;

        // Reload real x locations
        int i = 0;
        for (Primitive* child : children){
            i++;
            child->update_h_real_recursive();
        }

        updateTransformationMatrix();
        // Maybe move to caller of method to prevent every descendant trying to reset when only the triggering primitive should have its flag reset
        uiTransform.size_changed = false;
    }

    void Primitive::update_w_real_recursive() {
        
        if (uiTransform.unit_size.x == Unit::Pixel) {
            
                uiTransform.real_size.x = uiTransform.input_size.x;
            
        }
        else if (uiTransform.unit_size.x == Unit::Percent) {

            if (parent == nullptr) {
                uiTransform.real_size.x = (viewport_width * uiTransform.input_size.x) / 100;
            }
            else {
                uiTransform.real_size.x = (parent->uiTransform.real_size.x * uiTransform.input_size.x) / 100;
            }
            
        }


        uiTransform.real_size.x += uiTransform.input_offset.x;

        if(uiTransform.real_size.x > uiTransform.max_size.x)
            uiTransform.real_size.x = uiTransform.max_size.x;

        if(uiTransform.real_size.x < uiTransform.min_size.x)
            uiTransform.real_size.x = uiTransform.min_size.x;


        // Reload real x locations
        for (Primitive* child : children)
            child->update_w_real_recursive();


        updateTransformationMatrix();
        // Maybe move to caller of method to prevent every descendant trying to reset when only the triggering primitive should have its flag reset
        uiTransform.size_changed = false;
    }





    void Primitive::set_x(std::string x_str) {

        char hori_char = x_str[0];
        char unit_char = x_str[x_str.size() - 1];
        unsigned int num_value = atoi(x_str.substr(1, x_str.size() - 2).data());

        uiTransform.input_pos.x = num_value;

        if (hori_char == '<')
            uiTransform.ref_pos.set_hori(HoriRef::Left);
        else if (hori_char == '>')
            uiTransform.ref_pos.set_hori(HoriRef::Right);
        else if (hori_char == '|')
            uiTransform.ref_pos.set_hori(HoriRef::Center);

        if (unit_char == 'x')
            uiTransform.unit_pos.x = Unit::Pixel;
        else if (unit_char == '%')
            uiTransform.unit_pos.x = Unit::Percent;


        uiTransform.x_pos_changed = true;
    }

    void Primitive::set_y(std::string y_str) {

        char vert_char = y_str[0];
        char unit_char = y_str[y_str.size() - 1];
        unsigned int num_value = atoi(y_str.substr(1, y_str.size() - 2).data());

        uiTransform.input_pos.y = num_value;

        if (vert_char == '^')
            uiTransform.ref_pos.set_vert(VertRef::Top);
        else if (vert_char == '_')
            uiTransform.ref_pos.set_vert(VertRef::Bottom);
        else if (vert_char == '~')
            uiTransform.ref_pos.set_vert(VertRef::Center);


        if (unit_char == 'x')
            uiTransform.unit_pos.y = Unit::Pixel;
        else if (unit_char == '%') 
            uiTransform.unit_pos.y = Unit::Percent;

        uiTransform.y_pos_changed = true;
    }

    void Primitive::update_x_real_recursive() {

        if (uiTransform.ref_pos.hori_is(HoriRef::Left) ) {

            if(uiTransform.unit_pos.x == Unit::Pixel){

                if(parent == nullptr)
                    uiTransform.real_pos.x = uiTransform.input_pos.x;
                else 
                    uiTransform.real_pos.x = parent->uiTransform.real_pos.x + uiTransform.input_pos.x;
            }
            else if(uiTransform.unit_pos.x == Unit::Percent){

                if (parent == nullptr) 
                    uiTransform.real_pos.x = viewport_width * uiTransform.input_pos.x / 100;
                else
                    uiTransform.real_pos.x = parent->uiTransform.real_pos.x + parent->uiTransform.real_size.x * uiTransform.input_pos.x / 100;
            }


        }
        else if (uiTransform.ref_pos.hori_is(HoriRef::Right)) {

            // The x value when primitives right edge would be flush with parents right edge
            // If no parent then the viewport is taken as parent
            int right_reference_x;

            

            if(uiTransform.unit_pos.x == Unit::Pixel){

                if (parent == nullptr) {
                    right_reference_x = viewport_width - uiTransform.real_size.x;
                    uiTransform.real_pos.x = right_reference_x - uiTransform.input_pos.x;
                }
                else {
                    right_reference_x = parent->uiTransform.real_pos.x + parent->uiTransform.real_size.x - uiTransform.real_size.x;
                    uiTransform.real_pos.x = right_reference_x - uiTransform.input_pos.x;
                }

            }
            else if(uiTransform.unit_pos.x == Unit::Percent){

                if (parent == nullptr) {
                    right_reference_x = viewport_width - uiTransform.real_size.x;
                    uiTransform.real_pos.x = right_reference_x - viewport_width * uiTransform.input_pos.x / 100;
                }
                else {
                    right_reference_x = parent->uiTransform.real_pos.x + parent->uiTransform.real_size.x - uiTransform.real_size.x;
                    uiTransform.real_pos.x = right_reference_x - parent->uiTransform.real_size.x * uiTransform.input_pos.x / 100;
                }

            }
        }
        else if (uiTransform.ref_pos.hori_is(HoriRef::Center)) {

            if (parent == nullptr) {
                uiTransform.real_pos.x = viewport_width / 2 - uiTransform.real_size.x / 2;
            }
            else {
                uiTransform.real_pos.x = parent->uiTransform.real_pos.x + parent->uiTransform.real_size.x / 2 - uiTransform.real_size.x / 2;
            }

        }


        // Reload real x locations
        for (Primitive* child : children)
            child->update_x_real_recursive();


        updateTransformationMatrix();
    }

    void Primitive::update_y_real_recursive() {


        // Draw root primitives directly
        if ( uiTransform.ref_pos.vert_is(VertRef::Bottom) ) {


            if(uiTransform.unit_pos.y == Unit::Pixel){

                if (parent == nullptr) {
                    uiTransform.real_pos.y = uiTransform.input_pos.y;
                }
                else {
                    uiTransform.real_pos.y = parent->uiTransform.real_pos.y + uiTransform.input_pos.y;
                }

            }
            else if(uiTransform.unit_pos.y == Unit::Percent){

                if (parent == nullptr) {
                    uiTransform.real_pos.y = viewport_height * uiTransform.input_pos.y / 100;
                }
                else {
                    uiTransform.real_pos.y = parent->uiTransform.real_pos.y + parent->uiTransform.real_size.y * uiTransform.input_pos.y / 100;
                }

            }

        }
        else if (uiTransform.ref_pos.vert_is(VertRef::Top)) {

            if(uiTransform.unit_pos.y == Unit::Pixel){

                if (parent == nullptr) {
                    int zero_point_top = viewport_height - uiTransform.real_size.y;
                    // _real_pos.y_offset = zero_point_top - _input_pos.y_px;
                    uiTransform.real_pos.y = zero_point_top - uiTransform.input_pos.y;
                }
                else {
                    // move child to align with parent top, the subtract top offset (_real_pos.y_offset)
                    uiTransform.real_pos.y = parent->uiTransform.real_pos.y + parent->uiTransform.real_size.y - this->uiTransform.real_size.y - uiTransform.input_pos.y;
                }

            }
            else if(uiTransform.unit_pos.y == Unit::Percent){

                if (parent == nullptr) {
                    int zero_point_top = viewport_height - uiTransform.real_size.y;
                    // _y_real_offset = zero_point_top - _input_pos.y_px;
                    uiTransform.real_pos.y = zero_point_top - viewport_height * uiTransform.input_pos.y / 100;
                }
                else {
                    int zero_point_top = parent->uiTransform.real_pos.y + parent->uiTransform.real_size.y - uiTransform.real_size.y;
                    // move child to align with parent top, the subtract top offset (_y_real_offset)
                    uiTransform.real_pos.y = zero_point_top - parent->uiTransform.real_size.y * uiTransform.input_pos.y / 100;
                }
            }

        }
        else if (uiTransform.ref_pos.vert_is(VertRef::Center)) {

            if (parent == nullptr) {
                uiTransform.real_pos.y = viewport_height / 2 - uiTransform.real_size.y / 2;
            }
            else {
                uiTransform.real_pos.y = parent->uiTransform.real_pos.y + parent->uiTransform.real_size.y / 2 - this->uiTransform.real_size.y / 2;
            }

        }

        // Reload real y locations
        for (Primitive* child : children)
            child->update_y_real_recursive();

        updateTransformationMatrix();
        // uiTransform.y_pos_changed = false;
    }


    // make sure the transform matrix is updated to current height, width, x, and y
    void Primitive::updateTransformationMatrix() {
        uiTransform.M_m_s.x.x = uiTransform.real_size.x;
        uiTransform.M_m_s.y.y = uiTransform.real_size.y;

        uiTransform.M_m_s.x.w = uiTransform.real_pos.x;
        uiTransform.M_m_s.y.w = uiTransform.real_pos.y;

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

        bool x_between_min_and_max = (_x > (double)uiTransform.real_pos.x) && (_x < (double)(uiTransform.real_pos.x + uiTransform.real_size.x));
        bool y_between_min_and_max = (_y > (double)uiTransform.real_pos.y) && (_y < (double)(uiTransform.real_pos.y + uiTransform.real_size.y));

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




    void Primitive::try_update_box_recursive(){

        if (uiTransform.size_changed) {
            update_h_real_recursive();
            update_w_real_recursive();
            uiTransform.size_changed = false;

            // When changing size, all positions need to be updated
            uiTransform.x_pos_changed = true;
            uiTransform.y_pos_changed = true;
        }

        if (uiTransform.x_pos_changed) {
            update_x_real_recursive();
            uiTransform.x_pos_changed = false;
        }
        if (uiTransform.y_pos_changed) {
            update_y_real_recursive();
            uiTransform.y_pos_changed = false;
        }

    }

    void Primitive::render() {

        // WARN: Does update happen when rendereding turned on?
        // YES? sinze the change flags will remain untouched and then all updates will take place prior to next render!
        if(!render_enabled) 
            return;


        try_update_box_recursive();


        // If scrollable, then we need to check children AABBs, and discard all regions of children
        if(scrollable){}


        UI::RendererPrimitive& renderer = UI::get_renderer_ui();
        renderer.draw(*this);

    }

    void Primitive::render(RendererPrimitive& renderer) {

        // WARN: Does update happen when rendereding turned on?
        // YES? sinze the change flags will remain untouched and then all updates will take place prior to next render!
        if(!render_enabled) 
            return;


        try_update_box_recursive();


        // If scrollable, then we need to check children AABBs, and discard all regions of children
        if(scrollable){}

        renderer.draw(*this);

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
