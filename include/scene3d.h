#ifndef SCENE3D_H
#define SCENE3D_H

#include <depends.h>
#include <object3d.h>
#include <loader3d.h>

class Scene3d
{
public:
    Scene3d();
    ~Scene3d();
    const map<unistring, Object3d*> &objs() const;
    Object3d* obj(unistring alias);

    void select(unistring i){ m_objects[i]->select(true); }
    void deselect(unistring i){ m_objects[i]->select(false); }

    void addObject(GLuint addr, uint sz, GLuint tid, unistring alias);
private:
    map<unistring, Object3d*> m_objects;
    Loader3D m_loader3d;
};

#endif // SCENE3D_H
