
#ifdef _WIN64
#include <core.h>
#else 
#include <include/core.h>
#endif

Core::Core() : spawnid(0), m_console(false), m_consolecolour({255, 255, 255, 0}), m_citem(0)
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



    if(m_consoletext != nullptr)
        SDL_DestroyTexture(m_consoletext);

    m_cinput.clear();
    m_consoleHistory.clear();

    if(m_consolefont != nullptr)
        TTF_CloseFont(m_consolefont);
    //

    if(m_iout != nullptr)
        SDL_DestroyRenderer(m_iout);
    if(m_window != nullptr)
        SDL_DestroyWindow(m_window);
    SDL_ShowCursor(SDL_ENABLE);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

}

void Core::init()
{
    Config::loadCfg(m_appconf);
    pname = m_appconf.playername;

    m_camrect.x = 0; m_camrect.y = 0;
    m_camrect.w = m_appconf.app_width;
    m_camrect.h = m_appconf.app_height;

    if(TTF_Init() < 0)
    {
        Logger::err("Core", TTF_GetError());
    }
    m_consolefont = TTF_OpenFont(RES_ROOT "fonts/freemono.ttf", 18);

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0)
    {
        Logger::err("Core", SDL_GetError());
    }
    Logger::log("Core", "SDL2 init complete.");
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

    m_consolerect.x = 0; m_consolerect.y = 0;
    m_consolerect.w = m_appconf.app_width; m_consolerect.h = 24;

     m_consoletextrect.x = 0; m_consoletextrect.y = 0;
     m_consoletextrect.w = 16; m_consoletextrect.h = 24;


    if(m_appconf.is_full)
        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);

    SDL_ShowCursor(SDL_DISABLE);
    //
    m_texloader.loadTextures(m_iout, m_appconf);
    m_animator.loadAnimations(m_appconf);

    m_audiomgr.init();
    m_audiomgr.loadSounds(m_appconf);
    m_audiomgr.loadMusic(m_appconf);

    //
    mouse_rect.w = 32;
    mouse_rect.h = 32;

    updateConsole();
}

void Core::updateConsole()
{
    m_consoletext = SDL_CreateTextureFromSurface(m_iout, TTF_RenderText_Solid(m_consolefont, m_cinput.c_str(), m_consolecolour) );
    TTF_SizeText(m_consolefont, m_cinput.c_str(),&m_consoletextrect.w,&m_consoletextrect.h);
}

