#ifndef SCENE2D_H
#define SCENE2D_H

#include <include/depends.h>
#include <include/actor2d.h>
#include <include/texloader.h>

struct SceneInfo
{
  QString name;
  QString author;

  SceneInfo(QVariant n, QVariant a): name(n.toString()), author(a.toString()) {}

  void setName(QVariant n)
  {
      name = n.toString();
  }

  void setAuthor(QVariant a)
  {
      author = a.toString();
  }
};

class Scene2d
{
public:
    Scene2d();
    void draw(SDL_Renderer* rend=0);
    void spawn(vec2 pos=vec2(0,0), vec2 dim=vec2(0,0), QString textureAlias="none", TexLoader *tl=0);
    void spawn(vec2 pos=vec2(0,0), QString textureAlias="none", TexLoader *tl=0);

    void addActor(vec2 pos, vec2 dim, QStringList textures, QString name, QString type, QMap<QString, QString> trigger);
    SDL_Rect &getRect(QString name="");
    QMap<QString, Actor2d> &objs();

    SceneInfo sinfo;
private:
    QMap<QString, Actor2d> m_objs;
    QString name;
    QString author;
};

#endif // SCENE2D_H
