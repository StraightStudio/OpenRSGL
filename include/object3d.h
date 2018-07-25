#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <depends.h>
#include <vec3.h>

class Object3d
{
public:
    Object3d();
    void draw(GLuint shader_program);

    void update(const vector<GLfloat> &verts={0}, int draw_type=GL_STATIC_DRAW);

    GLuint m_VAO, m_buffer; // Vertex Array Object, vertex buffer
    size_t m_size;

    vector<GLfloat> vertices;
};

#endif // OBJECT3D_H
