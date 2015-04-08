#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"
#include "ShaderProgram.h"
#include "Quaternion.h"
#include "StateManager.h"
#include "glm/glm.hpp"

namespace RE
{

class Transform : public Component
{
private:

    glm::vec3 position, scale;
    Quaternion rotation;

public:

    Transform();
    ~Transform();

    void SetPosition(glm::vec3 position);
    void SetScale(glm::vec3 scale);
    void SetRotation(Quaternion rotation);

    glm::vec3 GetPosition() const;
    glm::vec3 GetScale() const;
    Quaternion GetRotation() const;

    glm::mat4 GetModelMatrix() const;
};

}

#endif // TRANSFORM_H
