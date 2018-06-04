#ifndef CONFIG_H
#define CONFIG_H

#define DW_WIDTH 1280
#define DW_HEIGHT 720

#include <include/depends.h>
#include <include/logger.h>

#define RES_ROOT "data/"
#define IMG_ROOT RES_ROOT"img/"
#define AUDIO_ROOT RES_ROOT"audio/"
#define SCENE_ROOT RES_ROOT"scenes/"

struct AppConfig
{
    QString app_name;
    QString app_author;
    QString app_version;
    int app_width;
    int app_height;
    bool is_full;
    QMap<QString, QString> app_textures;

    QString start_scene;
    QMap<QString, QString> app_scenes;
    QStringList background_audio_list;

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
};

class Config
{
public:
    static void cfgerr(QString errmsg);
    static void loadCfg(AppConfig *conf);
};
#endif // CONFIG_H
