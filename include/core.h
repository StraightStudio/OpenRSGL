#ifndef CORE_H
#define CORE_H

#include <logger.h>
#include <depends.h>
#include <gameevents.h>
#include <audiomanager.h>
#include <camera.h>
#include <loader3d.h>
#include <object3d.h>
#include <scene3d.h>
#include <shader.h>
#include <renderer.h>
#include <mdlloader.h>
#include <texloader.h>

class Core
{
public:
    Core();
    ~Core();
    void init();
    void cleanup();
    int exec();

    void draw_objs3D();

    void loadModels();

    void processEvents();

    //
    void updateConsole();

private:
    unistring current_scene;

    SDL_Event m_event;
    bool m_quit;

    AudioManager m_audiomgr;
    GameEvents m_processor;

    AppConfig m_appconf;

    // ================ O P E N  G L   V A R I A B L E S ================
    // etc...
    unistring pname;

    Camera m_camera;
    Renderer m_renderer;
    Scene3d *m_gamescene;

    MdlLoader m_mloader;
    TexLoader m_tloader;
};

#endif // CORE_H
