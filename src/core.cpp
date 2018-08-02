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

    if(m_gamescene != nullptr)
        delete m_gamescene;

    if(m_audiomgr != nullptr)
        delete m_audiomgr;

    SteamAPI_Shutdown();
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
    m_camera = Camera(90.f, (float)m_appconf.app_width/(float)m_appconf.app_height); //

    m_audiomgr = new AudioManager; // INIT BEFORE CREATING OBJECTS!!!

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
        m_camera.audioUpdate();
        m_renderer.render(m_camera, m_gamescene);
    }
    return 0;
}


void Core::loadModels()
{
    m_mloader.loadModel(RES_ROOT "cube.obj", "chessbox");
    m_tloader.loadTexture(IMG_ROOT "yellow.png", "yellow");
    m_tloader.loadTexture(IMG_ROOT "uv.png", "uv");

    GLuint maddr, taddr;
    uint sz;

    for(pair<unistring, unistring> item : mdl_list)
        m_mloader.loadModel(item.second, item.first);

    // ------- C H E S S   S P A W N S =====================

    m_tloader.getTextureInfo("uv", &taddr);
    m_mloader.getModelInfo("chessbox", &maddr, &sz);
    m_gamescene->addObject(maddr, sz, taddr, "chessboard");

    // Add figures for 'black' player
    m_tloader.getTextureInfo("yellow", &taddr);
    m_mloader.getModelInfo(PAWN_NAME, &maddr, &sz);
    unistring nm;
    for(int i=0; i < 8; i++)
    {
        nm = unistring(PAWN_NAME "_b")+to_string(i);

        m_gamescene->addObject(maddr, sz, taddr, nm);
        m_gamescene->obj(nm)->move( (DIR_RIGHT*(singleSquare*i))+glm::vec3(-singleSquare*2.5f, 0.18f, -singleSquare*3.5f) );
    }
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


    if(m_processor.keyDown(SDL_SCANCODE_W))
        m_renderer.lpos.x += 0.1f;
    else if(m_processor.keyDown(SDL_SCANCODE_W))
        m_renderer.lpos.x -= 0.1f;
    if(m_processor.keyDown(SDL_SCANCODE_A))
        m_renderer.lpos.z += 0.1f;
    else if(m_processor.keyDown(SDL_SCANCODE_D))
        m_renderer.lpos.z -= 0.1f;


    // ADD SLOW EFFECT ON KEY UP

    if(dir.length() != 0)
    {
        m_camera.rtsmove(dir);
    }

    if(m_processor.isMouseClicked(SDL_BUTTON_LEFT))
        m_processor.button_clicked[0] = false;
    if(m_processor.isMouseClicked(SDL_BUTTON_MIDDLE))
        m_processor.button_clicked[1] = false;
    if(m_processor.isMouseClicked(SDL_BUTTON_RIGHT))
        m_processor.button_clicked[2] = false;
}
