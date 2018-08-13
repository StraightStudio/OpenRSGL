#include <renderer.h>

Renderer::Renderer() :
	FBO(0), finalFBO(0),
    RBD(0), RBT(0),
    fbVAO(0), fbVBO(0),
    cubemapVAO(0), cubemapVBO(0),
    scrWidth(1280), scrHeight(720)
{
    lpos = glm::vec3(0.f, .5f, 0.f);
    lcolor = glm::vec3(1.f, 1.f, 1.f);
    m_fontmgr = new FontManager;
}
Renderer::~Renderer()
{
    SDL_ShowCursor(SDL_ENABLE);
    SDL_SetWindowGrab(m_window, SDL_FALSE);

    SDL_GL_DeleteContext(m_glcontext);

    if(m_iout != nullptr)
        SDL_DestroyRenderer(m_iout);
    if(m_window != nullptr)
        SDL_DestroyWindow(m_window);
    //
    SDL_Quit();

    if(m_main_shader != nullptr)
        delete m_main_shader;
    if(m_outline_shader != nullptr)
        delete m_outline_shader;
    if(m_fbuff_shader != nullptr)
        delete m_fbuff_shader;
    //
    glDeleteFramebuffers(1, &FBO);
    glDeleteFramebuffers(1, &finalFBO);
    glDeleteRenderbuffers(1, &RBT);
    glDeleteRenderbuffers(1, &RBD);
    glDeleteBuffers(1, &fbVBO);
    glDeleteVertexArrays(1, &fbVAO);
    glDeleteBuffers(1, &cubemapVBO);
    glDeleteVertexArrays(1, &cubemapVAO);
    //
}

void Renderer::render(Camera *m_cam, Scene3d *scene)
{
    // Draw all here
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        for(auto& obj : scene->objs())
        {
            m_main_shader->use();
            //
            m_main_shader->setFloat("lightPower", 1.f);
            m_main_shader->setVec3("viewPos", m_cam->getPos());
            m_main_shader->setVec3("ligtPos", lpos);
            m_main_shader->setVec3("lightColor", lcolor);
            //
            m_main_shader->setMat4("View", m_cam->matrix());
            obj.second->draw(m_main_shader);
        }

        glDepthFunc(GL_LEQUAL);
        glBindVertexArray(cubemapVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, Core::sharedPtr()->getCubemap(0));
            m_skybox_shader->use();
            m_skybox_shader->setInt("skybox", 0);
            m_skybox_shader->setMat4("View", m_cam->matrix());
            glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // 0 - screen


//    glBindVertexArray(fbVAO);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, FBT);
//        m_fbuff_shader->use();
//        m_fbuff_shader->setInt("renderTexture", 0);
//        glDrawArrays(GL_TRIANGLES, 0, 6); // 6 vertices
//        glBindTexture(GL_TEXTURE_2D, 0);
//    glBindVertexArray(0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, scrWidth, scrHeight, 0, 0, scrWidth, scrHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    // =========== F I N I S H E D  R E N D E R
    SDL_GL_SwapWindow(m_window);
    SDL_Delay(1000/TARGET_FPS); // 1000 / 60
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

    glEnable(GL_MULTISAMPLE);

    int w,h;
    SDL_GetWindowSize(m_window, &w, &h);
    glViewport(0, 0, w, h);

    m_main_shader = new Shader;
    m_outline_shader = new Shader;
    m_fbuff_shader = new Shader;
    m_skybox_shader = new Shader;

    m_main_shader->loadShader(RES_ROOT "shaders/main.vert", RES_ROOT "shaders/main.frag");
    m_outline_shader->loadShader(RES_ROOT "shaders/main.vert", RES_ROOT "shaders/outline.frag");
    m_fbuff_shader->loadShader(RES_ROOT "shaders/fbuff.vert", RES_ROOT "shaders/fbuff.frag");
    m_skybox_shader->loadShader(RES_ROOT "shaders/skybox.vert", RES_ROOT "shaders/skybox.frag");

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
    SDL_GL_MakeCurrent(m_window, m_glcontext);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 1 );
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

    //
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    int *major = (int*)calloc(1, sizeof(int));
    int *minor = (int*)calloc(1, sizeof(int));
	glGetIntegerv(GL_MAJOR_VERSION, major);
	glGetIntegerv(GL_MINOR_VERSION, minor);

    fprintf(stdout, "Maximum supported version - OpenGL %d.%d\n", *major, *minor); // Pointer, so we must use: *ptr to get value
    fflush(stdout);

    if(major != NULL)
    {
    	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, *major); // Maximum available
    	free(major);
    }
    else
    	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // OpenGL 4.x
    if(minor != NULL)
    {
    	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, *minor); // Maximum available
    	free(minor);
    }
    else
    	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3); // OpenGL x.3
    //

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

