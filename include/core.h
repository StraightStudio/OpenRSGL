#ifndef CORE_H
#define CORE_H

#include <logger.h>
#include <depends.h>
#include <scene2d.h>
#include <texloader.h>
#include <sceneparser.h>
#include <gameevents.h>
#include <audiomanager.h>
#include <animationmanager.h>
#include <vec3.h>
#include <camera.h>
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
    void draw_objs();

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
    // etc...
    unistring pname;
    SDL_Rect m_camrect;

    SDL_GLContext m_glcontext;
    GLuint m_modelview;
    GLuint m_shadeprog, m_vsh, m_fsh;


    Loader3D m_loader3d;
    Camera m_camera;

    Object3d test_obj;
};

#endif // CORE_H