int Core::exec()
{
    m_quit = false;

    m_sceneparser.loadScene(m_scene, m_appconf);
    m_scene.start(&m_audiomgr);
    while(!m_quit)
    {
        SDL_RenderClear(m_iout);
        // =====================================================
        int c;
        while(SDL_PollEvent(&m_event))
        {
            switch (m_event.type) {
                case SDL_QUIT:
                    m_quit = true;
                break;
                case SDL_KEYDOWN:
                    if(m_console)
                    {
                        if(m_event.key.keysym.scancode == CONSOLE_KEY)
                            m_console = false;
                        if(m_event.key.keysym.scancode == SDL_SCANCODE_RETURN)
                        {
                            vector<unistring> parts;
                            split(parts, m_cinput, is_any_of(" "), token_compress_on);
                            if(parts.size() > 1)
                            {
                                if(parts[0] == "spawn")
                                {
                                    if(m_appconf.app_models.count(parts[1]) > 0)
                                        m_scene.addActor(m_appconf, m_processor.mousePos(), parts[1], pname);
                                }
                            }
                            else
                            {
								if (m_cinput == "exit") {
									cleanup();
									exit (0);
								}
                            }

                            m_consoleHistory.push_back(m_cinput);
                            //
                            m_cinput.clear();
                            updateConsole();
                            m_citem = m_consoleHistory.size();
                        }
                        else if(m_event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
                        {
                            if(m_cinput.length() > 0)
                                m_cinput.pop_back();
                            updateConsole();
                        }
                        else if(m_event.key.keysym.scancode == SDL_SCANCODE_UP)
                        {
                            if(m_consoleHistory.size() > 0)
                            {
                                if(m_citem > 0)
                                {
                                    --m_citem;
                                    m_cinput = m_consoleHistory.at(m_citem);
                                    updateConsole();
                                }
                            }
                        }
                        else if(m_event.key.keysym.scancode == SDL_SCANCODE_DOWN)
                        {
                            if(m_consoleHistory.size() > 0)
                            {
                                if(m_citem < m_consoleHistory.size()-1)
                                {
                                    ++m_citem;
                                    m_cinput = m_consoleHistory.at(m_citem);
                                }
                                else
                                {
                                    m_cinput = "";
                                    m_citem = m_consoleHistory.size();
                                }
                                updateConsole();
                            }
                        }
                    }
                    else
                    {
                        if(m_event.key.keysym.scancode == CONSOLE_KEY)
                            m_console = true;
                    }
                break;
                case SDL_TEXTINPUT:
                    c = m_event.text.text[0];
                    if(m_console)
                    {
                        if( c >= 0x20 && c < 0x7E && c != 0x60) // 0x60 - `, 0x7E - ~
                        {
                            m_cinput += (char)c;
                            updateConsole();
                        }
                    }
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
        draw_objs();

        SDL_RenderPresent(m_iout);
        SDL_Delay(1000/TARGET_FPS);
    }
    return 0;
}

void Core::draw_objs()
{
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
                if(m_processor.isSelected(obj) || m_processor.isMouseOver(obj.real_rect))
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
            if(m_processor.oldmpos.x != 0 && m_processor.oldmpos.y != 0)
            {
                if( ( (selection_rect.w >= 8 && selection_rect.h >= 8) || (selection_rect.w <= -8 && selection_rect.h <= -8) ) )
                {
                    m_processor.addSelected(m_scene.m_objs, selection_rect);
                }
                //
                m_processor.oldmpos = vec2(0,0);
            }
        }
    }


    m_scene.doOperations();
    obj.reset();
    //
    if(m_console)
    {
        SDL_SetRenderDrawColor(m_iout, 32, 32, 32, 0);
        SDL_RenderFillRect(m_iout, &m_consolerect);
        SDL_RenderCopy(m_iout, m_consoletext, &m_camrect, &m_consoletextrect);
        SDL_SetRenderDrawColor(m_iout, 0, 0, 0, 0);
    }
    //
    mouse_rect.x = m_processor.mousePos().x-mouse_rect.w/2; mouse_rect.y = m_processor.mousePos().y;
    SDL_RenderCopy(m_iout, m_texloader.getTex( "cursor_"+m_processor.mouse_state ), &m_camrect, &mouse_rect);
}

void Core::initialSpawn()
{
    m_scene.addActor(m_appconf, vec2(200, 200), "ussr_barracks", pname);
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
    vec2 sp;
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
                m_scene.m_objs[obj.getName()].setPos( act.vec2Data(1) );
            break;
            case POS_RES_ACTION:
                m_scene.m_objs[obj.getName()].setDim( act.vec2Data(0) );
                m_scene.m_objs[obj.getName()].setPos( act.vec2Data(1) );
            break;
            case RESIZE_ACTION:
                m_scene.m_objs[obj.getName()].setDim( act.vec2Data(0) );
            break;
            case RES_SND_ACTION:
                m_audiomgr.playSound(act.data);
                m_scene.m_objs[obj.getName()].setDim( act.vec2Data(0) );
            break;
            case POS_RES_SND_ACTION:
                m_audiomgr.playSound(act.data);
                m_scene.m_objs[obj.getName()].setDim( act.vec2Data(0) );
                m_scene.m_objs[obj.getName()].setPos( act.vec2Data(1) );
            break;

            case MOV_ACTION:
                m_scene.m_objs[obj.getName()].moveTo(act.vec2Data(1));
            break;

            case SND_MOV_ACTION:
                m_audiomgr.playSound(act.data);
                m_scene.m_objs[obj.getName()].moveTo(act.vec2Data(1));
            break;

            case SOUND_ACTION:
                m_audiomgr.playSound(act.data);
            break;
            case SCENE_ACTION:
                m_sceneparser.loadScene(m_scene, act.data, m_appconf);
                m_scene.start(&m_audiomgr);
                if(m_scene.sinfo.type == "game")
                    initialSpawn();
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
    obj.reset();
    if(m_processor.isMouseClicked(SDL_BUTTON_LEFT))
            m_processor.button_clicked[0] = false;
    if(m_processor.isMouseClicked(SDL_BUTTON_RIGHT))
        m_processor.button_clicked[2] = false;
}
