#include "../include/core.h"

Core::Core()
{

}

void Core::setWindow(int width, int height, QString title)
{
    SDL_SetWindowSize(m_window, width, height);
    m_log.log("Core", "Window size set.");
    SDL_SetWindowTitle(m_window, title.toStdString().c_str());
    m_log.log("Core", "Window title set.");
}

void Core::init()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0)
    {
        m_log.err("Core", SDL_GetError());
        exit(-1);
    }
    m_log.log("Core", "SDL2 init complete.");
    SDL_CreateWindowAndRenderer(DW_WIDTH, DW_HEIGHT, SDL_WINDOW_SHOWN, &m_window, &m_iout);
    if(m_window == 0 || m_iout == 0)
    {
        m_log.err("Core", "SDL2 failed to create window and/or renderer.");
        exit(-1);
    }
    m_log.log("Core", "Window & renderer created.");
    //
    m_texloader.addTex("mario.png", "mario", m_iout);

    m_scene.spawn(vec2(200, 200), "mario", &m_texloader);
    m_log.log("Core", "Spawned object.");
}

int Core::exec()
{
    m_quit = false;
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
        m_scene.draw(m_iout);
        SDL_RenderPresent(m_iout);
    }
    return 0;
}
