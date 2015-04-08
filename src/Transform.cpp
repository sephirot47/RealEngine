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

void Transform::SetPosition(glm::vec3 position)
{
    this->position = position;
}

void Transform::SetScale(glm::vec3 scale)
{
    this->scale = scale;
}

void Transform::SetRotation(Quaternion rotation)
{
    this->rotation = rotation;
}

glm::vec3 Transform::GetPosition() const
{
    return position;
}

glm::vec3 Transform::GetScale() const
{
    return scale;
}

Quaternion Transform::GetRotation() const
{
    return rotation;
}

glm::mat4 Transform::GetModelMatrix() const
{
    glm::mat4 identity;
    glm::mat4 T = glm::translate(identity, position);
    glm::mat4 R = glm::mat4_cast(rotation);
    glm::mat4 S = glm::scale(identity, scale);
    return T * R * S;
}

