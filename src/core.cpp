#include <core.h>

Core::Core()
{
}

Core::~Core()
{
    cleanup();
}

void Core::cleanup()
{
    Logger::log("Core", "Started cleanup...");

    m_audiomgr.clear();

    if(m_gamescene != nullptr)
        delete m_gamescene;

    SteamAPI_Shutdown();
    IMG_Quit();
    SDL_Quit();
}

void Core::init()
{
    //Config::loadCfg(m_appconf);
    pname = m_appconf.playername;
#ifdef STEAM
    if(!SteamAPI_IsSteamRunning())
        Logger::warn("Core", "SteamAPI init error!");
    else
        SteamAPI_Init();

    pname = unistring( SteamFriends()->GetPersonaName() );
    Logger::info("SteamAPI info", "Hello, "+pname+" :D !" );
#endif
    //
    m_appconf.app_width = 1280;
    m_appconf.app_height = 720;

    m_renderer.init(m_appconf.app_width, m_appconf.app_height);
    m_camera = Camera(90.f, (float)m_appconf.app_width/(float)m_appconf.app_height);

    m_gamescene = new Scene3d();
    loadModels();
}

int Core::exec()
{
    m_quit = false;

    while(!m_quit)
    {
        while(SDL_PollEvent(&m_event))
        {
            switch (m_event.type) {
                case SDL_QUIT:
                    m_quit = true;
                break;
                case SDL_MOUSEBUTTONDOWN:
                    if(m_event.button.button > 0)
                    {
                        if(m_event.button.button == SDL_BUTTON_MIDDLE)
                            m_renderer.grabMouse();

                        m_processor.button_clicked[m_event.button.button-1] = true;
                        m_processor.button_down[m_event.button.button-1] = true;
                    }
                break;
                case SDL_MOUSEBUTTONUP:
                    if(m_event.button.button > 0)
                    {
                        //
                        if(m_event.button.button == SDL_BUTTON_MIDDLE)
                            m_renderer.releaseMouse();
                        //
                        m_processor.button_down[m_event.button.button-1] = false;
                    }
                break;
                case SDL_MOUSEWHEEL:
                    if(m_event.wheel.y < 0)
                        m_camera.move(0.f, +1.0f, 0.f);
                    else if(m_event.wheel.y > 0)
                        m_camera.move(0.f, -1.0f, 0.f);
                break;
                case SDL_MOUSEMOTION:
                    if(m_processor.isMouseDown(SDL_BUTTON_MIDDLE))
                    {
                        if(m_event.motion.xrel > 0)
                            m_camera.rotate(glm::vec3(0,1,0), +10.f);
                        else if(m_event.motion.xrel < 0)
                            m_camera.rotate(glm::vec3(0,1,0), -10.f);
                    }
                break;
                default:
                break;
            }
        }

        processEvents();
        m_renderer.render(m_camera, m_gamescene);
    }
    return 0;
}


void Core::loadModels()
{
    m_gamescene->addObject("christiansborg.obj", IMG_ROOT "BigTextureChristiansborg.png");
    m_gamescene->addObject("cube.obj", IMG_ROOT "yellow.png");
    m_gamescene->obj(1)->move(glm::vec3(1.f, 0.f, 0.f));
    //m_gamescene->addObject("christiansborg.obj", "BigTextureChristiansborg.png");
}

void Core::processEvents()
{
    glm::vec3 dir;
    if(m_processor.keyDown(SDL_SCANCODE_LEFT))
        dir.x = -1.f;
    else if(m_processor.keyDown(SDL_SCANCODE_RIGHT))
        dir.x = +1.f;

    if(m_processor.keyDown(SDL_SCANCODE_UP))
        dir.z = +1.f;
    else if(m_processor.keyDown(SDL_SCANCODE_DOWN))
        dir.z = -1.f;

    if(dir.length() != 0)
        m_camera.rtsmove(dir);


    if(m_processor.isMouseClicked(SDL_BUTTON_LEFT))
            m_processor.button_clicked[0] = false;
    if(m_processor.isMouseClicked(SDL_BUTTON_RIGHT))
        m_processor.button_clicked[2] = false;
}
