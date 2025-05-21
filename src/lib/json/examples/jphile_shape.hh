#include <string>
#include <vector>

#include "lib/log.hh"

#include "../physon.hh"
#include "../physon_types.hh"

#include "shape.hh"
#include "jphile.hh"


class JPhileShape : JPhile {

    std::vector<Shape> shapes;

    /** Returns the float 2d-points of a json-point array, e.g. [[0.0, 0.0],[1.0, 1.0]] */
    // std::vector<Point> unwrap_point_array(json_array_wrap point_array);

public:

    JPhileShape(Phile& _file) :  JPhile(_file) {

        // At this point we've already got a parsed json file at 'json'.


        // json_object_variants root_obj = json.root_var.get_object();

        JsonVar& root_var = json.get_root();
        if(!root_var.is_array()){
            std::string msg = "JPhileShape: root json value not array. File: " +  file.file_path_abs;
            plib::plog_error("JPHILE", "json_format", msg);
            return;
        }

        json_array_variants root_array = root_var.get_array();


        // Iterate over shapes
        for (JsonVar& shape_entry : root_array){
            
            if(!shape_entry.is_object()){
                std::string msg = "JPhileShape: shape array entry is not object. File: " +  file.file_path_abs;
                plib::plog_error("JPHILE", "json_format", msg);
                return;
            }

            // json_object_variants shape = array_entry.get_object();

            json_kv_variant type_kv = shape_entry.find_in_object("type");
            if(type_kv.second.get_string() != "shape"){
                std::string msg = "JPhileShape: array entry is not of type shape. File: " +  file.file_path_abs;
                plib::plog_error("JPHILE", "json_format", msg);
                return;
            }

            shape_t shape_type;

            json_kv_variant shape_type_kv = shape_entry.find_in_object("shape");
            if(shape_type_kv.second.get_string() == "line")
                shape_type = shape_t::line;
            
            size_t point_count;

            switch (shape_type){

            case shape_t::line:
                point_count = 2;
                break;
            
            default:
                point_count = 0;
                break;
            }

            // Verify Points In Shape
            json_kv_variant point_kv;
            std::vector<Point> points;
            for (size_t i = 1; i < point_count+1; i++){
                std::string point_str = "p" + std::to_string(i);

                point_kv = shape_entry.find_in_object(point_str);

                if(!point_kv.second.is_array()){
                    std::string msg = "JPhileShape: shape point is not of type array. File: " +  file.file_path_abs;
                    plib::plog_error("JPHILE", "json_format", msg);
                    return;
                }

                // POINT ARRAY
                json_array_variants& point_array_var = point_kv.second.get_array();

                // X
                JsonVar& x_var = point_array_var[0];
                json_float& x_fl = x_var.get_float();
                std::cout << x_fl << ", ";
                
                // Y
                JsonVar y_var = point_array_var[1];
                json_float y_fl = y_var.get_float();
                std::cout << y_fl << std::endl;
                
                points.emplace_back(x_fl, y_fl);

            }
            
            // Create shape
            Shape new_shape (points);
            new_shape.print();
        }
        
        


    };
    // ConfigShape(std::string config_string) :  JPhile(config_string, "") {};
    // std::vector<Shape>& load_shapes();
};




// std::vector<Point> ConfigShape::unwrap_point_array(json_array_wrap point_array){
//     std::vector<Point> points;

//     for(size_t i = 0; i < point_array.size(); i++){

//         JsonWrapper point_wrapper = point_array[i];

//         // Unwrap point
//         json_array_wrap point_array = physon.unwrap_array(point_wrapper);

//         json_float point_x = physon.unwrap_float(point_array[0]);
//         json_float point_y = physon.unwrap_float(point_array[1]);

//         Point point = {point_x, point_y};
//         points.push_back(point);

//     }

//     return points;
// }



// std::vector<Shape>& ConfigShape::load_shapes(){

//     physon.parse();
    
//     JsonWrapper root_object_wrapper = physon.root_wrapper;
//     json_object_wrap root_object = physon.unwrap_object(root_object_wrapper);

//     // Loop shapes
//     for(JsonWrapper kv_wrap : root_object){
//         json_kv_wrap kv = physon.unwrap_kv(kv_wrap);

//         Shape new_shape;

//         std::string shape_name = kv.first;
//         json_array_wrap point_array = physon.unwrap_array(kv.second);


//         if(shape_name == "line"){
//             new_shape.type = SHAPE::LINE;

//             // JsonWrapper point_1_wrapper = point_array[0];
//             // JsonWrapper point_2_wrapper = point_array[1];

//             // // Unwrap point 1
//             // json_array_wrap point_1_array = physon.unwrap_array(point_1_wrapper);
//             // json_float point_1_x = physon.unwrap_float(point_1_array[0]);
//             // json_float point_1_y = physon.unwrap_float(point_1_array[1]);
//             // Point point_1 = {point_1_x, point_1_y};
//             // new_shape.points.push_back(point_1);
            
//             // // Unwrap point 2
//             // json_array_wrap point_2_array = physon.unwrap_array(point_2_wrapper);
//             // json_float point_2_x = physon.unwrap_float(point_2_array[0]);
//             // json_float point_2_y = physon.unwrap_float(point_2_array[1]);
//             // Point point_2 = {point_2_x, point_2_y};
//             // new_shape.points.push_back(point_2);
//         }


//         new_shape.points = unwrap_point_array(point_array);

//         shapes.push_back(new_shape);

//     }

//     return shapes;
// }
