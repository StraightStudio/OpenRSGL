#include <camera.h>

Camera::Camera()
{
    Camera(90.f, 1.6f);
}


Camera::Camera(float fov, float aspect) :
    pos(glm::vec3(-.5f,DEFAULT_CAM_HEIGHT,0)),
    rot(glm::vec3(-DEFAULT_CAM_PITCH,0,0)),
    m_fov(fov),
    m_aspect(aspect),
    m_speed(DEFAULT_SCROLL_SPEED),

    cam_front(glm::vec3(0,0,-1)),
    cam_up(glm::vec3(0,1,0))
{
    audio_position = (ALfloat*)calloc(3, sizeof(ALfloat)); // Memory for position (x,y,z)
    audio_direction = (ALfloat*)calloc(3, sizeof(ALfloat)); // Memory for direction (x,y,z)
}

Camera::~Camera()
{
    if(audio_position != nullptr)
        free(audio_position);
    if(audio_direction != nullptr)
        free(audio_direction);
}

void Camera::move(float mx, float my, float mz)
{
    move(glm::vec3(mx, my, mz));
}

void Camera::move(glm::vec3 mv)
{
    pos += mv*m_speed;
}

void Camera::setPos(glm::vec3 newpos)
{
    pos = newpos;
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

void Camera::rotate(glm::vec3 axis, float angle)
{
    rot += glm::normalize(axis) * angle;

    RotationMatrix = glm::rotate(glm::mat4(), rot.x, glm::vec3(1,0,0)) * glm::rotate(glm::mat4(), rot.y, glm::vec3(0,1,0)) * glm::rotate(glm::mat4(), rot.z, glm::vec3(0,0,1));
}

void Camera::audioUpdate()
{
    audio_position[0] = pos.x; audio_position[1] = pos.y; audio_position[2] = pos.z;
    audio_direction[0] = 0; audio_direction[1] = 0; audio_direction[2] = 0;

    alListenerfv(AL_POSITION, audio_position);
    alListenerfv(AL_ORIENTATION, audio_direction);
}

glm::vec3 &Camera::fwdVector()
{
    return cam_front;
}

glm::vec3 &Camera::getPos()
{
    return pos;
}

glm::mat4x4 &Camera::matrix()
{
    ProjectionMatrix = glm::perspective(glm::radians(m_fov), m_aspect, 0.1f, 1000.0f);

    cam_front.x = cos(glm::radians(rot.y)) * cos(glm::radians(rot.x));
    cam_front.y = sin(glm::radians(rot.x));
    cam_front.z = sin(glm::radians(rot.y)) * cos(glm::radians(rot.x));
    cam_front = glm::normalize(cam_front);

    ProjectionMatrix *= glm::lookAt(pos, pos+cam_front, cam_up);

    return ProjectionMatrix;
}
