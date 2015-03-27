#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "StateManager.h"
#include "Material.h"
#include "Mesh.h"
#include "glm/glm.hpp"

using namespace glm;

class GameObject
{

friend class Scene;

private:

    int id;

    Mesh *mesh;
    Material *material;
    vec3 position, rotation, scale;

public:

    GameObject();
    virtual ~GameObject();

    Mesh *GetMesh() const;
    Material *GetMaterial() const;
    int GetId() const;
};

#endif // GAMEOBJECT_H
