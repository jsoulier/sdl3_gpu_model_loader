#version 450

layout(location = 0) in vec2 i_uv;
layout(location = 1) in vec3 i_normal;
layout(location = 0) out vec4 o_color;
layout(set = 2, binding = 0) uniform sampler2D s_diffuse;

void main()
{
    o_color = texture(s_diffuse, i_uv);
}