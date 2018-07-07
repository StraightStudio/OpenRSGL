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

    void draw(SDL_Renderer* rend=nullptr);
    void spawn(vec2 pos=vec2(0,0), vec2 dim=vec2(0,0), unistring textureAlias="none", TexLoader *tl=nullptr);
    void spawn(vec2 pos=vec2(0,0), unistring textureAlias="none", TexLoader *tl=nullptr);

    SDL_Rect &getRect(unistring name="");

    void clear();

    SceneInfo sinfo;
    void addActor(AppConfig &conf, vec2 pos, unistring model_id, unistring parent);

    void addActor(vec2 pos, vec2 dim, vec2 rdim, map<unistring, unistring> trigger, unistring name, unistring type, unistring texture, unistring anim_idle);

    void addToQueue(unistring object);
    void doOperations(); // Add / delete actors.

    map<unistring, Actor2d> m_objs;
    unistrlist toDelete;
private:
    unistring name;
    unistring author;
    int objid;
};

#endif // SCENE2D_H
