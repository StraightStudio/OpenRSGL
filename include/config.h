#ifndef CONFIG_H
#define CONFIG_H

// -----------=
#define DEBUG
// -----------=

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
    QString app_name;
    QString app_author;
    QString app_version;
    int app_width;
    int app_height;
    bool is_full;
    QString playername;
    QMap<QString, Animation2d> app_animations;
    QMap<QString, QString> app_textures;
    QMap<QString, int> anim_fps;

    QString start_scene;
    QMap<QString, Actor2d> app_models;
    QMap<QString, QString> sound_files;
    QMap<QString, QString> music_files;

    AppConfig(){}

    void setName(QVariant n){ app_name = n.toString();}
    void setAuthor(QVariant a){ app_author = a.toString();}
    void setVersion(QVariant v){ app_version = v.toString();}
    void setDimension(int w, int h){ app_width = w; app_height = h;}
    void setStartScene(QVariant scene) {start_scene = scene.toString();}

    const QString &name() const { return app_name; }
    const QString &author() const { return app_author; }
    const QString &version() const { return app_version; }

    const QString &getStartScene() const { return start_scene; }

    QStringList getSoundAliases();
    QStringList getSoundFiles();
    QStringList getMusicAliases();
    QStringList getMusicFiles();
};

class Config
{
public:
    static void cfgerr(QString errmsg);
    static void loadCfg(AppConfig *conf);
    static void cfgwarn(QString warnmsg);
};
#endif // CONFIG_H
