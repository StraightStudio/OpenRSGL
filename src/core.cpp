#include <core.h>

Core::Core() : spawnid(0), m_fov(90.f), m_campos(0, -50.f, 0), m_camrot(30.f, 0, 0)
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
    m_loader3d.LoadModel(RES_ROOT "test.obj", "test");

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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
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

    glLoadIdentity(); // Reset Transformations

    glRotatef(m_camrot.X(), 1.f, 0.f, 0.f);
    glTranslatef(m_campos.X(), m_campos.Y(), m_campos.Z());

	glEnableClientState(GL_VERTEX_ARRAY);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glVertexPointer(3, GL_FLOAT, 0, m_loader3d.GetModel("test"));
    glDrawArrays(GL_TRIANGLES, 0, m_loader3d.GetSize("test"));
	glDisableClientState(GL_VERTEX_ARRAY);

    SDL_GL_SwapWindow(m_window);

    SDL_Delay(1000/TARGET_FPS);
}

void Core::initGL()
{
    glViewport( 0.0f, 0.0f, m_appconf.app_width, m_appconf.app_height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( m_fov, (float)m_appconf.app_width/(float)m_appconf.app_height, 0, 1000.f );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
}

void Core::processEvents()
{
    m_processor.mouse_state = "none";

    if(m_scene.sinfo.type == "game")
    {
        if(m_processor.keyDown(SDL_SCANCODE_LCTRL))
            SDL_RenderSetScale(m_iout, 2.0f, 2.0f);
        else if(m_processor.keyDown(SDL_SCANCODE_LSHIFT))
            SDL_RenderSetScale(m_iout, 1.0f, 1.0f);
    }

    Actor2d a;
    Action act;
    unistrlist info;
    info.resize(2, "");
    vec3 sp;
    for(const auto &it : m_scene.m_objs)
    {
        obj.reset();
        obj = it.second;

        if(m_scene.sinfo.type == "game" && pname != obj.parent)
            continue;

        act.reset();
        if(obj.type == "button")
            act = m_processor.processUIobject(obj);
        else if(obj.type == "actor")
            act = m_processor.processActor(obj);
        else if(obj.type == "building")
            act = m_processor.processBuilding(obj);

        switch(act.id)
        {
            case POS_ACTION:
                m_scene.m_objs[obj.getName()].setPos( act.vec3Data(1) );
            break;
            case POS_RES_ACTION:
                m_scene.m_objs[obj.getName()].setDim( act.vec3Data(0) );
                m_scene.m_objs[obj.getName()].setPos( act.vec3Data(1) );
            break;
            case RESIZE_ACTION:
                m_scene.m_objs[obj.getName()].setDim( act.vec3Data(0) );
            break;
            case RES_SND_ACTION:
                m_audiomgr.playSound(act.data);
                m_scene.m_objs[obj.getName()].setDim( act.vec3Data(0) );
            break;
            case POS_RES_SND_ACTION:
                m_audiomgr.playSound(act.data);
                m_scene.m_objs[obj.getName()].setDim( act.vec3Data(0) );
                m_scene.m_objs[obj.getName()].setPos( act.vec3Data(1) );
            break;

            case MOV_ACTION:
                m_scene.m_objs[obj.getName()].moveTo(act.vec3Data(1));
            break;

            case SND_MOV_ACTION:
                m_audiomgr.playSound(act.data);
                m_scene.m_objs[obj.getName()].moveTo(act.vec3Data(1));
            break;

            case SOUND_ACTION:
                m_audiomgr.playSound(act.data);
            break;
            case SCENE_ACTION:
                m_sceneparser.loadScene(m_scene, act.data, m_appconf);
                m_scene.start(&m_audiomgr);
            break;
            case SPW_ACTION:
            split(info, act.data, is_any_of(":"), token_compress_on);
            if(info.size() != 2)
                break;
            for(auto const &object : m_scene.m_objs)
            {
                a = object.second;
                if(a.structType == info[0])
                {
                    sp = a.getPos()+a.so;
                    break;
                }
            }
            if(sp.x != 0)
            {
                m_scene.addActor(m_appconf, sp, info[1].c_str(), pname);
                //
                srand(time(nullptr));
                m_audiomgr.playSound( "ussr_soldier_spawn_"+ to_string(rand()%3) );
            }
            break;
            case SELECTED_BUILDING_ACTION:

            break;
            case DESELECTED_BUILDING_ACTION:

            break;
            case QUIT_ACTION:
            m_quit = act.boolData();
            break;
            case 0:
            break;
            default:
            Config::cfgwarn( "Undefined event triggered!\n"+act.data);
            break;
        }
    }
    //
    if(m_processor.keyDown(SDL_SCANCODE_LEFT))
        m_campos.x += 1.f;
    else if(m_processor.keyDown(SDL_SCANCODE_RIGHT))
        m_campos.x -= 1.f;

    if(m_processor.keyDown(SDL_SCANCODE_UP))
        m_campos.z += 1.f;
    else if(m_processor.keyDown(SDL_SCANCODE_DOWN))
        m_campos.z -= 1.f;
    //
    obj.reset();
    if(m_processor.isMouseClicked(SDL_BUTTON_LEFT))
            m_processor.button_clicked[0] = false;
    if(m_processor.isMouseClicked(SDL_BUTTON_RIGHT))
        m_processor.button_clicked[2] = false;
}
