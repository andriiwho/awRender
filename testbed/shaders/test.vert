#version 460 core

#extension GL_GOOGLE_include_directive:enable

layout(location=0) in vec3 position;
layout(location=1) in vec3 color;

layout(location=0) out vec3 pass_color;

void main()
{
    gl_Position = vec4(position.xyz, 1.0);
    pass_color = color;
}