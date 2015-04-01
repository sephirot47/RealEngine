#include "../include/GameObject.h"

using namespace RE;


int GameObject::globalId = 0;


GameObject::GameObject()
{
    id = globalId++;
}

GameObject::~GameObject()
{

}

void GameObject::AddComponent(Component &component) const
{

}

void GameObject::RemoveComponent(Component &component) const
{

}

int GameObject::GetId() const
{
    return id;
}
