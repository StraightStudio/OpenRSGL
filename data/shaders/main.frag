#version 430 core

uniform sampler2D mainTexture; //this is the texture

in vec3 normalPos;
in vec2 texCoordOut; //this is the texture coord
in vec3 fragPos; // For lighting

out vec4 fragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main(){
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(normalPos);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max( dot(norm, lightDir), 0.0 );
    vec3 diffuse = diff * lightColor;

    //fragColor = texture2D(mainTexture, texCoordOut);
    vec3 result = vec3(texture2D(mainTexture, texCoordOut)) * (ambient + diffuse);
    fragColor = vec4(result , 1.0 );
} 
