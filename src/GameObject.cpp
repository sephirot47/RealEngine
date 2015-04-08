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

int GameObject::GetId() const
{
    return id;
}
