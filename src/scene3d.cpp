#include <scene3d.h>

Scene3d::Scene3d()
{

}

Scene3d::~Scene3d()
{
    for(auto &fo : m_objects)
        delete fo.second;
}

const map<unistring, Object3d*> &Scene3d::objs() const
{
    return m_objects;
}

Object3d *Scene3d::obj(unistring alias)
{
    return m_objects[alias];
}

//void Scene3d::addObject(GLuint addr, uint sz, GLuint tid)
//{
//    Object3d* tmp = new Object3d();

//    tmp->setModel(addr, sz);
//    tmp->setTex(tid);

//    m_objects.push_back(tmp);
//}

void Scene3d::addObject(GLuint addr, uint sz, GLuint tid, unistring alias)
{
    Object3d* tmp = new Object3d();

    tmp->setModel(addr, sz);
    tmp->setTex(tid);

    m_objects[alias] = tmp;
}
