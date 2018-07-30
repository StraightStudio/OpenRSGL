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

    void loadTex(unistring fname);
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
    void getTextureInfo(unistring alias, GLuint *texid);
private:
    map<unistring, GLuint> m_texids;
    Texture tmptex;
};

#endif // TEXLOADER_H