void Renderer::genRenderbuffer(int w, int h, bool isDepth, GLuint &buff)
{
    if(buff == 0)
        glGenRenderbuffers(1, &buff);
    glBindRenderbuffer(GL_RENDERBUFFER, buff);
		int samples = 4; // May be 8 or craZZyyy 16!!!
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, isDepth ? GL_DEPTH_COMPONENT24 : GL_RGBA16, w, h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, isDepth ? GL_DEPTH_ATTACHMENT : GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, buff);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Renderer::init(int width, int height)
{
	scrWidth = width; scrHeight = height;
    initSDL();
    setWindowSize(glm::vec2(scrWidth,scrHeight));
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

    if(FBO == 0)
    	glGenFramebuffers(1, &FBO);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO); // Bind
	genRenderbuffer(scrWidth, scrHeight, false, RBT);
	genRenderbuffer(scrWidth, scrHeight, true, RBD);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        Logger::err("Renderer", "Can't init framebuffer!");
        exit(-1);
    }
    //
    genSkybox();

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind framebuffer
    Logger::log("Renderer", "Framebuffer init complete.");
}

void Renderer::genSkybox()
{
	GLfloat skyboxVertices[] = {
	            // positions
	            -10.0f,  10.0f, -10.0f,
	            -10.0f, -10.0f, -10.0f,
	             10.0f, -10.0f, -10.0f,
	             10.0f, -10.0f, -10.0f,
	             10.0f,  10.0f, -10.0f,
	            -10.0f,  10.0f, -10.0f,

	            -10.0f, -10.0f,  10.0f,
	            -10.0f, -10.0f, -10.0f,
	            -10.0f,  10.0f, -10.0f,
	            -10.0f,  10.0f, -10.0f,
	            -10.0f,  10.0f,  10.0f,
	            -10.0f, -10.0f,  10.0f,

	             10.0f, -10.0f, -10.0f,
	             10.0f, -10.0f,  10.0f,
	             10.0f,  10.0f,  10.0f,
	             10.0f,  10.0f,  10.0f,
	             10.0f,  10.0f, -10.0f,
	             10.0f, -10.0f, -10.0f,

	            -10.0f, -10.0f,  10.0f,
	            -10.0f,  10.0f,  10.0f,
	             10.0f,  10.0f,  10.0f,
	             10.0f,  10.0f,  10.0f,
	             10.0f, -10.0f,  10.0f,
	            -10.0f, -10.0f,  10.0f,

	            -10.0f,  10.0f, -10.0f,
	             10.0f,  10.0f, -10.0f,
	             10.0f,  10.0f,  10.0f,
	             10.0f,  10.0f,  10.0f,
	            -10.0f,  10.0f,  10.0f,
	            -10.0f,  10.0f, -10.0f,

	            -10.0f, -10.0f, -10.0f,
	            -10.0f, -10.0f,  10.0f,
	             10.0f, -10.0f, -10.0f,
	             10.0f, -10.0f, -10.0f,
	            -10.0f, -10.0f,  10.0f,
	             10.0f, -10.0f,  10.0f
	        };
	if(cubemapVAO == 0)
		glGenVertexArrays(1, &cubemapVAO);
	if(cubemapVBO == 0)
		glGenBuffers(1, &cubemapVBO);
	glBindVertexArray(cubemapVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubemapVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);
}
//void Renderer::genTexture(int w, int h, bool isDepth, GLuint &texture)
//{
//    glGenTextures(1, &texture);
//    glBindTexture(GL_TEXTURE_2D, texture);
//    glTexImage2D(GL_TEXTURE_2D, 0, (isDepth ? GL_DEPTH_COMPONENT : GL_RGBA16), w, h, 0,
//                 (isDepth ? GL_DEPTH_COMPONENT : GL_RGBA), GL_FLOAT, nullptr);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

//    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture
//}

