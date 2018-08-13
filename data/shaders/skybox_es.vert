#version 140 es
layout (location = 0) in vec3 vertPos;

out vec3 TexCoords;

uniform mat4 View;

void main()
{
    TexCoords = vertPos;
    vec4 pos = View * vec4(vertPos, 1.0);
    gl_Position = pos.xyww;
}
