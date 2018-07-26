#version 430 core
layout(location = 0) in vec3 vertCoord;
layout(location = 1) in vec2 texCoord;

out vec2 texCoordOut;

uniform mat4 MVP; // ModelViewProjection (Total matrix)

void main(){
    texCoordOut = texCoord;
    gl_Position =  MVP * vec4(vertCoord, 1);
}
