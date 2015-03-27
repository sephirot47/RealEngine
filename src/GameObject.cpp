#include "../include/GameObject.h"

GameObject::GameObject()
{
    mesh = new Mesh();
    mesh->LoadFromFile("Assets/Models/sphere.obj");
    material = new Material();
    position = rotation = vec3(0);
    scale = vec3(1);
}

GameObject::~GameObject()
{
    if(mesh) delete mesh;
    if(material) delete material;
}

Mesh *GameObject::GetMesh() const
{
    return mesh;
}

Material *GameObject::GetMaterial() const
{
    return material;
}

int GameObject::GetId() const
{
    return id;
}
