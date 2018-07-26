#version 430 core

uniform sampler2D mainTexture; //this is the texture
in vec2 texCoordOut; //this is the texture coord

out vec4 fragColor;

void main(){
    fragColor = texture2D(mainTexture, texCoordOut);
} 
