#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <depends.h>
#include <config.h>
#include <texloader.h>
#include <shader.h>

class Object3d
{
public:
    Object3d();
    ~Object3d();
    void draw(Shader *shader);

    void setTex(GLuint texid);
    //
    void move(glm::vec3 mv);
    void rotate(glm::vec3 axis, float angle);
    void scale(float factor);
    //
    void select(bool yes);
    bool isSelected();
    uint vertexCount();

    void setModel(GLuint addr, uint sz);

    GLuint m_vao;
    uint m_size;

    GLuint m_texid; // texid for opengl, texloc for glsl

    glm::vec3 pos, rot;
    float m_scale;

    glm::mat4 ModelMatrix;
private:
    bool selected;
};

#endif // OBJECT3D_H
