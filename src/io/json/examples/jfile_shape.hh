#pragma once

#include <string>
#include <vector>

#include "lib/log.hh"

#include "../json_types.hh"

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


    void loop_shape_array(j_array& root_array);
    f2 extract_point_from_array_2d(JsonVar& json_var);
    void warn_shape_type_mismatch(std::string parsed, std::string deduced){
        std::cout << "WARNING: Type mismatch in JPhileSHape. " << "Parsed='" << parsed << "'. Deduced='" << deduced << "'." << std::endl;
    }



public:

    JFileShape(std::string str_path) :  JsonFile(str_path){

        JsonVar& root_var = json.get_root();
        
        if(!verify_type_array(root_var, root_not_array_msg))
            return;

        j_array root_array = root_var.get_array();

        loop_shape_array(root_array);
    }

    static std::string serialize_shape(Shape& shape);

};

std::string JFileShape::serialize_shape(Shape& shape){

    JsonVar shape_object = j_object();
    shape_object.object_push( {"type", j_string("shape") } );
    shape_object.object_push( {"name", shape.get_shape_name(shape.get_type()) } );
    // JsonVar type = "shape";
    // JsonVar shape = shape.get_shape_name();
    // JsonVar points = j_array();

    for(size_t i = 0; i < shape.get_point_count(); i++){
        j_kv kv_point;
        std::string px = "p" + std::to_string(i);

        JsonVar point_array = j_array();

        point_array.get_array().push_back(j_float(shape[i].x));
        point_array.get_array().push_back(j_float(shape[i].y));

        shape_object.object_push( {px, point_array} );
    }


    // Configure serializer
    FloatFormat float_format = { float_representation::fixed_trimmed, 4};
    JSerialConfig serial_config = { serial_ws::new_lines, 4, float_format }; // 4 indent
    JsonSerializer serializer { serial_config };


    return serializer.serialize(shape_object);

}


void JFileShape::loop_shape_array(j_array& root_array){

    // Iterate over shapes
    for (JsonVar& shape_entry : root_array){
        
        if(!verify_type_object(shape_entry, msg_entry_not_object))
            return;

        OptPtr<JsonVar> type = shape_entry.object_find("type");
        if(!json_equals_string(type.get_ref(), "shape", msg_entry_type_not_shape))
            return;


        // Points
        JsonVar* point_var;
        std::vector<f2> points;
        size_t max_point_count = 10000;


        for (size_t i = 1; i < max_point_count; i++){
            
            // Find point
            std::string point_str = "p" + std::to_string(i);
            OptPtr<JsonVar> point_var_opt = shape_entry.object_find(point_str);
            if(point_var_opt.is_null())
                break;
            point_var = point_var_opt.get_ptr();
            
            // Point data is array
            if(!verify_type_array(*point_var, msg_shape_point_not_array))
                return;

            // Extract
            f2 point = extract_point_from_array_2d(*point_var);
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
        OptPtr<JsonVar> shape_type = shape_entry.object_find("shape");
        std::string parsed_shape_type = shape_type.get_ref().get_string();
        std::string deduced_shape_type = new_shape.get_shape_name(new_shape.get_type());
        if(parsed_shape_type != deduced_shape_type)
            warn_shape_type_mismatch(parsed_shape_type, deduced_shape_type);

    }

}


f2 JFileShape::extract_point_from_array_2d(JsonVar& json_var){

    j_array& point_array_var = json_var.get_array();

    if(point_array_var.size() > 2)
        plib::plog_error("JPHILE", "json_format", "Shape: 2d array contains more that 2 coordinates");
    if(point_array_var.size() < 2)
        plib::plog_error("JPHILE", "json_format", "Shape: 2d array does not contain enough coordinates");

    // X
    JsonVar& x_var = point_array_var[0];
    j_float& x_fl = x_var.get_float();
    
    // Y
    JsonVar& y_var = point_array_var[1];
    j_float& y_fl = y_var.get_float();


    return {(float)x_fl, (float)y_fl};
}
