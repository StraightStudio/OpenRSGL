#ifndef CONFIG_H
#define CONFIG_H

// -----------=
#define DEBUG
// -----------=

#define CONSOLE_KEY SDL_SCANCODE_GRAVE

#define DW_WIDTH 1280
#define DW_HEIGHT 720
#define DW_VERSION "0.1b"

#include <include/depends.h>
#include <include/logger.h>
#include <include/actor2d.h>

#define RES_ROOT "data/"
#define IMG_ROOT RES_ROOT"img/"
#define AUDIO_ROOT RES_ROOT"audio/"
#define SCENE_ROOT RES_ROOT"scenes/"
#define MODELS_ROOT RES_ROOT"models/"

class Actor2d;

struct Animation2d {
    int fps, frameCount;
    QList<SDL_Rect> frames;

    Animation2d(int afc, int afps)
    {
        frameCount = afc;
        fps = afps;
    }

    Animation2d(int afps)
    {
        fps = afps;
    }

    Animation2d(){}

    void addFrame(SDL_Rect r)
    {
        frames.append(r);
    }

    void setFrames(QList<SDL_Rect> fr)
    {
        frames.clear();
        frames = fr;
    }
};

struct AppConfig
{
    unistring app_name;
    unistring app_author;
    unistring app_version;
    int app_width;
    int app_height;
    bool is_full;
    unistring playername;
    map<unistring, Animation2d> app_animations;
    map<unistring, unistring> app_textures;
    map<unistring, int> anim_fps;

    unistring start_scene;
    map<unistring, Actor2d> app_models;
    map<unistring, unistring> sound_files;
    map<unistring, unistring> music_files;

    AppConfig(){}

    void setName(unistring n){ app_name = n;}
    void setAuthor(unistring a){ app_author = a;}
    void setVersion(unistring v){ app_version = v;}
    void setDimension(int w, int h){ app_width = w; app_height = h;}
    void setStartScene(unistring scene) {start_scene = scene;}

    unistring name() { return app_name; }
    unistring author() { return app_author; }
    unistring version() { return app_version; }

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
    static void loadCfg(AppConfig *conf);
    static void cfgwarn(unistring warnmsg);
};
#endif // CONFIG_H
