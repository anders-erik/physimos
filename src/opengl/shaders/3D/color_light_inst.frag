#version 460 core

uniform vec4 mesh_color;

uniform vec3 light_pos;
uniform vec3 light_color;


in vec3 color;
in vec3 frag_pos;
in vec3 frag_normal;

out vec4 FragColor;


// vec3 light_pos = vec3(0.0, 0.0, 1.0);
// vec3 light_color = vec3(0.5, 0.5, 0.5);
vec3 ambient = vec3(0.2, 0.2, 0.2);


void main()
{
    vec3 base_color = color;

    vec3  light_dir = normalize(light_pos - frag_pos);

    float diffuse_float = max(dot(light_dir, frag_normal), 0.0);
    vec3  diffuse       = diffuse_float * light_color;

    vec3  color_out  = (ambient + diffuse) * base_color;

    FragColor = vec4(color_out, 1.0);
}


