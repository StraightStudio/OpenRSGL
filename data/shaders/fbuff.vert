#version 430 core
layout(location = 0) in vec3 vertCoord;
layout(location = 1) in vec2 texCoord;

out vec2 texCoordOut;

void main()
{
    gl_Position = vec4(vertCoord, 1.0);
    texCoordOut = texCoord;
}
