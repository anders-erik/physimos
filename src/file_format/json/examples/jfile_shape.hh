#pragma once

#include <string>
#include <vector>

#include "lib/log.hh"

#include "../physon.hh"
#include "../physon_types.hh"

#include "math/shape.hh"
#include "jfile.hh"


class JFileShape : JsonFile {

    std::vector<Shape> shapes;

    // Log Messages
    std::string log_base = "JFileShape: ";
    std::string root_not_array_msg  = log_base + "root json value not array.";
    std::string msg_entry_not_object = log_base + "shape entry is not object.";
    std::string msg_entry_type_not_shape = log_base + "shape entry is not type shape.";
    std::string msg_shape_point_not_array = log_base + "shape point is not json array.";


    void loop_shape_array(json_array_variants& root_array);
    f2 extract_point_from_array_2d(JsonVar& json_var);
    void warn_shape_type_mismatch(std::string parsed, std::string deduced){
        std::cout << "WARNING: Type mismatch in JPhileSHape. " << "Parsed='" << parsed << "'. Deduced='" << deduced << "'." << std::endl;
    }



public:

    JFileShape(std::string str_path) :  JsonFile(str_path){

        JsonVar& root_var = json.get_root();
        
        if(!verify_type_array(root_var, root_not_array_msg))
            return;

        json_array_variants root_array = root_var.get_array();

        loop_shape_array(root_array);
    }

    static std::string serialize_shape(Shape& shape);

};

std::string JFileShape::serialize_shape(Shape& shape){

    JsonVar shape_object = json_object_variants();
    shape_object.push_to_object( {"type", json_string("shape") } );
    shape_object.push_to_object( {"name", shape.get_shape_name(shape.get_type()) } );
    // JsonVar type = "shape";
    // JsonVar shape = shape.get_shape_name();
    // JsonVar points = json_array_variants();

    for(size_t i = 0; i < shape.get_point_count(); i++){
        json_kv_variant kv_point;
        std::string px = "p" + std::to_string(i);

        JsonVar point_array = json_array_variants();

        point_array.push_to_array(json_float(shape[i].x));
        point_array.push_to_array(json_float(shape[i].y));

        shape_object.push_to_object( {px, point_array} );
    }


    // Configure serializer
    FloatFormat float_format = { float_representation::fixed_trimmed, 4};
    JSerialConfig serial_config = { serial_ws::new_lines, 4, float_format }; // 4 indent
    JsonSerializer serializer { serial_config };


    return serializer.serialize(shape_object);

}


void JFileShape::loop_shape_array(json_array_variants& root_array){

    // Iterate over shapes
    for (JsonVar& shape_entry : root_array){
        
        if(!verify_type_object(shape_entry, msg_entry_not_object))
            return;

        json_kv_variant type_kv = shape_entry.find_in_object("type");
        if(!json_equals_string(type_kv.second, "shape", msg_entry_type_not_shape))
            return;


        // Points
        json_kv_variant point_kv;
        std::vector<f2> points;
        size_t max_point_count = 10000;


        for (size_t i = 1; i < max_point_count; i++){
            
            // Find point
            std::string point_str = "p" + std::to_string(i);
            point_kv = shape_entry.find_in_object(point_str);
            if(kv_is_empty(point_kv))
                break;
            
            // Point data is array
            if(!verify_type_array(point_kv.second, msg_shape_point_not_array))
                return;

            // Extract
            f2 point = extract_point_from_array_2d(point_kv.second);
            points.push_back(point);

        }
        if(points.size() >= max_point_count)
            std::cout << "WARNING: Maximum number of allowed points was parsed in JFileShape." << std::endl;
        // std::cout << "Points found: " << points.size() << std::endl;
        

        // Create shape
        Shape& new_shape = shapes.emplace_back(points);
        new_shape.print();


        // Shape Type check
        // Make sure that if shape name exists in json file, we compare to deduced type. If they do not match then a warning is logged.
        json_kv_variant shape_type_kv = shape_entry.find_in_object("shape");
        std::string parsed_shape_type = shape_type_kv.second.get_string();
        std::string deduced_shape_type = new_shape.get_shape_name(new_shape.get_type());
        if(parsed_shape_type != deduced_shape_type)
            warn_shape_type_mismatch(parsed_shape_type, deduced_shape_type);

    }

}


f2 JFileShape::extract_point_from_array_2d(JsonVar& json_var){

    json_array_variants& point_array_var = json_var.get_array();

    if(point_array_var.size() > 2)
        plib::plog_error("JPHILE", "json_format", "Shape: 2d array contains more that 2 coordinates");
    if(point_array_var.size() < 2)
        plib::plog_error("JPHILE", "json_format", "Shape: 2d array does not contain enough coordinates");

    // X
    JsonVar& x_var = point_array_var[0];
    json_float& x_fl = x_var.get_float();
    
    // Y
    JsonVar& y_var = point_array_var[1];
    json_float& y_fl = y_var.get_float();


    return {(float)x_fl, (float)y_fl};
}
