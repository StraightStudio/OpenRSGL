#version 430 core

uniform sampler2D mainTexture; //this is the texture
in vec2 texCoordOut; //this is the texture coord

void main(){
    gl_FragColor = texture2D(mainTexture, texCoordOut);
} 
