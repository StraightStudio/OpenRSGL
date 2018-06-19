#include "../include/core.h"

Core::Core() : spawnid(0)
{
}

void Core::cleanup()
{
    Logger::log("Core", "Started cleanup...");

    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_iout);

    SDL_ShowCursor(SDL_ENABLE);

    IMG_Quit();
    SDL_Quit();
}

void Core::init()
{
    Config::loadCfg(&m_appconf);
    pname = m_appconf.playername;

    if(TTF_Init() < 0)
    {
        Logger::err("Core", TTF_GetError());
    }

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0)
    {
        Logger::err("Core", SDL_GetError());
    }
    Logger::log("Core", "SDL2 init complete.");
    m_window    = SDL_CreateWindow("Stratwenty " DW_VERSION, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DW_WIDTH, DW_HEIGHT, SDL_WINDOW_SHOWN);

    m_iout      = SDL_CreateRenderer(m_window, 0, SDL_RENDERER_ACCELERATED);

    if(m_iout == 0)
    {
        Config::cfgwarn("HW renderer failed to init.\nRunning in SW mode.");
        m_iout = SDL_CreateRenderer(m_window, 0, SDL_RENDERER_SOFTWARE);
        if(m_iout == 0)
        {
            Config::cfgwarn("SW renderer failed to init.\nExiting with error: "+QString(SDL_GetError()));
            cleanup();
            exit(-1);
        }
    }

    if(m_window == 0)
    {
        Logger::err("Core", "SDL2 failed to create window.");
    }
    Logger::log("Core", "Window & renderer created.");
    SDL_SetWindowSize(m_window, m_appconf.app_width, m_appconf.app_height);
    Logger::log("Core", "Window size set.");
    SDL_SetWindowTitle(m_window, QString( m_appconf.app_name+" "+m_appconf.app_version ).toStdString().c_str() );
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
    m_texloader.loadTextures(m_iout, m_appconf);
    m_animator.loadAnimations(m_appconf);

    m_audiomgr.init();
    m_audiomgr.loadSounds(m_appconf);
    m_audiomgr.loadMusic(m_appconf);

    //
    mouse_rect.w = 32;
    mouse_rect.h = 32;
}

int Core::exec()
{
    m_quit = false;

    m_sceneparser.loadScene(&m_scene, m_appconf);
    while(!m_quit)
    {
        SDL_RenderClear(m_iout);
        // =====================================================
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
        draw_objs();

        SDL_RenderPresent(m_iout);
        SDL_Delay(1000/TARGET_FPS);
    }
    cleanup();
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
    for(Actor2d a : m_scene.objs().values())
    {
        if(m_scene.sinfo.type == "game")
        {
            if(a.type == "actor")
            {
                if(m_processor.isSelected(a) || m_processor.isMouseOver(a.real_rect))
                {
                    SDL_SetRenderDrawColor(m_iout, 0, 255, 0, SDL_ALPHA_OPAQUE);
                    SDL_RenderFillRect(m_iout, &a.healthBar());
                    if(a.move_direction != IDLE)
                    {
                        SDL_RenderDrawLine(m_iout, a.rect.x+a.rect.w/2, a.rect.y+a.rect.h-16, a.targetPos.x+a.rect.w/2, a.targetPos.y+a.rect.h-16);
                    }
        #ifdef DEBUG
                    SDL_SetRenderDrawColor(m_iout, 255, 0, 0, SDL_ALPHA_OPAQUE);
                    SDL_RenderDrawRect(m_iout, &a.real_rect);
        #endif
                }
            }
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
                SDL_SetRenderDrawColor(m_iout, 0, 255, 0, SDL_ALPHA_OPAQUE);
                if( (selection_rect.w >= 8 && selection_rect.h >= 8) || (selection_rect.w <= -8 && selection_rect.h <= -8) )
                    SDL_RenderDrawRect(m_iout, &selection_rect);
            }
            else
            {
                if( ( (selection_rect.w >= 8 && selection_rect.h >= 8) || (selection_rect.w <= -8 && selection_rect.h <= -8) ) )
                {
                    m_processor.addSelected(m_scene.objs().values(), selection_rect);
                }
                //
                m_processor.oldmpos = vec2(0,0);
            }
        }
        SDL_SetRenderDrawColor(m_iout, 0, 0, 0, SDL_ALPHA_OPAQUE);
        if(a.visible)
        {
            SDL_RenderCopy(m_iout, m_texloader.getTex( a.tex() ), &m_animator.frame(a.curAnim, a.curFrame), &a.rect);
            m_scene.objs()[a.getName()].nextFrame( m_animator.fcount(a.curAnim), m_animator.fps(a.curAnim) );
        }
    }
    mouse_rect.x = m_processor.mousePos().x-mouse_rect.w/2; mouse_rect.y = m_processor.mousePos().y;
    SDL_RenderCopy(m_iout, m_texloader.getTex("cursor_"+m_processor.mouse_state), NULL, &mouse_rect);
}

