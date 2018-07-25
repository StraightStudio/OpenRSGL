#include <core.h>

Core::Core() :
    spawnid(0)
{
    selection_rect.x = 0;
    selection_rect.y = 0;

    selection_rect.w = 0;
    selection_rect.h = 0;
}

Core::~Core()
{
    cleanup();
}

void Core::cleanup()
{
    Logger::log("Core", "Started cleanup...");

    m_scene.clear();
    m_scene.doOperations();

    m_animator.clear();
    m_audiomgr.clear();
    m_texloader.clear();

    if(m_iout != nullptr)
        SDL_DestroyRenderer(m_iout);
    if(m_window != nullptr)
        SDL_DestroyWindow(m_window);
    SDL_ShowCursor(SDL_ENABLE);

#ifdef TESTING
    SDL_GL_DeleteContext(m_glcontext);
#endif
    SteamAPI_Shutdown();
    IMG_Quit();
    SDL_Quit();
}

void Core::init()
{
	Config::loadCfg(m_appconf);
    pname = m_appconf.playername;
#ifdef STEAM
    if(!SteamAPI_IsSteamRunning())
        Logger::warn("Core", "SteamAPI init error!");
    else
        SteamAPI_Init();

    pname = unistring( SteamFriends()->GetPersonaName() );
    Logger::info("SteamAPI info", "Hello, "+pname+" :D !" );
#endif
    //

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0)
    {
        Logger::err("Core", SDL_GetError());
    }
    Logger::log("Core", "SDL2 init complete.");

#ifdef TESTING
    m_window    = SDL_CreateWindow("Stratwenty " DW_VERSION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DW_WIDTH, DW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    m_glcontext = SDL_GL_CreateContext(m_window);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

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
#else
    m_window    = SDL_CreateWindow("Stratwenty " DW_VERSION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DW_WIDTH, DW_HEIGHT, SDL_WINDOW_SHOWN);

    m_iout      = SDL_CreateRenderer(m_window, 0, SDL_RENDERER_ACCELERATED);

    if(m_iout == nullptr)
    {
        Config::cfgwarn("HW renderer failed to init.\nRunning in SW mode.");
        m_iout = SDL_CreateRenderer(m_window, 0, SDL_RENDERER_SOFTWARE);
        if(m_iout == nullptr)
        {
            Config::cfgwarn("SW renderer failed to init.\nExiting with error: "+unistring(SDL_GetError()));
            exit(-1);
        }
    }
#endif

    if(m_window == nullptr)
    {
        Logger::err("Core", "SDL2 failed to create window.");
    }
    Logger::log("Core", "Window & renderer created.");
    SDL_SetWindowSize(m_window, m_appconf.app_width, m_appconf.app_height);
    Logger::log("Core", "Window size set.");
    SDL_SetWindowTitle(m_window, unistring(m_appconf.app_name+" " DW_VERSION).c_str() );
    Logger::log("Core", "Window title set.");

    map_rect.w = 164;
    map_rect.h = 164;
    map_rect.x = 2;
    map_rect.y = m_appconf.app_height-map_rect.h-2;

    menu_rect.w = m_appconf.app_width;
    menu_rect.h = map_rect.h+4;
    menu_rect.x = 0;
    menu_rect.y = m_appconf.app_height-menu_rect.h;

    if(m_appconf.is_full)
        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);

    SDL_ShowCursor(SDL_DISABLE);
    //
#ifndef TESTING
    m_texloader.loadTextures(m_iout, m_appconf);
    m_animator.loadAnimations(m_appconf);

    updateConsole();
#endif

    m_audiomgr.init();
    m_audiomgr.loadSounds(m_appconf);
    m_audiomgr.loadMusic(m_appconf);

    //
    mouse_rect.w = 32;
    mouse_rect.h = 32;


    initGL();
}

int Core::exec()
{
    m_quit = false;

#ifndef TESTING
    m_sceneparser.loadScene(m_scene, m_appconf);
    m_scene.start(&m_audiomgr);
#endif

    while(!m_quit)
    {
        // =====================================================
        int c;
        while(SDL_PollEvent(&m_event))
        {
            switch (m_event.type) {
                case SDL_QUIT:
                    m_quit = true;
                break;
                case SDL_MOUSEBUTTONDOWN:
                    if(m_event.button.button > 0)
                    {
                        m_processor.button_clicked[m_event.button.button-1] = true;
                        m_processor.button_down[m_event.button.button-1] = true;
                    }
                break;
                case SDL_MOUSEBUTTONUP:
                    if(m_event.button.button > 0)
                    {
                        m_processor.button_down[m_event.button.button-1] = false;
                    }
                break;
                default:
                break;
            }
        }
        // =====================================================
        processEvents();
#ifndef TESTING
        draw_objs();
#else
        draw_objs3D();
#endif
    }
    return 0;
}

