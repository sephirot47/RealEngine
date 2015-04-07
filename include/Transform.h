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
public:

    glm::vec3 position, scale;
    Quaternion rotation;

    Transform();
    ~Transform();
};

}

#endif // TRANSFORM_H
