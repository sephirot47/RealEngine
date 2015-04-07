#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

namespace RE
{

class Component
{

private:

    Component(const Component &comp) {}

protected:

    Component() {}
    virtual ~Component() {}
};

}

#endif // COMPONENT_H
