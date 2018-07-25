#ifndef CAMERA_H
#define CAMERA_H

#define DEFAULT_CAM_HEIGHT 10.f
#define DEFAULT_CAM_PITCH 60.f
#define DEFAULT_SCROLL_SPEED 0.5f

#include <depends.h>
#include <vec3.h>

class Camera
{
public:
    Camera();
    Camera(float fov, float aspect);

    void move(float mx=0.f, float my=0.f, float mz=0.f);
    void move(glm::vec3 mv);

    void rtsmove(glm::vec3 direction);

    void rotate(float rx=0.f, float ry=0.f, float rz=0.f);
    glm::mat4x4 &matrix();


    glm::vec3 pos, rot;

    glm::vec3 cam_front, cam_up;

    float m_fov, m_aspect, m_speed;
    glm::mat4x4 ProjectionMatrix;
};

#endif // CAMERA_H
