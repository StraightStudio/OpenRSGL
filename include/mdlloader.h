#ifndef MDLLOADER_H
#define MDLLOADER_H

#include <depends.h>
#include <loader3d.h>

class MdlLoader
{
public:
    MdlLoader();
    ~MdlLoader();

    void loadModel(unistring mdl, unistring alias);

    void getModelInfo(unistring alias, GLuint* addr, uint* size);
private:
    Loader3D m_loader;

    vector<GLfloat> vdata;
    map<unistring, GLuint> m_VAOs;
    map<unistring, GLuint> m_buffers;
    map<unistring, uint> m_sizes;
};

#endif // MDLLOADER_H
