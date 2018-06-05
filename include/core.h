#ifndef CORE_H
#define CORE_H

#include <include/logger.h>

#include <include/depends.h>
#include <include/scene2d.h>
#include <include/texloader.h>
#include <include/sceneparser.h>
#include <include/gameevents.h>
#include <include/audiomanager.h>

class Core
{
public:
    Core();
    ~Core();
    void init();
    void spawn();
    int exec();
    void loadTextures();
    void draw_objs();

    void processEvents();

private:
    QString current_scene;

    SDL_Window* m_window;
    SDL_Renderer* m_iout;
    SDL_Event m_event;
    Scene2d m_scene;
    bool m_quit;

    TexLoader m_texloader;
    AudioManager m_audiomgr;
    SceneParser m_sceneparser;
    GameEvents m_processor;

    AppConfig m_appconf;
};

#endif // CORE_H
