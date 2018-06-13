#ifndef CORE_H
#define CORE_H

#include <include/logger.h>

#include <include/depends.h>
#include <include/scene2d.h>
#include <include/texloader.h>
#include <include/sceneparser.h>
#include <include/gameevents.h>
#include <include/audiomanager.h>
#include <include/animationmanager.h>
#include <include/vec2.h>

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

    void initialSpawn();

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
    AnimationManager m_animator;

    //
    SDL_Rect selection_rect;
    SDL_Rect mouse_rect;
    int spawnid;
    //

    SDL_Rect menu_rect;
    SDL_Rect map_rect;

    // ================ L O G I C   V A R I A B L E S ================
    QString colony_name;
    QString cash_name;
    QString religion_name;
    QString commander_name;

    float popularity; // 0.00 - 1.00 ( 0-100% )


    int cash_balance;
    int wood_balance;
    int stone_balance;
    int iron_balance;
    // etc...
    QString pname;
    vec2 cam_pos; // Camera Position.
};

#endif // CORE_H
