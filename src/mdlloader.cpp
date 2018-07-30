#include <mdlloader.h>

MdlLoader::MdlLoader()
{

}

MdlLoader::~MdlLoader()
{
    for(pair<unistring, GLuint> vao : m_VAOs)
        glDeleteVertexArrays(1, &vao.second);
    for(pair<unistring, GLuint> buff : m_buffers)
        glDeleteBuffers(1, &buff.second);

    vdata.clear();
    m_VAOs.clear();
    m_buffers.clear();
}

void MdlLoader::loadModel(unistring mdl, unistring alias)
{
    m_loader.LoadModel(mdl, &vdata);

    if(m_VAOs[alias] == 0)
        glGenVertexArrays(1, &m_VAOs[alias]);
    if(m_buffers[alias] == 0)
        glGenBuffers(1, &m_buffers[alias]);

    GLfloat data[vdata.size()];
    for(int i=0; i < vdata.size(); i++)
        data[i] = vdata.at(i);

    glBindVertexArray(m_VAOs[alias]);
        glBindBuffer(GL_ARRAY_BUFFER, m_buffers[alias]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    glBindVertexArray(0);

    m_sizes[alias] = vdata.size()/5;
}

void MdlLoader::getModelInfo(unistring alias, GLuint *addr, uint *size)
{
    if( (m_VAOs.find(alias) == m_VAOs.end()) ||
        (m_buffers.find(alias) == m_buffers.end()))
    {
        fprintf(stdout, "ModelInfo won't be retrieved.\n");
        fflush(stdout);
        return;
    }

    *addr = m_VAOs.at(alias);
    *size = m_sizes.at(alias);
}
