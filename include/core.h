#ifndef CORE_H
#define CORE_H

#include <include/logger.h>

#include <include/depends.h>
#include <include/scene2d.h>
#include <include/texloader.h>

class Core
{
public:
    Core();
    void setWindow(int width, int height, QString title);
    void init();
    void spawn();
    int exec();

private:
    QString current_scene;

    SDL_Window* m_window;
    SDL_Renderer* m_iout;
    SDL_Event m_event;
    Scene2d m_scene;
    bool m_quit;
    Logger m_log;
    TexLoader m_texloader;
};

#endif // CORE_H
