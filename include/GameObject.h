#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <typeinfo>
#include <typeindex>
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

    struct ComponentPair
    {
        std::type_index type;
        Component *component;
        ComponentPair(Component* c) : type( std::type_index(typeid(*c)) ) { component = c; }
    };

    static int globalId;

    int id;
    std::vector<ComponentPair> components;

public:

    GameObject();
    virtual ~GameObject();

    template <class T> void AddComponent(const T &component)
    {
        components.push_back( ComponentPair( (Component*)&component ) );
    }

    template <class T> bool HasComponent() const
    {
        for(int i = 0; i < components.size(); ++i)
            if(std::type_index(typeid(T)) == components[i].type)
                return true;
        return false;
    }

    template <class T> T* GetComponent() const
    {
        for(int i = 0; i < components.size(); ++i)
            if(std::type_index(typeid(T)) == components[i].type)
                return (T*) components[i].component;

        return nullptr;
    }

    template <class T> void RemoveComponent()
    {
        for(int i = 0; i < components.size(); ++i)
            if(std::type_index(typeid(T)) == components[i].type) { components.erase( components.begin() + i ); }
    }

    int GetId() const;
};

}

#endif // GAMEOBJECT_H
