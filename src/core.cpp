#include <core.h>

Core* Core::shared_instance = NULL;

Core::Core()
{
    shared_instance = this;
}

Core::~Core()
{
    cleanup();
}

void Core::cleanup()
{
    Logger::log("Core", "Started cleanup...");

    if(m_appconf != nullptr)
        delete m_appconf;

    if(m_mloader != nullptr)
        delete m_mloader;
    if(m_tloader != nullptr)
        delete m_tloader;

    if(m_event != nullptr)
        delete m_event;
    if(m_processor != nullptr)
        delete m_processor;

    if(m_netmgr != nullptr)
        delete m_netmgr;

    if(m_renderer != nullptr)
        delete m_renderer;
    if(m_camera != nullptr)
        delete m_camera;
    if(m_gamescene != nullptr)
        delete m_gamescene;

    if(m_audiomgr != nullptr)
        delete m_audiomgr;

    SteamAPI_Shutdown();
}

void Core::init(bool server)
{
    m_appconf = new AppConfig;
    m_appconf->app_width = 1280;
    m_appconf->app_height = 720;
    //Config::loadCfg(m_appconf);
#ifdef STEAM
    if(!SteamAPI_IsSteamRunning())
        Logger::warn("Core", "SteamAPI init error!");
    else
        SteamAPI_Init();

    pname = unistring( SteamFriends()->GetPersonaName() );
    Logger::info("SteamAPI info", "Hello, "+pname+" :D !" );
#endif
    //
    m_renderer = new Renderer;
    m_renderer->init(m_appconf->app_width, m_appconf->app_height);

    m_event     = new SDL_Event;
    m_processor = new GameEvents;

    m_camera = new Camera(90.f, (float)m_appconf->app_width/(float)m_appconf->app_height); //

    m_audiomgr = new AudioManager; // INIT BEFORE CREATING OBJECTS!!!
    m_gamescene = new Scene3d;
    m_mloader = new MdlLoader;
    m_tloader = new TexLoader;

    loadModels();
    loadAudio();

    m_netmgr = new NetworkManager(server);
}

int Core::exec()
{
    m_quit = false;
    while(!m_quit)
    {
        m_netmgr->process();

        processEvents();
        m_camera->audioUpdate();
        m_renderer->render(m_camera, m_gamescene);
    }
    return 0;
}

void Core::loadTextures()
{
    m_tloader->loadCubemap({IMG_ROOT "right.png", IMG_ROOT "left.png", IMG_ROOT "top.png", IMG_ROOT "bottom.png", IMG_ROOT "front.png", IMG_ROOT "back.png"}, 0);
    for(auto& ti : m_appconf->app_textures)
        m_tloader->loadTexture(ti.second, ti.first);
}


void Core::loadModels()
{
    for(auto& mi : m_appconf->app_models)
    {
        m_mloader->loadModel(mi.second, mi.first);
    }
    //
    GLuint ma, ta, sz;
    m_mloader->loadModel(RES_ROOT"cube.obj", "cube");
    m_tloader->loadTexture(IMG_ROOT"uv.png", "uv");

    m_tloader->getTextureInfo("uv", &ta);
    m_mloader->getModelInfo("cube", &ma, &sz);

    m_gamescene->addObject(ma, sz, ta, "Cubic");
}

void Core::loadAudio()
{
    m_audiomgr->loadSounds(m_appconf);
    m_audiomgr->loadMusic(m_appconf);
}

void Core::processEvents()
{
    glm::vec3 dir;
    while(SDL_PollEvent(m_event))
    {
        switch (m_event->type) {
            case SDL_QUIT:
                m_quit = true;
            break;
            case SDL_MOUSEBUTTONDOWN:
                if(m_event->button.button > 0)
                {
                    if(m_event->button.button == SDL_BUTTON_MIDDLE)
                        m_renderer->grabMouse();

                    m_processor->button_clicked[m_event->button.button-1] = true;
                    m_processor->button_down[m_event->button.button-1] = true;
                }
            break;
            case SDL_MOUSEBUTTONUP:
                if(m_event->button.button > 0)
                {
                    //
                    if(m_event->button.button == SDL_BUTTON_MIDDLE)
                        m_renderer->releaseMouse();
                    //
                    m_processor->button_down[m_event->button.button-1] = false;
                }
            break;
            case SDL_MOUSEWHEEL:
                if(m_event->wheel.y < 0)
                    m_camera->move(0.f, +1.0f, 0.f);
                else if(m_event->wheel.y > 0)
                    m_camera->move(0.f, -1.0f, 0.f);
            break;
            case SDL_MOUSEMOTION:
                if(m_processor->isMouseDown(SDL_BUTTON_MIDDLE))
                {
                    if(m_event->motion.xrel > 0)
                        m_camera->rotate(glm::vec3(0,1,0), +1.f);
                    else if(m_event->motion.xrel < 0)
                        m_camera->rotate(glm::vec3(0,1,0), -1.f);
                }
            break;
            default:
            break;
        }
    }

    // End of event processing
    if(m_processor->isMouseClicked(SDL_BUTTON_LEFT))
        m_processor->button_clicked[0] = false;
    if(m_processor->isMouseClicked(SDL_BUTTON_MIDDLE))
        m_processor->button_clicked[1] = false;
    if(m_processor->isMouseClicked(SDL_BUTTON_RIGHT))
        m_processor->button_clicked[2] = false;
}

void Core::connect(unistring ip, uint16_t port)
{
    if(m_netmgr)
        m_netmgr->connect(ip, port);
}

void Core::onClientJoin(unistring addr)
{
    if(m_netmgr->isClient())
    {
        m_audiomgr->playSound("onJoin", m_audiomgr->zero());
    }
}

void Core::onServerJoin(unistring addr)
{
    Logger::log("Core", "IP "+addr+" connected");
}

void Core::onDisconnect(unistring addr)
{
    Logger::log("Core", "IP "+addr+" disconnected");
}

void Core::netUpdate()
{

}

GLuint Core::getCubemap(int id)
{
    return m_tloader->cubemap(id);
}

Core *Core::sharedPtr()
{
    return shared_instance;
}
