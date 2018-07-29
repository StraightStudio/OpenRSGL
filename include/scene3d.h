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
    const vector<Object3d *> &objs() const;
    Object3d* obj(int i);

    void addObject(unistring mdl, unistring texture);
private:
    vector<Object3d*> m_objects;
    Loader3D m_loader3d;
};

#endif // SCENE3D_H
