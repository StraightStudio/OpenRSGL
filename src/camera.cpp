#include <camera.h>

Camera::Camera() :
    pos(glm::vec3(0,DEFAULT_CAM_HEIGHT,0)),
    rot(glm::vec3(-DEFAULT_CAM_PITCH,0,0)),
    m_fov(90.f),
    m_aspect(1.6),
    m_speed(DEFAULT_SCROLL_SPEED),

    cam_front(glm::vec3(0,0,-1)),
    cam_up(glm::vec3(0,1,0))
{

}


Camera::Camera(float fov, float aspect) :
    pos(glm::vec3(0,DEFAULT_CAM_HEIGHT,0)),
    rot(glm::vec3(-DEFAULT_CAM_PITCH,0,0)),
    m_fov(fov),
    m_aspect(aspect),
    m_speed(DEFAULT_SCROLL_SPEED),

    cam_front(glm::vec3(0,0,-1)),
    cam_up(glm::vec3(0,1,0))
{

}

void Camera::move(float mx, float my, float mz)
{
    pos += glm::vec3(mx, my, mz)*m_speed;
}

void Camera::move(glm::vec3 mv)
{
    pos += mv*m_speed;
}

void Camera::rtsmove(glm::vec3 direction)
{
    if(direction.x == 1.f)
        move(+glm::normalize(glm::cross(cam_front, cam_up)));
    else if(direction.x == -1.f)
        move(-glm::normalize(glm::cross(cam_front, cam_up)));

    if(direction.z == 1.f)
        move(+cam_front.x, +0.f, +cam_front.z);
    else if(direction.z == -1.f)
        move(-cam_front.x, -0.f, -cam_front.z);
}

void Camera::rotate(float rx, float ry, float rz)
{

}

glm::mat4x4 &Camera::matrix()
{
    ProjectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect, 0.1f, 1000.0f);

    cam_front.x = cos(glm::radians(rot.y)) * cos(glm::radians(rot.x));
    cam_front.y = sin(glm::radians(rot.x));
    cam_front.z = sin(glm::radians(rot.y)) * cos(glm::radians(rot.x));

    cam_front = glm::normalize(cam_front);

    ProjectionMatrix *= glm::lookAt( pos, pos+cam_front, cam_up);
    return ProjectionMatrix;
}