void Core::initialSpawn()
{
    m_scene.addActor(m_appconf, vec2(200, 200), "ussr_barracks", pname);
}

void Core::processEvents()
{
    m_processor.mouse_state = "none";

    if(m_processor.keyDown(SDL_SCANCODE_LCTRL))
        SDL_RenderSetScale(m_iout, 2.0f, 2.0f);
    else if(m_processor.keyDown(SDL_SCANCODE_LSHIFT))
        SDL_RenderSetScale(m_iout, 1.0f, 1.0f);

    Action act;
    QStringList info;
    vec2 sp;
    for(Actor2d obj : m_scene.objs().values())
    {
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
                m_scene.objs()[obj.getName()].setPos( act.vec2Data(1) );
            break;
            case POS_RES_ACTION:
                m_scene.objs()[obj.getName()].setDim( act.vec2Data(0) );
                m_scene.objs()[obj.getName()].setPos( act.vec2Data(1) );
            break;
            case RESIZE_ACTION:
                m_scene.objs()[obj.getName()].setDim( act.vec2Data(0) );
            break;
            case RES_SND_ACTION:
                m_audiomgr.playSound(act.stringData());
                m_scene.objs()[obj.getName()].setDim( act.vec2Data(0) );
            break;
            case POS_RES_SND_ACTION:
                m_audiomgr.playSound(act.stringData());
                m_scene.objs()[obj.getName()].setDim( act.vec2Data(0) );
                m_scene.objs()[obj.getName()].setPos( act.vec2Data(1) );
            break;

            case MOV_ACTION:
                m_scene.objs()[obj.getName()].moveTo(act.vec2Data(1));
            break;

            case SND_MOV_ACTION:
                m_audiomgr.playSound(act.stringData());
                m_scene.objs()[obj.getName()].moveTo(act.vec2Data(1));
            break;

            case SOUND_ACTION:
                m_audiomgr.playSound(act.stringData());
            break;
            case SCENE_ACTION:
                m_sceneparser.loadScene(&m_scene, act.stringData(), m_appconf);
                m_scene.start(&m_audiomgr);
                if(m_scene.sinfo.type == "game")
                    initialSpawn();
            break;
            case SPW_ACTION:
            info = act.stringData().split(":");
            if(info.count() != 2)
                break;
            for(Actor2d a : m_scene.objs().values())
            {
                if(a.structType == info[0])
                {
                    sp = a.getPos()+a.so;
                    break;
                }
            }
            if(sp.x != 0)
            {
                m_scene.addActor(m_appconf, sp, info[1], pname);
                //
                srand(time(0));
                m_audiomgr.playSound("ussr_soldier_spawn_"+QString::number(random() % 3));
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
            Config::cfgwarn("Undefined event triggered!");
            break;
        }
    }
    if(m_processor.isMouseClicked(SDL_BUTTON_LEFT))
            m_processor.button_clicked[0] = false;
    if(m_processor.isMouseClicked(SDL_BUTTON_RIGHT))
            m_processor.button_clicked[2] = false;
}
