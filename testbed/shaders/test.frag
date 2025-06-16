#version 460 core

#extension GL_EXT_nonuniform_qualifier:enable

layout(location=0) out vec4 out_color;

layout(location=0) in vec3 color;

void main()
{
    out_color = vec4(color.xyz, 1.0);
}