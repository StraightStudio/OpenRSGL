#ifndef SCENE2D_H
#define SCENE2D_H

#include <include/depends.h>
#include <include/actor2d.h>
#include <include/texloader.h>
#include <include/audiomanager.h>
#include <include/config.h>

struct SceneInfo
{
  QString name;
  QString author;
  QString bg_track;
  QString type;

  SceneInfo(QVariant n, QVariant a): name(n.toString()), author(a.toString()) {}

  void setName(QVariant n)
  {
      name = n.toString();
  }

  void setAuthor(QVariant a)
  {
      author = a.toString();
  }
  void setBgTrack(QVariant a)
  {
      bg_track = a.toString();
  }
  void setType(QVariant t)
  {
      type = t.toString();
  }

  void reset()
  {
      name.clear();
      author.clear();
      bg_track.clear();
      type.clear();
  }
};

class Scene2d
{
public:
    Scene2d();
    void start(AudioManager *mgr);

    void draw(SDL_Renderer* rend=0);
    void spawn(vec2 pos=vec2(0,0), vec2 dim=vec2(0,0), QString textureAlias="none", TexLoader *tl=0);
    void spawn(vec2 pos=vec2(0,0), QString textureAlias="none", TexLoader *tl=0);

    SDL_Rect &getRect(QString name="");
    QMap<QString, Actor2d> &objs();

    void clear();

    SceneInfo sinfo;
    void addActor(AppConfig &conf, vec2 pos, QString model, QString parent);
    void addActor(vec2 pos, vec2 dim, vec2 rdim, QMap<QString, QString> trigger, QString name, QString type, QString texture, QString anim_idle);
private:
    QMap<QString, Actor2d> m_objs;
    QString name;
    QString author;
    int objid;
};

#endif // SCENE2D_H
