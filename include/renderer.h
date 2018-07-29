#ifndef RENDERER_H
#define RENDERER_H

#include <depends.h>
#include <loader3d.h>
#include <object3d.h>
#include <camera.h>
#include <scene3d.h>
#include <shader.h>

class Renderer // RTS-style renderer
{
public:
    Renderer();
    ~Renderer();
    void addTarget(unistring model, unistring texture);
    void render(Camera &m_cam, Scene3d *scene);
    void init(int width, int height);
    void initGL();
    void initSDL();

    void setWindowTitle(unistring title);
    void setWindowSize(glm::vec2 size);

    void grabMouse();
    void releaseMouse();

    void genTexture(int w, int h, bool isDepth, GLuint &texture);
private:
    Shader  *m_main_shader,
            *m_outline_shader,
            *m_fbuff_shader;

    GLuint FBO, FBT, FBD; // FrameBufferObject FrameBufferTexture FrameBufferDepth
    GLuint fbVAO, fbVBO;

    SDL_Window* m_window;
    SDL_Renderer* m_iout;
    SDL_GLContext m_glcontext;
};

#endif