void Core::draw_objs()
{
    /*
    if(m_scene.sinfo.type == "game")
    {
        SDL_SetRenderDrawColor(m_iout, 192, 192, 192, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(m_iout, &menu_rect);
        SDL_SetRenderDrawColor(m_iout, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(m_iout, &map_rect);
        SDL_SetRenderDrawColor(m_iout, 0, 0, 0, SDL_ALPHA_OPAQUE);
    }
    for(const auto &it : m_scene.m_objs)
    {
        obj.reset();
        obj = it.second;
        if(m_scene.sinfo.type == "game")
        {
            if(obj.type == "actor")
            {
                if(m_processor.isSelected(obj) || m_processor.isMouseOver( Cube3D(obj.real_rect) ))
                {
                    SDL_SetRenderDrawColor(m_iout, 0, 255, 0, SDL_ALPHA_OPAQUE);
                    SDL_RenderFillRect(m_iout, &obj.healthBar());
                    if(obj.move_direction != IDLE)
                    {
                        SDL_RenderDrawLine(m_iout, obj.rect.x+obj.rect.w/2, obj.rect.y+obj.rect.h-16, obj.targetPos.x+obj.rect.w/2, obj.targetPos.y+obj.rect.h-16);
                    }
        #ifdef DEBUG
                    SDL_SetRenderDrawColor(m_iout, 255, 0, 0, SDL_ALPHA_OPAQUE);
                    SDL_RenderDrawRect(m_iout, &obj.real_rect);
        #endif
                    SDL_SetRenderDrawColor(m_iout, 0, 0, 0, SDL_ALPHA_OPAQUE);
                }
            }
        }
        if(obj.visible)
        {
            SDL_RenderCopy(m_iout, m_texloader.getTex( obj.tex() ), &m_animator.frame(obj.curAnim, obj.curFrame), &obj.rect);
            m_scene.m_objs[obj.getName()].nextFrame( m_animator.fcount(obj.curAnim), m_animator.fps(obj.curAnim) );
        }
    }


    if(m_scene.sinfo.type == "game")
    {
        if(m_processor.isMouseDown(1))
        {
            if(m_processor.oldmpos.x == 0 && m_processor.oldmpos.y == 0)
            {
                m_processor.oldmpos = m_processor.mousePos();
                selection_rect.x = m_processor.oldmpos.x;
                selection_rect.y = m_processor.oldmpos.y;
            }
            selection_rect.w = m_processor.mousePos().x-m_processor.oldmpos.x;
            selection_rect.h = m_processor.mousePos().y-m_processor.oldmpos.y;

            if(!SDL_RectEmpty(&selection_rect))
            {
                SDL_SetRenderDrawColor(m_iout, 0, 255, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawRect(m_iout, &selection_rect);
                SDL_SetRenderDrawColor(m_iout, 0, 0, 0, SDL_ALPHA_OPAQUE);
            }
        }
        else
        {
            if(m_processor.oldmpos.X() != 0 && m_processor.oldmpos.Y() != 0 && m_processor.oldmpos.Z() != 0)
            {
                if( ( (selection_rect.w >= 8 && selection_rect.h >= 8) || (selection_rect.w <= -8 && selection_rect.h <= -8) ) )
                {
                    m_processor.addSelected(m_scene.m_objs, selection_rect);
                }
                //
                m_processor.oldmpos = vec3(0, 0, 0);
            }
        }
    }


    m_scene.doOperations();
    obj.reset();

    mouse_rect.x = m_processor.mousePos().x-mouse_rect.w/2; mouse_rect.y = m_processor.mousePos().y;
    SDL_RenderCopy(m_iout, m_texloader.getTex( "cursor_"+m_processor.mouse_state ), &m_camrect, &mouse_rect);

    SDL_RenderPresent(m_iout);
    SDL_Delay(1000/TARGET_FPS);
    */
}

