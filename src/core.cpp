#include "../include/core.h"

Core::Core() : spawnid(0)
{
}

Core::~Core()
{
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_iout);

    SDL_ShowCursor(SDL_ENABLE);

    SDL_Quit();
}

void Core::init()
{
    Config::loadCfg(&m_appconf);

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0)
    {
        Logger::err("Core", SDL_GetError());
        exit(-1);
    }
    Logger::log("Core", "SDL2 init complete.");
    SDL_CreateWindowAndRenderer(DW_WIDTH, DW_HEIGHT, SDL_WINDOW_SHOWN, &m_window, &m_iout);
    if(m_window == 0 || m_iout == 0)
    {
        Logger::err("Core", "SDL2 failed to create window and/or renderer.");
        exit(-1);
    }
    Logger::log("Core", "Window & renderer created.");
    //

        Logger::err("Core", "Can't open audio device!");
    //
    SDL_SetWindowSize(m_window, m_appconf.app_width, m_appconf.app_height);
    Logger::log("Core", "Window size set.");
    SDL_SetWindowTitle(m_window, QString( m_appconf.app_name+" "+m_appconf.app_version ).toStdString().c_str() );
    Logger::log("Core", "Window title set.");
    SDL_SetRenderDrawBlendMode(m_iout, SDL_BLENDMODE_ADD);
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
        while(SDL_PollEvent(&m_event))
        {
            switch (m_event.type) {
            case SDL_QUIT:
                m_quit = true;
                break;
            default:
                break;
            }
        }
        processEvents();
        //
        draw_objs();

        SDL_RenderPresent(m_iout);
        SDL_Delay(1000/TARGET_FPS);
    }
    return 0;
}

void Core::draw_objs()
{
    Actor2d a;
    for(int i=m_scene.objs().count()-1; i >= 0; i--)
    {
        a = m_scene.objs().values().at(i);
        if(a.type == "actor" || a.type == "building")
        {
            if(m_processor.isSelected(a) || m_processor.isMouseOver(a.real_rect))
            {
                SDL_SetRenderDrawColor(m_iout, 0, 255, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(m_iout, &a.healthBar());
            }
            //
            if(a.move_direction != IDLE)
            {
                SDL_RenderDrawLine(m_iout, a.rect.x+a.rect.w/2, a.rect.y+a.rect.h-16, a.targetPos.x+a.rect.w/2, a.targetPos.y+a.rect.h-16);
            }
#ifdef DEBUG
            SDL_SetRenderDrawColor(m_iout, 255, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawRect(m_iout, &a.real_rect);
#endif
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

            if(m_processor.isMouseOver(a.real_rect) && m_processor.isMouseUp(3))
            {
                m_processor.selectionList[a.getName()] = a;
            }
        }
        else
        {
            if( (selection_rect.w >= 8 && selection_rect.h >= 8) || (selection_rect.w <= -8 && selection_rect.h <= -8) )
            {
                m_processor.addSelected(m_scene.objs().values(), selection_rect);
            }
            //
            m_processor.oldmpos = vec2(0,0);
        }
        SDL_SetRenderDrawColor(m_iout, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderCopy(m_iout, m_texloader.getTex( a.tex() ), &m_animator.frame(a.curAnim, a.curFrame), &a.rect);
        m_scene.objs()[a.getName()].nextFrame( m_animator.fcount(a.curAnim), m_animator.fps(a.curAnim) );
    }
    mouse_rect.x = m_processor.mousePos().x-mouse_rect.w/2; mouse_rect.y = m_processor.mousePos().y;
    SDL_RenderCopy(m_iout, m_texloader.getTex("cursor_"+m_processor.mouse_state), NULL, &mouse_rect);
}

void Core::processEvents()
{
    m_processor.updateMouse();
    m_processor.mouse_state = "none";
    //
    if(m_processor.keyDown(SDL_SCANCODE_LCTRL))
        SDL_RenderSetScale(m_iout, 2.0f, 2.0f);
    else if(m_processor.keyDown(SDL_SCANCODE_LSHIFT))
        SDL_RenderSetScale(m_iout, 1.0f, 1.0f);
    //
    Action act;
    QStringList info;
    vec2 sp;
    for(Actor2d obj : m_scene.objs().values())
    {
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

            case SOUND_ACTION:
                m_audiomgr.playSound(act.stringData());
            break;
            case SCENE_ACTION:
                m_sceneparser.loadScene(&m_scene, act.stringData(), m_appconf);
                m_scene.start(&m_audiomgr);
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
            m_scene.addActor(sp, vec2(64,72), vec2(48,64), info[1]+QString::number(spawnid), "actor", "ussr_soldier", "ussr_sw");
            spawnid++;
            //
            srand(time(0));
            m_audiomgr.playSound("ussr_soldier_spawn_"+QString::number(random() % 3));
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
        act.reset();
    }
}
