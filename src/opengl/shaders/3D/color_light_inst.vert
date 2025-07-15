#version 460 core

layout (location = 0 ) in vec3 a_pos;
layout (location = 1 ) in vec3 a_normal;
layout (location = 2 ) in vec3 a_color;

uniform mat4 view;
uniform mat4 perspective;

out vec3 frag_pos;
out vec3 frag_normal;

out vec3 color;

vec2 instance_xy_pos(uint instance_id)
{
    uint x_uint = instance_id;
    uint y_uint = 4000000000 - instance_id;

    // xorshift
    x_uint ^= x_uint << 13;
    x_uint ^= x_uint >> 17;
    x_uint ^= x_uint << 5;
    y_uint ^= y_uint << 13;
    y_uint ^= y_uint >> 17;
    y_uint ^= y_uint << 5;

    x_uint = x_uint % 100;
    y_uint = y_uint % 100;

    float x = float(x_uint) * 0.1;
    float y = float(y_uint) * 0.1;
    return vec2(x, y);
}

void main()
{
    mat4 model_mat = mat4(1.0);
    vec2 pos_xy = instance_xy_pos(gl_InstanceID);
    model_mat[3][0] = pos_xy.x;
    model_mat[3][1] = pos_xy.y;

    gl_Position = perspective * view * model_mat * vec4(a_pos, 1.0);

    vec3 normal_model = normalize(a_pos);

    frag_normal = mat3(transpose(inverse(model_mat))) * a_normal;
    frag_pos = vec3(model_mat * vec4(a_pos, 1.0));
    
    color = a_color;
}


