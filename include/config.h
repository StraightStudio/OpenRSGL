
#ifndef CONFIG_H
#define CONFIG_H

// -----------=
#define DEBUG
// -----------=

#define CONSOLE_KEY SDL_SCANCODE_GRAVE

#define DW_WIDTH 1280
#define DW_HEIGHT 720
#define DW_FULL false
#define DW_NAME "OpenRSGL"
#define DW_VERSION "082018 prerelease"

#include <depends.h>
#include <logger.h>

#define RES_ROOT "data/"
#define IMG_ROOT RES_ROOT"img/"
#define AUDIO_ROOT RES_ROOT"audio/"
#define SCENE_ROOT RES_ROOT"scenes/"
#define MODELS_ROOT RES_ROOT"models/"

struct AppConfig
{
    AppConfig() :
        app_width(DW_WIDTH), app_height(DW_HEIGHT), is_full(DW_FULL),
        app_name("SDL_OGL_Engine" DW_VERSION), app_author("Dennis Softman")
    {

    }
    unistring app_name;
    unistring app_author;
    int app_width;
    int app_height;
    bool is_full;
    unistring playername;
    map<unistring, unistring> app_textures;
    map<unistring, int> anim_fps;

    unistring start_scene;
    map<unistring, unistring> sound_files;
    map<unistring, unistring> music_files;
    unistrlist resource_files;

    void setName(unistring n){ app_name = n;}
    void setAuthor(unistring a){ app_author = a;}
    void setDimension(int w, int h){ app_width = w; app_height = h;}
    void setStartScene(unistring scene) {start_scene = scene;}

    unistring name() { return app_name; }
    unistring author() { return app_author; }

    unistring getStartScene() { return start_scene; }

    vector<unistring> getSoundAliases();
    vector<unistring> getSoundFiles();
    vector<unistring> getMusicAliases();
    vector<unistring> getMusicFiles();
};

class Config
{
public:
    static void cfgerr(unistring errmsg);
    static void loadCfg(AppConfig &conf);
    static void cfgwarn(unistring warnmsg);
};
#endif // CONFIG_H

