#include "../include/Camera.h"

using namespace RE;


Camera::Camera() : Component("Camera"),
                   position(glm::vec3(0)), rotation(glm::quat()),
                   ortho(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f)),
                   perspective(glm::perspective(45.0f * 3.1415f/180.0f, 1.0f, 0.1f, 150.0f)),
                   mode(CameraMode::Perspective)
{

}

Camera::~Camera()
{

}

glm::mat4 Camera::GetView() const
{
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 T = glm::translate(view, position);
    glm::mat4 R = glm::mat4_cast(rotation);
    return glm::inverse(T * R);
}

glm::mat4 Camera::GetProjection() const
{
    return mode == CameraMode::Ortho ? ortho : perspective;
}

void Camera::Bind(const ShaderProgram &program) const
{
    glm::mat4 view = GetView();
    program.SetUniform("view", view);
    if(mode == CameraMode::Ortho) program.SetUniform("projection", ortho);
    else program.SetUniform("projection", perspective);
}

void Camera::LookAt(const glm::vec3 position, const glm::vec3 lookTo, const glm::vec3 up)
{
    this->position = position;
    if(this->position == lookTo) { rotation = glm::quat(); return; }

    glm::mat4 m = glm::lookAt(position, lookTo, up);
    rotation = glm::quat_cast( glm::transpose(m) );
}

void Camera::LookAt(const glm::vec3 lookTo, const glm::vec3 up)
{
    if(position == lookTo) { rotation = glm::quat(); return; }

    glm::mat4 m = glm::lookAt(position, lookTo, up);
    rotation = glm::quat_cast( glm::transpose(m) );
}

void Camera::SetPosition(const glm::vec3 position)
{
    this->position = position;
}

void Camera::SetRotation(const glm::quat rotation)
{
    this->rotation = rotation;
}

void Camera::SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
    ortho = glm::ortho(left, right, bottom, top, zNear, zFar);
}

void Camera::SetOrtho(const glm::mat4 &ortho)
{
    this->ortho = ortho;
}

void Camera::SetPerspective(float fovyDegrees, float aspect, float zNear, float zFar)
{
    this->perspective = glm::perspective(fovyDegrees * 3.141592653589f/180.0f, aspect, zNear, zFar);
}

void Camera::SetPerspective(const glm::mat4 &perspective)
{
    this->perspective = perspective;
}

void Camera::SetMode(CameraMode mode)
{
    this->mode = mode;
}



glm::vec3 Camera::GetPosition() const
{
    return position;
}

glm::quat Camera::GetRotation() const
{
    return rotation;
}

glm::mat4 Camera::GetOrtho() const
{
    return ortho;
}

glm::mat4 Camera::GetPerspective() const
{
    return perspective;
}
