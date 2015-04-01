#ifndef CAMERA_H
#define CAMERA_H

#include "Component.h"
#include "ShaderProgram.h"
#include "Quaternion.h"
#include "StateManager.h"
#include "glm/glm.hpp"

namespace RE
{

class Camera : public Component
{
public:

    enum Mode
    {
        Ortho,
        Perspective
    };

private:

    glm::vec3 position;
    Quaternion rotation;
    glm::mat4 ortho, perspective;
    Mode mode;

public:

    Camera();
    ~Camera();

    void LookAt(const glm::vec3 position, const glm::vec3 lookTo, const glm::vec3 up);
    void LookAt(const glm::vec3 lookTo, const glm::vec3 up);

    void SetPosition(const glm::vec3 position);
    void SetRotation(const Quaternion rotation);

    void SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
    void SetOrtho(const glm::mat4 &ortho);
    void SetPerspective(float fovyDegrees, float aspect, float zNear, float zFar);
    void SetPerspective(const glm::mat4 &perspective);
    void SetMode(Mode mode);

    glm::vec3 GetPosition() const;
    Quaternion GetRotation() const;
    glm::mat4 GetView() const;

    glm::vec3 GetFront() const;
    glm::vec3 GetUp() const;
    glm::vec3 GetRight() const;

    glm::mat4 GetProjection() const;
    glm::mat4 GetOrtho() const;
    glm::mat4 GetPerspective() const;
};

}

#endif // CAMERA_H
