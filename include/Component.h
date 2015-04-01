#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

namespace RE
{

class Component
{

private:
    const std::string name;

protected:

    Component(std::string name) : name(name) {}
    Component(const Component &comp) {}
    virtual ~Component() {}

public:

    std::string GetName() const { return name; }
};

}

#endif // COMPONENT_H
