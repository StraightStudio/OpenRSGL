#ifndef SCENE2D_H
#define SCENE2D_H

#include <include/depends.h>
#include <include/actor2d.h>
#include <include/texloader.h>

class Scene2d
{
public:
    Scene2d();
    void draw(SDL_Renderer* rend);
    void spawn(vec2 pos, vec2 dim, QString textureAlias, TexLoader *tl);
    void spawn(vec2 pos, QString textureAlias, TexLoader *tl);
private:
    vector<Actor2d> m_objs;
};

#endif // SCENE2D_H
