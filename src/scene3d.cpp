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

void Scene3d::addObject(unistring mdl, unistring texture)
{
    Object3d* tmp = new Object3d();

    m_loader3d.LoadModel(unistring(RES_ROOT)+mdl, tmp);
    tmp->update(texture);
    fprintf(stdout, "%p load status: %d size.\n", tmp, (int)tmp->vertexData.size());
    fflush(stdout);

    m_objects.push_back(tmp);
}
