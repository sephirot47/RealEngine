#ifndef CAMERA_H
#define CAMERA_H

#include "Component.h"
#include "ShaderProgram.h"
#include "StateManager.h"
#include "glm/glm.hpp"

namespace RE
{

class Camera : public Component
{
public:

    enum CameraMode
    {
        Ortho,
        Perspective
    };

private:

    glm::vec3 position;
    glm::quat rotation;
    glm::mat4 ortho, perspective;
    CameraMode mode;

public:

    Camera();
    ~Camera();

    void Bind(const ShaderProgram &program) const;

    void LookAt(const glm::vec3 position, const glm::vec3 lookTo, const glm::vec3 up);
    void LookAt(const glm::vec3 lookTo, const glm::vec3 up);

    void SetPosition(const glm::vec3 position);
    void SetRotation(const glm::quat rotation);

    void SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
    void SetOrtho(const glm::mat4 &ortho);
    void SetPerspective(float fovyDegrees, float aspect, float zNear, float zFar);
    void SetPerspective(const glm::mat4 &perspective);
    void SetMode(CameraMode mode);

    glm::vec3 GetPosition() const;
    glm::quat GetRotation() const;
    glm::mat4 GetView() const;

    glm::mat4 GetProjection() const;
    glm::mat4 GetOrtho() const;
    glm::mat4 GetPerspective() const;
};

}

#endif // CAMERA_H
