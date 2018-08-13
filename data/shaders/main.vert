#version 430 core
layout(location = 0) in vec3 vertCoord;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normalCoord;

out vec3 vertCoordOut;
out vec2 texCoordOut;
out vec3 fragPos;
out vec3 normalPosOut;

uniform mat4 View, Model; // ModelViewProjection (Total matrix)

void main(){
    fragPos = vec3(Model * vec4(vertCoord, 1.0));

    normalPosOut = mat3(transpose(inverse(Model))) * normalCoord;
    texCoordOut = texCoord;
    vertCoordOut = vertCoord;

    gl_Position = View * vec4(fragPos, 1.0);
}
