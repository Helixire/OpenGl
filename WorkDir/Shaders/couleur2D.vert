#version 330 core

layout (location = 0) in vec3 in_Vertex;
layout (location = 1) in vec3 in_Color;

out vec3 Color;

uniform mat4    MVP;

void main()
{
    Color = in_Color;
    gl_Position = MVP * vec4(in_Vertex, 1.0);
}
