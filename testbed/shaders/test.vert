#version 460 core

layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_color;

layout(location=0) out vec3 pass_color;

void main()
{
    gl_Position = vec4(in_position.xyz, 1.0);
    pass_color = in_color;
}