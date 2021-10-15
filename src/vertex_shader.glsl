#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

void main()
{
    mat4 transform;

    gl_Position = transform * vec4(position, 1.0);
}