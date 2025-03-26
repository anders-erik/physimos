#version 330 core

layout(location = 0) in vec3 aPos;

// out vec2 textCoord;

void main()
{
    // vec4 pos_tmp = vec4(0.0);

    // if(gl_VertexID == 0){
    //     pos_tmp = vec4(0.0, 0.0, 0.0, 1.0);
    // }
    // else if(gl_VertexID == 1){
    //     pos_tmp = vec4(1.0, 0.0, 0.0, 1.0);
    // }
    // else if(gl_VertexID == 2){
    //     pos_tmp = vec4(1.0, 1.0, 0.0, 1.0);
    // }

    // gl_Position = pos_tmp;

    gl_Position = vec4(aPos, 1.0);

    // textCoord = aTextCoord;
}