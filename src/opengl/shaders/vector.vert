#version 330 core

layout (location = 0) in vec3 vertPos;

uniform vec3 vector;
uniform float vector_scale;
uniform vec3 vector_color;

uniform mat4 rotation_mat;
uniform mat4 translation_mat;
uniform mat4 view;
uniform mat4 projection;


out vec4 color;

void main(){

    color = vec4(vector_color, 1.0);

    vec3 vertPos_scaled = vertPos * vector_scale;

    // gl_Position = projection * view * translation_mat * vec4(scaled_vector, 1.0);
    // gl_Position = projection * view * translation_mat * vec4(vertPos, 1.0);

    gl_Position = projection * view * translation_mat * rotation_mat * vec4(vertPos_scaled, 1.0);
}