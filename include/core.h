#ifndef CORE_H
#define CORE_H

#include <depends.h>
#include <logger.h>
#include <audiomanager.h>
#include <camera.h>
#include <loader3d.h>
#include <object3d.h>
#include <scene3d.h>
#include <shader.h>
#include <mdlloader.h>
#include <texloader.h>

class GameEvents;
#include <gameevents.h>

class NetworkManager;
#include <networkmanager.h>

class Renderer;
#include <renderer.h>

class Core
{
public:
    Core();
    ~Core();
    void init(bool server);
    void cleanup();
    int exec();

    void draw_objs3D();

    void loadTextures();
    void loadModels();
    void loadAudio();

    void processEvents();

    void initChess();

    void connect(unistring ip, uint16_t port);
    //
    void onClientJoin(unistring addr);
    void onServerJoin(unistring addr);
    void onDisconnect(unistring addr);

    void netUpdate();
    //
    GLuint getCubemap(int id);

    static Core *sharedPtr();
private:
    SDL_Event* m_event;
    bool m_quit;

    GameEvents* m_processor;
    AppConfig* m_appconf;

    Camera* m_camera;
    Renderer* m_renderer;
    Scene3d* m_gamescene;

    AudioManager* m_audiomgr;
    NetworkManager* m_netmgr;

    MdlLoader* m_mloader;
    TexLoader* m_tloader;

    static Core* shared_instance; // Engine-only var
};

#endif // CORE_H
