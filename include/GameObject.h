#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <map>

#include "StateManager.h"
#include "Component.h"
#include "Material.h"
#include "Mesh.h"
#include "glm/glm.hpp"

namespace RE
{

class GameObject
{

friend class Scene;

private:

    static int globalId;

    int id;
    typedef std::map<std::string, Component*> ComponentMap;
    ComponentMap components;

public:

    GameObject();
    virtual ~GameObject();

    void AddComponent(Component &component) const;
    void RemoveComponent(Component &component) const;

    int GetId() const;
};

}

#endif // GAMEOBJECT_H
