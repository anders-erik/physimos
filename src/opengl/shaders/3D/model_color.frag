#version 460 core

in vec3 fragPos;
in vec3 normal;
in vec3 color;

out vec4 FragColor;

void main() {

    // Lamp
    vec3 light_pos =   vec3(3.0, 3.0, 5.0);
    // vec3 light_color = vec3(0.3, 0.2, 0.2);
    vec3 light_color = vec3(0.5, 0.5, 0.5);
    vec3 light_dir = normalize(light_pos - fragPos);

    vec3 ambient = vec3(0.4, 0.4, 0.4);

    float diffuse_value = max(dot(light_dir, normal), 0.0);
    vec3 diffuse = vec3(diffuse_value);

    // vec3 base_color = vec3(0.5, 0.5, 0.5);
    vec3 base_color = color;
    vec3 color_out = (ambient + diffuse*light_color) * base_color;


    // FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    FragColor = vec4(color_out, 1.0);
    // FragColor = vec4(color, 1.0);

}


