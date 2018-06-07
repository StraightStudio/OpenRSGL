#ifndef SCENEPARSER_H
#define SCENEPARSER_H

#include <include/depends.h>
#include <include/scene2d.h>

class SceneParser
{
public:
    SceneParser();
    void readScene(AppConfig &conf, Scene2d *target, QString file);
    void loadScene(Scene2d *target, AppConfig &conf);
    void loadScene(Scene2d *target, QString file, AppConfig &conf);
};

#endif // SCENEPARSER_H
