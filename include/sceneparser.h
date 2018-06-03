#ifndef SCENEPARSER_H
#define SCENEPARSER_H

#include <include/depends.h>
#include <include/scene2d.h>

class SceneParser
{
public:
    SceneParser();
    void readScene(Scene2d *target, QString file);
};

#endif // SCENEPARSER_H