void Core::draw_objs3D()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ======================================================================================

    glm::mat4 ModelMatrix = glm::mat4(1.0f);  // Индивидуально для каждой модели

    glm::mat4 MVP = m_camera.matrix() * ModelMatrix; // Помните, что умножение матрицы производиться в обратном порядке

    // ======================================================================================

    test_obj.draw(m_shadeprog);
    m_modelview = glGetUniformLocation(m_shadeprog, "MVP");
    glUniformMatrix4fv(m_modelview, 1, GL_FALSE, &MVP[0][0]);

    SDL_GL_SwapWindow(m_window);

    SDL_Delay(1000/TARGET_FPS);
}

void Core::initGL()
{
//    glViewport( 0.0f, 0.0f, m_appconf.app_width, m_appconf.app_height );
//    glMatrixMode( GL_PROJECTION );
//    glLoadIdentity( );
//    gluPerspective( m_fov, (float)m_appconf.app_width/(float)m_appconf.app_height, 0, 1000.f );
//    glMatrixMode( GL_MODELVIEW );
//    glLoadIdentity( );
    m_camera = Camera(90.f, (float)m_appconf.app_width/(float)m_appconf.app_height);

    initShaders(RES_ROOT "shaders/main.vert", RES_ROOT "shaders/main.frag"); // Vertex & Fragment shaders

    m_loader3d.LoadModel(RES_ROOT "test.obj", test_obj.vertices);
    test_obj.update(test_obj.vertices);
}

void Core::initShaders(unistring fvertex, unistring ffragment)
{
    unistring tvs, tfs; // TemporaryString (Vertex, Fragment)

    tvs.clear();
    ifstream vshader(fvertex);
    getline(vshader, tvs, (char)vshader.eof());
    vshader.close();

    tfs.clear();
    ifstream fshader(ffragment);
    getline(fshader, tfs, (char)fshader.eof());
    fshader.close();

    m_vsh = glCreateShader(GL_VERTEX_SHADER);
    m_fsh = glCreateShader(GL_FRAGMENT_SHADER);

    //

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Компилируем Вершинный шейдер
    printf("Compiling shader: %s\n", fvertex.c_str());
    char const * vsh_src = tvs.c_str();
    glShaderSource(m_vsh, 1, &vsh_src, NULL);
    glCompileShader(m_vsh);


    // Выполняем проверку Вершинного шейдера
    glGetShaderiv(m_vsh, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(m_vsh, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(m_vsh, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
    }


    // Компилируем Фрагментный шейдер
    printf("Compiling shader: %s\n", ffragment.c_str());
    char const * fsh_src = tfs.c_str();
    glShaderSource(m_fsh, 1, &fsh_src, NULL);
    glCompileShader(m_fsh);


    // Проверяем Фрагментный шейдер
    glGetShaderiv(m_fsh, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(m_fsh, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(m_fsh, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
    }


    // Создаем шейдерную программу и привязываем шейдеры к ней
    fprintf(stdout, "Connecting shaders...\n");
    m_shadeprog = glCreateProgram();
    glAttachShader(m_shadeprog, m_vsh);
    glAttachShader(m_shadeprog, m_fsh);
    glLinkProgram(m_shadeprog);


    // Проверяем шейдерную программу
    glGetProgramiv(m_shadeprog, GL_LINK_STATUS, &Result);
    glGetProgramiv(m_shadeprog, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(m_shadeprog, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
    }

    fflush(stdout);
    //
    glDeleteShader(m_vsh);
    glDeleteShader(m_fsh);
}

void Core::processEvents()
{
    glm::vec3 dir;
    if(m_processor.keyDown(SDL_SCANCODE_LEFT))
        dir.x = -1.f;
    else if(m_processor.keyDown(SDL_SCANCODE_RIGHT))
        dir.x = +1.f;

    if(m_processor.keyDown(SDL_SCANCODE_UP))
        dir.z = +1.f;
    else if(m_processor.keyDown(SDL_SCANCODE_DOWN))
        dir.z = -1.f;

    if(dir.length() != 0)
        m_camera.rtsmove(dir);


    if(m_processor.isMouseClicked(SDL_BUTTON_LEFT))
            m_processor.button_clicked[0] = false;
    if(m_processor.isMouseClicked(SDL_BUTTON_RIGHT))
        m_processor.button_clicked[2] = false;
}
