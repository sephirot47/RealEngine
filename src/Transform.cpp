#include "../include/Transform.h"

using namespace RE;

Transform::Transform()
{
    position = glm::vec3(0);
    scale = glm::vec3(1);
}

Transform::~Transform()
{
}

glm::mat4 Transform::GetModelMatrix()
{
    glm::mat4 identity;
    glm::mat4 T = glm::translate(identity, position);
    glm::mat4 R = glm::mat4_cast(rotation);
    glm::mat4 S = glm::scale(identity, scale);
    return T * R * S;
}

