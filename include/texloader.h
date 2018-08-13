#ifndef TEXLOADER_H
#define TEXLOADER_H

#include <depends.h>
#include <logger.h>

struct Texture
{
    uint32 width, height, depth;
    GLubyte* data;

    Texture(uint w, uint h, GLubyte* d)
        :   width(w), height(h), data(d)
    {

    }

    Texture()
        :   width(0), height(0), data(nullptr)
    {

    }

    void loadTex(unistring fname, bool cubemap=false);
    void unloadTex()
    {
        width,height = 0;

        free(data);
    }
};

class TexLoader
{
public:
    TexLoader();
    ~TexLoader();

    void loadTexture(unistring tf, unistring alias);
    GLuint loadTexture(unistring tf);
    void loadCubemap(vector<unistring> cbf, int id);
    void getTextureInfo(unistring alias, GLuint *texid);
    GLuint cubemap(int id);
private:
    map<unistring, GLuint> m_texids;
    vector<GLuint> m_cubemaps;
    Texture tmptex;
};

#endif // TEXLOADER_H
