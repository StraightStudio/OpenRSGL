#ifndef SCENEPARSER_H
#define SCENEPARSER_H

#ifdef _WIN64
#include <depends.h>
#include <scene2d.h>
#else
#include <include/depends.h>
#include <include/scene2d.h>
#endif

class SceneParser
{
public:
    SceneParser();
    void readScene(AppConfig &conf, Scene2d &target, unistring file);
    void loadScene(Scene2d &target, AppConfig &conf);
    void loadScene(Scene2d &target, unistring file, AppConfig &conf);
};

#endif // SCENEPARSER_H
