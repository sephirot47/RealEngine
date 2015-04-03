#ifndef QUATERNION_H
#define QUATERNION_H

#include "glm/glm.hpp"

namespace RE
{

class Quaternion : public glm::quat
{
public:
    Quaternion();
    Quaternion(glm::quat q);

    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;
    Quaternion Normalized() const;

    static Quaternion LookAt(const glm::vec3 eye, const glm::vec3 lookTo, const glm::vec3 up);
    static Quaternion AxisAngle(const glm::vec3 axis, float degrees);
    static Quaternion FromAxes(const glm::vec3 xAxis, const glm::vec3 yAxis, const glm::vec3 zAxis);
    static Quaternion FromTo(const glm::vec3 from, const glm::vec3 to);
};

}

#endif // QUATERNION_H
