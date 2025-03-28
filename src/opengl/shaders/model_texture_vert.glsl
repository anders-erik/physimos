#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 local;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // vec4 pos_tmp;
    // if(gl_VertexID == 0){
    //     pos_tmp = vec4(0.0, 0.0, 0.0, 1.0);
    // }
    // if(gl_VertexID == 1){
    //     pos_tmp = vec4(1.0, 1.0, 0.0, 1.0);
    // }
    // if(gl_VertexID == 2){
    //     pos_tmp = vec4(0.0, 1.0, 0.0, 1.0);
    // }

    // Outputs
    
    vec3 FragPos = vec3(model * vec4(aPos, 1.0));
    // vec3 FragPos = vec3(model * pos_tmp);
    TexCoords = aTexCoords;

    Normal = mat3(transpose(inverse(model))) * aNormal;
    // Normal =  aNormal; 

    gl_Position = projection * view * vec4(FragPos, 1.0);
    // gl_Position = projection * view * pos_tmp;
    
}