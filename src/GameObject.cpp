#include "../include/GameObject.h"

using namespace RE;

int GameObject::globalId = 0;

GameObject::GameObject()
{
    id = globalId++;

    transform = new Transform();
    AddComponent(*transform);
    initialTransformChanged = false; //after!
}

GameObject::~GameObject()
{
    if(transform and not initialTransformChanged) delete transform;
}

Transform *GameObject::GetTransform() const
{
    return GetComponent<Transform>();
}

int GameObject::GetId() const
{
    return id;
}
