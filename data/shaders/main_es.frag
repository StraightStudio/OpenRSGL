#version 140 es

uniform sampler2D mainTexture; //this is the texture

in vec3 normalPosOut;
in vec2 texCoordOut; //this is the texture coord
in vec3 fragPos; // For lighting

out vec4 fragColor;

uniform int selected;
uniform float lightPower;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

float PHI = 1.61803398874989484820459*00000.1; // Golden Ratio
float PI  = 3.14159265358979323846264*00000.1; // PI
float SQ2 = 1.41421356237309504880169*10000.0; // Square Root of Two


float gold_noise(in vec2 coordinate, in float seed){
    return fract(tan(distance(coordinate*(seed+PHI), vec2(PHI, PI)))*SQ2);
}

void main(){
    /* ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor * lightPower;

    // diffuse
    vec3 norm = normalize(normalPosOut);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max( dot(norm, lightDir), 0.0 );
    vec3 diffuse = diff * lightColor * lightPower;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor * lightPower;
    */
    fragColor = texture2D(mainTexture, texCoordOut);
    if(selected == 1)
    {
        float toMix = gold_noise(texCoordOut, 0.f);
        if(toMix > 0.5f)
        {
            fragColor = mix(fragColor, vec4(0.5f, 0.5f, 0.5f, 1.f), 0.3f);
        }
    }
} 
