#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <depends.h>
#include <config.h>
#include <shader.h>

struct Texture
{
    uint32 width, height, depth;
    GLubyte* data;
    unistring name;

    Texture(uint w, uint h, uchar* d)
        :   width(w), height(h)
    {

    }

    Texture()
        :   width(0), height(0), data(nullptr)
    {

    }

    void loadTex(unistring fname);

    void unloadTex()
    {
        width = 0;
        height = 0;

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
    void draw(Shader &shader);

    void setTex(unistring fname);

    void update(uint draw_type=GL_STATIC_DRAW);
    void update(unistring targetTex="none");

    //
    void move(glm::vec3 mv);
    void rotate(glm::vec3 axis, float angle);
    void scale(float factor);
    //
    bool isSelected();
    uint vertexCount();

    GLuint m_matid, m_texid; // texid for opengl, texloc for glsl
    GLuint m_texloc, m_vertloc;

    glm::vec3 pos, rot;
    float m_scale;

    GLuint m_VAO, m_buffer; // Vertex Array Object, vertex buffer
    size_t m_size;

    vector<GLfloat> vertexData;

    glm::mat4 ModelMatrix;

    Texture mainTex;
private:
    bool selected;
};

#endif // OBJECT3D_H
