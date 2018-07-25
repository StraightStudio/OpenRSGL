#include <object3d.h>

Object3d::Object3d()
{

}

void Object3d::draw(GLuint shader_program)
{
    glUseProgram(shader_program);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glVertexAttribPointer(
       0,                  // Атрибут 0. Подробнее об этом будет рассказано в части, посвященной шейдерам.
       3,                  // Размер
       GL_FLOAT,           // Тип
       GL_FALSE,           // Указывает, что значения не нормализованы
       0,                  // Шаг
       (void*)0            // Смещение массива в буфере
    );

    // Вывести треугольник!
    glDrawArrays(GL_TRIANGLES, 0, m_size); // Начиная с вершины 0, всего 3 вершины -> один треугольник

    glDisableVertexAttribArray(0);
}

void Object3d::update(const vector<GLfloat> &verts, int draw_type)
{
    if(m_VAO == 0)
        glGenVertexArrays(1, &m_VAO);
    if(m_buffer == 0)
        glGenBuffers(1, &m_buffer);

    vertices = verts;

    m_size = vertices.size();
    GLfloat data[m_size];
    for(int i=0; i < m_size; i++)
        data[i] = verts.at(i);

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, draw_type);
}
