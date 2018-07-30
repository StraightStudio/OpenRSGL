#include <scene3d.h>

Scene3d::Scene3d()
{

}

Scene3d::~Scene3d()
{
    for(Object3d* fo : m_objects)
        free(fo);
}

const vector<Object3d*> &Scene3d::objs() const
{
    return m_objects;
}

Object3d *Scene3d::obj(int i)
{
    return m_objects[i];
}

void Scene3d::addObject(GLuint addr, uint sz, GLuint tid)
{
    Object3d* tmp = new Object3d();

    tmp->setModel(addr, sz);
    tmp->setTex(tid);

    m_objects.push_back(tmp);
}
