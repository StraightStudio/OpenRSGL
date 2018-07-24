#ifndef SCENEPARSER_H
#define SCENEPARSER_H

#include <depends.h>
#include <scene2d.h>

class SceneParser
{
public:
    SceneParser();
    void readScene(AppConfig &conf, Scene2d &target, unistring file);
    void loadScene(Scene2d &target, AppConfig &conf);
    void loadScene(Scene2d &target, unistring file, AppConfig &conf);
};

#endif // SCENEPARSER_H
