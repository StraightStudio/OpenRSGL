#ifndef RENDERER_H
#define RENDERER_H

#include <depends.h>
#include <loader3d.h>
#include <object3d.h>
#include <camera.h>
#include <scene3d.h>
#include <shader.h>
#include <fontmanager.h>

class Core;

#include <core.h>

class Renderer // RTS-style renderer
{
public:
    Renderer();
    ~Renderer();
    void addTarget(unistring model, unistring texture);
    void render(Camera *m_cam, Scene3d *scene);
    void init(int width, int height);
    void initGL();
    void initSDL();

    void setWindowTitle(unistring title);
    void setWindowSize(glm::vec2 size);

    void grabMouse();
    void releaseMouse();

    void genRenderbuffer(int w, int h, bool isDepth, GLuint &buff);

    void genSkybox();

    glm::vec3 lpos; // Light position
private:
    int scrWidth, scrHeight;
    Shader  *m_main_shader,
            *m_outline_shader,
            *m_fbuff_shader,
            *m_skybox_shader;

    GLuint finalFBO, FBO, RBT, RBD; // FrameBuffer Object, RenderBuffer Texture, RenderBuffer Depth
    GLuint fbVAO, fbVBO; // For drawing to 'screen'
    GLuint cubemapVAO, cubemapVBO;

    SDL_Window* m_window;
    SDL_Renderer* m_iout;
    SDL_GLContext m_glcontext;

    FontManager* m_fontmgr;

    glm::vec3 lcolor; // Light color
};

#endif
