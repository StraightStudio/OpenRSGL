#include <renderer.h>

Renderer::Renderer() :
    FBO(0), FBT(0)
{

}
Renderer::~Renderer()
{
    SDL_ShowCursor(SDL_ENABLE);
    SDL_SetWindowGrab(m_window, SDL_FALSE);

    if(m_iout != nullptr)
        SDL_DestroyRenderer(m_iout);
    if(m_window != nullptr)
        SDL_DestroyWindow(m_window);
    //
    delete m_main_shader;
    delete m_outline_shader;
    delete m_fbuff_shader;
    //
    glDeleteTextures(1, &FBT);
    glDeleteTextures(1, &FBD);
    glDeleteBuffers(1, &fbVBO);
    glDeleteVertexArrays(1, &fbVAO);
    //
    SDL_GL_DeleteContext(m_glcontext);
}

void Renderer::render(Camera &m_cam, Scene3d *scene)
{
    // Draw all here
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        for(int i=0; i < scene->objs().size(); i++)
        {
            if(scene->obj(i)->isSelected())
            {
                glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glStencilMask(0xFF);
                glEnable(GL_DEPTH_TEST);
                scene->obj(i)->scale(1.f);
                m_main_shader->use();
                m_main_shader->setMat4("View", m_cam.matrix());
                scene->obj(i)->draw(m_main_shader);

                glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
                glStencilMask(0x00);
                glDisable(GL_DEPTH_TEST);
                scene->obj(i)->scale(1.1f);
                m_outline_shader->use();
                m_outline_shader->setMat4("View", m_cam.matrix());
                scene->obj(i)->draw(m_outline_shader);
                glStencilMask(0xFF);
                glEnable(GL_DEPTH_TEST);
            }
            else
            {
                scene->obj(i)->scale(1.f);
                m_main_shader->use();
                m_main_shader->setMat4("View", m_cam.matrix());
                scene->obj(i)->draw(m_main_shader);
            }
        }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //

    /* ======== DOESN'T WORK
    for(size_t i=0; i < m_rendertargets.size(); i++)
    {
        fprintf(stdout, "Rendering object of size %d...\n", (int)m_rendertargets.at(i).m_size);
        fflush(stdout);


    }
    */

    glBindVertexArray(fbVAO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, FBT);
        m_fbuff_shader->use();
        m_fbuff_shader->setInt("renderTexture", 0);
        glDrawArrays(GL_TRIANGLES, 0, 6); // 6 vertices
        glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

    // =========== F I N I S H E D  R E N D E R
    SDL_GL_SwapWindow(m_window);
    SDL_Delay(1000/TARGET_FPS); // 60fps
}

void Renderer::initGL()
{
    glEnable(GL_NORMALIZE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW); // Which faces are 'BACK'? Counter-ClockWise ( aka CW: 1->3->2, CCW: 1->2->3)

    glShadeModel(GL_SMOOTH);

    glEnable(GL_FOG);

    glEnable(GL_MULTISAMPLE);

    //
    GLfloat fogColor[4] = {.5f, .5f, .5f, 1.f};
    GLfloat density = 0.3f;
    glFogi (GL_FOG_MODE, GL_LINEAR);
    glFogfv (GL_FOG_COLOR, fogColor);
    glFogf (GL_FOG_DENSITY, density);
    glHint (GL_FOG_HINT, GL_NICEST);
    //

    int w,h;
    SDL_GetWindowSize(m_window, &w, &h);
    glViewport(0, 0, w, h);


    m_main_shader = new Shader();
    m_outline_shader = new Shader();
    m_fbuff_shader = new Shader();

    m_main_shader->loadShader(RES_ROOT "shaders/main.vert", RES_ROOT "shaders/main.frag"); // Vertex & Fragment shaders for texture
    m_outline_shader->loadShader(RES_ROOT "shaders/main.vert", RES_ROOT "shaders/outline.frag"); // Vertex & Fragment shaders for outline
    m_fbuff_shader->loadShader(RES_ROOT "shaders/fbuff.vert", RES_ROOT "shaders/fbuff.frag");


    Logger::log("Renderer", "OpenGL init complete.");
}

void Renderer::initSDL()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0)
    {
        Logger::err("Renderer", SDL_GetError());
    }
    Logger::log("Renderer", "SDL2 init complete.");

    m_window    = SDL_CreateWindow(DW_NAME " " DW_VERSION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DW_WIDTH, DW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    m_glcontext = SDL_GL_CreateContext(m_window);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 1 );
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetSwapInterval(1); // VSync

#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK)
        Logger::err("Core", "Failed to init GLEW!");
#endif
    m_iout      = SDL_CreateRenderer(m_window, -1, 0);


    if(m_window == nullptr)
    {
        Logger::err("Core", "SDL2 failed to create window.");
    }
}

void Renderer::setWindowTitle(unistring title)
{
    SDL_SetWindowTitle(m_window, title.c_str());
}

void Renderer::setWindowSize(glm::vec2 size)
{
    SDL_SetWindowSize(m_window, (int)size.x, (int)size.y);
}

void Renderer::grabMouse()
{
    SDL_SetWindowGrab(m_window, SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Renderer::releaseMouse()
{
    SDL_SetWindowGrab(m_window, SDL_FALSE);
    SDL_SetRelativeMouseMode(SDL_FALSE);
}

void Renderer::init(int width, int height)
{
    initSDL();
    setWindowSize(glm::vec2(width,height));
    initGL();

    if(fbVAO == 0)
        glGenVertexArrays(1, &fbVAO);
    if(fbVBO == 0)
        glGenBuffers(1, &fbVBO);
                        // vertex  //texture
    GLfloat quad[] = {  -1.f,+1.f,  0.f,1.f, // Top left
                        -1.f,-1.f,  0.f,0.f, // Bottom left
                        +1.f,-1.f,  1.f,0.f, // Bottom right

                        -1.f,+1.f,  0.f,1.f, // Top left
                        +1.f,-1.f,  1.f,0.f, // Bottom right
                        +1.f,+1.f,  1.f,1.f}; // Top right
    glBindVertexArray(fbVAO);
        glBindBuffer(GL_ARRAY_BUFFER, fbVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0); // location 0 in shaders (vertCoords)
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(1); // location 1 in shaders (texCoords)
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)(2*sizeof(GLfloat))); // after 2 elemets
    glBindVertexArray(0);

    genTexture(width, height, false, FBT);
    genTexture(width, height, true, FBD);
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO); // Bind

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,FBT,0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,FBD,0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        Logger::err("Renderer", "Can't init framebuffer!");
        exit(-1);
    }
    //

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind
    Logger::log("Renderer", "Framebuffer init complete.");
}

void Renderer::genTexture(int w, int h, bool isDepth, GLuint &texture)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, (isDepth ? GL_DEPTH_COMPONENT : GL_RGBA16), w, h, 0,
                 (isDepth ? GL_DEPTH_COMPONENT : GL_RGBA), GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
}

