#version 430 core
uniform sampler2D renderTexture; //this is the texture
in vec2 texCoordOut; //this is the texture coord

out vec4 fragColor;

void main(void)
{
    fragColor = texture2D(renderTexture, texCoordOut);
    //fragColor = vec4(1.f, 0.f, 0.f, 1.f);
}
