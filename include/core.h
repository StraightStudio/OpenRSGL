#ifndef CORE_H
#define CORE_H

#include <logger.h>
#include <depends.h>
#include <gameevents.h>
#include <audiomanager.h>
#include <camera.h>
#include <loader3d.h>
#include <object3d.h>

class Core
{
public:
    Core();
    ~Core();
    void init();
    void cleanup();
    void spawn();
    int exec();
    void loadTextures();

    void draw_objs3D();
    void initGL();
    void initShaders(unistring fvertex="", unistring ffragment="");

    void processEvents();

    //
    void updateConsole();

private:
    unistring current_scene;

    SDL_Window* m_window;
    SDL_Renderer* m_iout;
    SDL_Event m_event;
    bool m_quit;

    AudioManager m_audiomgr;
    GameEvents m_processor;

    AppConfig m_appconf;

    //
    SDL_Rect selection_rect;
    SDL_Rect mouse_rect;
    int spawnid;
    //

    SDL_Rect menu_rect;
    SDL_Rect map_rect;

    // ================ O P E N  G L   V A R I A B L E S ================
    // etc...
    unistring pname;
    SDL_Rect m_camrect;

    SDL_GLContext m_glcontext;
    GLuint m_shadeprog, m_vsh, m_fsh;


    Loader3D m_loader3d;
    Camera m_camera;

    Object3d test_obj;
};

#endif // CORE_H
