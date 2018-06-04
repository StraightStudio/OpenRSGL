#include "../include/core.h"

Core::Core()
{

}

Core::~Core()
{
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_iout);
    for(Mix_Music* track : tracks)
        Mix_FreeMusic(track);

    for(Mix_Chunk* chunk : sounds.values())
        Mix_FreeChunk(chunk);

    Mix_Quit();
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
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        Logger::err("Core", "Can't open audio device!");
    for(QString track : m_appconf.background_audio_list)
    {
        tracks.append(Mix_LoadMUS(QString(AUDIO_ROOT+track).toStdString().c_str()));
    }    
    Mix_HaltMusic();
    Mix_PlayMusic(tracks[0], 0);
    //
    SDL_SetWindowSize(m_window, m_appconf.app_width, m_appconf.app_height);
    Logger::log("Core", "Window size set.");
    SDL_SetWindowTitle(m_window, QString( m_appconf.app_name+" "+m_appconf.app_version ).toStdString().c_str() );
    Logger::log("Core", "Window title set.");
    if(m_appconf.is_full)
        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
    //
    m_texloader.loadTextures(m_iout, m_appconf);
}

int Core::exec()
{
    m_quit = false;

    m_sceneparser.readScene(&m_scene, m_appconf.getStartScene());

    for(Actor2d a : m_scene.objs().values())
    {
        if(a.type == "button")
        {
            QString arg = a.triggerArgument("hover");
            if(!arg.isEmpty())
                sounds[arg] = Mix_LoadWAV(QString(AUDIO_ROOT+arg).toStdString().c_str());
        }
    }
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
        //
        parse_ui();
        //
        draw_objs();

        SDL_RenderPresent(m_iout);
    }
    return 0;
}

void Core::parse_ui()
{
    for(Actor2d obj : m_scene.objs().values())
    {
        if(obj.type == "button")
        {
            if(GameEvents::isMouseOver(&obj.getRect()))
            {
                if(ui_btns[obj.getName()] != "hovered")
                {
                    if(obj.triggerAction("hover") == SOUND_ACTION)
                    {
                        Mix_PlayChannel(-1, sounds[obj.triggerArgument("hover")], 0);
                    }
                    m_scene.objs()[obj.getName()].setDim(vec2(obj.getRect().w+16, obj.getRect().h+16));
                    m_scene.objs()[obj.getName()].setPos(vec2(obj.getRect().x-8, obj.getRect().y-8));
                    ui_btns[obj.getName()] = "hovered";
                }
                if(GameEvents::isMouseDown(1))
                {
                    switch(obj.triggerAction("click"))
                    {
                        case SCENE_ACTION:
                            m_sceneparser.loadScene(&m_scene, obj.triggerArgument("click"));
                        break;
                        case QUIT_ACTION:
                            m_quit=true;
                        break;
                    }
                }
            }

            if(!GameEvents::isMouseOver(&obj.getRect()))
            {
                if(ui_btns[obj.getName()] == "hovered")
                {
                    m_scene.objs()[obj.getName()].setDim(vec2(obj.getRect().w-16, obj.getRect().h-16));
                    m_scene.objs()[obj.getName()].setPos(vec2(obj.getRect().x+8, obj.getRect().y+8));
                    ui_btns[obj.getName()] = "none";
                }
            }
        }
        // ======================================================================================

    }
}

void Core::draw_objs()
{
    for(Actor2d a: m_scene.objs().values())
    {
        SDL_RenderCopy(m_iout, m_texloader.getTex( a.getTex(0) ), NULL, &a.rect);
    }
}
