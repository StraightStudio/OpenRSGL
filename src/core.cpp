#include "../include/core.h"

Core::Core()
{

}

Core::~Core()
{
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_iout);

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
    if(m_appconf.is_full)
        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
    //
    m_texloader.loadTextures(m_iout, m_appconf);

    m_audiomgr.init();
    m_audiomgr.loadSounds(m_appconf);
    m_audiomgr.loadMusic(m_appconf);
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
    for(Actor2d a: m_scene.objs().values())
    {
        SDL_RenderCopy(m_iout, m_texloader.getTex( a.curAnim ), &a.getFrame(), &a.rect);
        m_scene.objs()[a.getName()].nextFrame();
    }
}

void Core::processEvents()
{
    Action act;
    for(Actor2d obj : m_scene.objs().values())
    {
        if(obj.type == "button")
        {
            act = m_processor.processUIobject(obj);

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

                case SOUND_ACTION:
                    m_audiomgr.playSound(act.stringData());
                break;
                case SCENE_ACTION:
                    m_sceneparser.loadScene(&m_scene, act.stringData(), m_appconf);
                    m_scene.start(&m_audiomgr);
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
}
