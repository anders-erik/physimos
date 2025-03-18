#version 460 core

layout (location = 0 ) in vec3 aPos;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 perspective;


out vec3 fragPos;
out vec3 normal;

void main() {
    // if(gl_VertexID == 0){
    //     gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    // }
    // if(gl_VertexID == 1){
    //     gl_Position = vec4(1.0, 1.0, 0.0, 1.0);
    // }
    // if(gl_VertexID == 2){
    //     gl_Position = vec4(0.0, 1.0, 0.0, 1.0);
    // }

    gl_Position = perspective * view * transform * vec4(aPos, 1.0);

    vec3 normal_model = normalize(aPos);

    normal = mat3(transpose(inverse(transform))) * normal_model;
    fragPos = vec3(transform * vec4(aPos, 1.0));
}


