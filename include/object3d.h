#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <depends.h>
#include <config.h>

struct Texture
{
    int width, height, depth;
    uchar* data;
    unistring name;

    Texture(uint w, uint h, uchar* d)
        :   width(w), height(h), data(d)
    {

    }

    Texture()
        :   width(0), height(0), data(nullptr)
    {

    }

    void loadTex(unistring fname)
    {
        name = fname;
        data = stbi_load(name.c_str(), &width, &height, &depth, 0);
        stbi_vertical_flip(data, width, height, depth);
    }

    void unloadTex()
    {
        width = 0;
        height = 0;

        stbi_image_free(data);
        data = nullptr;
    }

    ~Texture()
    {
        unloadTex();
    }
};

class Object3d
{
public:
    Object3d();
    ~Object3d();
    void draw(GLuint shader_program, glm::mat4 &cam_matrix);

    void setTex(unistring fname);

    void update(const vector<GLfloat> &verts={0}, int draw_type=GL_STATIC_DRAW);
    void update(int draw_type=GL_STATIC_DRAW, unistring targetTex="none");

    //
    void move(glm::vec3 mv);
    void rotate(glm::vec3 axis, float angle);
    //

    GLuint m_matid, m_texid; // texid for opengl, texloc for glsl
    GLuint m_texloc, m_vertloc;
    glm::mat4 m_matrix;

    glm::vec3 pos, rot;

    GLuint m_VAO, m_buffer, m_uvbuff; // Vertex Array Object, vertex buffer, texture buffer
    size_t m_size, m_tsize;

    vector<GLfloat> vertices, texCoords;

    glm::mat4 MoveMatrix, RotateMatrix, ScaleMatrix;

    Texture mainTex;
};

#endif // OBJECT3D_H
