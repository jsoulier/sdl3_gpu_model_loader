#version 450

layout(location = 0) in vec3 i_position;
layout(location = 1) in vec2 i_uv;
layout(location = 2) in vec3 i_normal;
layout(location = 0) out vec2 o_uv;
layout(location = 1) out vec3 o_normal;
layout(set = 1, binding = 0) uniform t_matrix
{
    mat4 u_matrix;
};

void main()
{
    gl_Position = u_matrix * vec4(i_position, 1.0f);
    o_uv = i_uv;
    o_normal = i_normal;
}