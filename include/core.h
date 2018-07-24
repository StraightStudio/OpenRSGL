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
#include <vec2.h>


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

    void initialSpawn();

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

    Actor2d obj; // TMP object


    bool m_console;
    unistring m_cinput;
    TTF_Font *m_consolefont;
    SDL_Rect m_consolerect;
    SDL_Color m_consolecolour;

    SDL_Rect m_consoletextrect;
    SDL_Texture* m_consoletext;

	unsigned long int m_citem;
    vector<unistring> m_consoleHistory;

    // ================ L O G I C   V A R I A B L E S ================
    // etc...
    unistring pname;
    SDL_Rect m_camrect;

    SDL_GLContext m_glcontext;
    GLuint m_shaderprogram;
    GLuint m_VAO; // Vertex Arrays Object
    GLuint m_VBO; // Vertex Buffer Object
    GLuint m_vsh; // VertexShader (Model)
    GLuint m_fsh; // FragmentShader (Texture, Color)
    float m_fov;

    // S T E A M   A P I

};

#endif // CORE_H
