#ifndef SCENE2D_H
#define SCENE2D_H

#include <include/depends.h>
#include <include/actor2d.h>
#include <include/texloader.h>
#include <include/audiomanager.h>
#include <include/config.h>

struct SceneInfo
{
  unistring name;
  unistring author;
  unistring bg_track;
  unistring type;

  SceneInfo(unistring n, unistring a): name(n), author(a) {}

  void setName(unistring n)
  {
      name = n;
      boost::algorithm::to_lower(name);
  }

  void setAuthor(unistring a)
  {
      author = a;
      boost::algorithm::to_lower(author);
  }
  void setBgTrack(unistring a)
  {
      bg_track = a;
      boost::algorithm::to_lower(bg_track);
  }
  void setType(unistring t)
  {
      type = t;
      boost::algorithm::to_lower(type);
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
    void addActor(AppConfig &conf, vec2 pos, unistring model_id, unistring parent);
    void addActor(vec2 pos, vec2 dim, vec2 rdim, QMap<QString, QString> trigger, unistring name, unistring type, unistring texture, unistring anim_idle);
private:
    QMap<QString, Actor2d> m_objs;
    unistring name;
    unistring author;
    int objid;
};

#endif // SCENE2D_H
