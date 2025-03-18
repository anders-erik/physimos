#version 460 core

in vec3 fragPos;
in vec3 normal;

out vec4 FragColor;

void main() {

    vec3 light_pos =   vec3(0.0, 0.0, 5.0);
    vec3 light_color = vec3(0.3, 0.2, 0.2);
    vec3 light_dir = normalize(light_pos - fragPos);

    vec3 ambient = vec3(0.1, 0.1, 0.1);

    float diffuse_value = max(dot(light_dir, normal), 0.0);
    vec3 diffuse = vec3(diffuse_value);

    vec3 base_color = vec3(0.5, 0.5, 0.5);
    vec3 color = (ambient + diffuse*light_color) * base_color;

    FragColor = vec4(color, 1.0);

}


