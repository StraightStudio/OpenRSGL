#ifndef CAMERA_H
#define CAMERA_H

#define DEFAULT_CAM_HEIGHT 1.5f
#define DEFAULT_CAM_PITCH 60.f
#define DEFAULT_SCROLL_SPEED 0.1f

#include <depends.h>

class Camera
{
public:
    Camera();
    Camera(float fov, float aspect);
    ~Camera();

    void move(float mx=0.f, float my=0.f, float mz=0.f);
    void move(glm::vec3 mv);

    void setPos(glm::vec3 newpos);

    void rtsmove(glm::vec3 direction);

    void rotate(glm::vec3 axis, float angle);

    void audioUpdate();
    glm::vec3 &fwdVector();

    glm::vec3 &getPos();

    glm::mat4 &matrix();

private:
    glm::vec3 pos, rot;

    glm::vec3 cam_front, cam_up;

    float m_fov, m_aspect, m_speed;
    glm::mat4 ProjectionMatrix, RotationMatrix, PositionMatrix;

    ALfloat* audio_position;
    ALfloat* audio_direction;
};

#endif // CAMERA_H
