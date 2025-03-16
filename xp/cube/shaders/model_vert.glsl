#version 460 core

layout (location = 0 ) in vec3 aPos;


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

    gl_Position = vec4(aPos, 1.0);
}


