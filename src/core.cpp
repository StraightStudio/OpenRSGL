#include "../include/core.h"

Core::Core()
{

}

void Core::setWindow(int width, int height, QString title)
{
    SDL_SetWindowSize(m_window, width, height);
    Logger::log("Core", "Window size set.");
    SDL_SetWindowTitle(m_window, title.toStdString().c_str());
    Logger::log("Core", "Window title set.");
}

void Core::init()
{
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
    m_texloader.loadTextures(m_iout);
    Logger::log("Core", "Finished texture loading.");

    Logger::log("Core", "Spawned object.");
}

int Core::exec()
{
    m_quit = false;

    m_sceneparser.readScene(&m_scene, "menu.json");
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

        if(GameEvents::isMouseOver( &m_scene.getRect(0) ) )
            if(GameEvents::isMouseDown(1))
                Logger::log("Core", "Clicking mario!");

        draw_objs();

        SDL_RenderPresent(m_iout);
    }
    return 0;
}

//void Core::loadTextures()
//{
//    QFile tlist(RES_ROOT"texture.list");
//    tlist.open(QIODevice::ReadOnly);
//    if(!tlist.isOpen()){
//        Logger::err("Core", "Can't find 'data/texture.list'.");
//        exit(-1);
//    }
//    QTextStream in(&tlist);

//    QString line;
//    QStringList parts;

//    while(!in.atEnd())
//    {
//        line = in.readLine();
//        parts = line.split(":");

//        if(parts.count() > 2)
//        {
//            Logger::err("Core", "Incorrect structure format in 'data/texture.list'.");
//            exit(-1);
//        }
//        m_texloader.addTex(parts.at(1).trimmed(), parts.at(0).trimmed(), m_iout);
//    }
//    tlist.close();
//}

void Core::draw_objs()
{
    for(size_t i=0; i < m_scene.objs().size(); i++)
    {
        SDL_RenderCopy(m_iout, m_texloader.getTex( m_scene.objs().at(i).getTex(0) ), NULL, &m_scene.objs().at(i).rect);
    }
}
