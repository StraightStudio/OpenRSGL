#ifndef SHADER_H
#define SHADER_H

#include <depends.h>

class Shader
{
public:
    Shader();
    GLuint ID;
    void use();

    void setMat4(unistring id, glm::mat4 &matrix);
    void setInt(unistring id, int value);
    void setFloat(unistring id, float value);

    void loadShader(unistring fvname, unistring ffname);
};

#endif
