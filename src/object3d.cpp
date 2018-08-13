#include <object3d.h>

Object3d::Object3d() :
    m_scale(1.f),
    pos(glm::vec3(0,0,0)),
    rot(glm::vec3(0,0,0)),
    ModelMatrix(glm::mat4(1.f)),
    m_vao(0),
    selected(false),
    apos{0.0, 0.0, 0.0}
{
    alGenSources(1, &asrc);
    alSourcef(asrc, AL_PITCH, 1.0f);
    alSourcef(asrc, AL_GAIN, 1.0f);
    alSourcefv(asrc, AL_POSITION, apos);
    alSourcei(asrc, AL_LOOPING, AL_FALSE);
    alSourcei(asrc, AL_SOURCE_RELATIVE, AL_FALSE);
}

Object3d::~Object3d()
{

}

void Object3d::draw(Shader *shader)
{
    ModelMatrix = glm::mat4(1.f);
    ModelMatrix = glm::translate(ModelMatrix, pos);
    ModelMatrix = glm::rotate(ModelMatrix, rot.x, glm::vec3(1,0,0))*glm::rotate(ModelMatrix, rot.y, glm::vec3(0,1,0))*glm::rotate(ModelMatrix, rot.z, glm::vec3(0,0,1));
    ModelMatrix = glm::scale(ModelMatrix, glm::vec3(m_scale, m_scale, m_scale));
    shader->setMat4("Model", ModelMatrix);

    glBindVertexArray(m_vao);
        glActiveTexture(GL_TEXTURE0);           // 0 here  |
        glBindTexture(GL_TEXTURE_2D, m_texid);  //         | for this
        shader->setInt("mainTexture", 0);       // 0 there |
        glDrawArrays(GL_TRIANGLES, 0, m_size);
        glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void Object3d::setTex(GLuint texid)
{
    m_texid = texid;
}

void Object3d::move(glm::vec3 mv)
{
    pos += mv;

    apos[0] = pos.x;
    apos[1] = pos.y;
    apos[2] = pos.z;
    //ModelMatrix = glm::translate(ModelMatrix, pos);
}

void Object3d::rotate(glm::vec3 axis, float angle)
{
    rot += glm::normalize(axis) * glm::radians(angle);
    //ModelMatrix = glm::rotate(ModelMatrix, rot.x, glm::vec3(1,0,0))*glm::rotate(ModelMatrix, rot.y, glm::vec3(0,1,0))*glm::rotate(ModelMatrix, rot.z, glm::vec3(0,0,1));
}

void Object3d::scale(float factor)
{
    m_scale = factor;
    //ModelMatrix = glm::scale(ModelMatrix, glm::vec3(m_scale, m_scale, m_scale));
}

void Object3d::setPos(glm::vec3 newpos)
{
    pos = newpos;
}

void Object3d::select(bool yes)
{
    selected = yes;
}

bool Object3d::isSelected()
{
    return selected;
}

void Object3d::setModel(GLuint addr, uint sz)
{
    m_vao = addr;
    m_size = sz;
}

ALuint Object3d::audiosource()
{
    return asrc;
}
