#include "../include/Quaternion.h"

using namespace RE;

Quaternion::Quaternion() : glm::quat()
{
}

Quaternion::Quaternion(glm::quat q) : glm::quat(q)
{
}

glm::vec3 Quaternion::GetForward() const { return (*this) * glm::vec3(0, 0, -1); }
glm::vec3 Quaternion::GetUp() const { return (*this) * glm::vec3(0, 1, 0); }
glm::vec3 Quaternion::GetRight() const { return (*this) * glm::vec3(1, 0, 0); }

Quaternion Quaternion::Normalized() const
{
    float l = this->length();
    Quaternion q(*this);
    q.x /= l;
    q.y /= l;
    q.z /= l;
    q.w /= l;
    return q;
}

Quaternion Quaternion::AxisAngle(const glm::vec3 &axis, float degrees)
{
    return angleAxis(degrees, axis);
}

Quaternion Quaternion::FromAxes(const glm::vec3& xAxis, const glm::vec3& yAxis, const glm::vec3& zAxis)
{
    glm::mat3 kRot;
    kRot[0] = xAxis;
    kRot[1] = yAxis;
    kRot[2] = zAxis;
    return Quaternion(glm::quat_cast(kRot));
}

Quaternion Quaternion::FromTo(const glm::vec3 &from, const glm::vec3 &to)
{
    Quaternion q;
    glm::vec3 a = cross(from, to);
    q.x = a.x; q.y = a.y; q.z = a.z;
    float d = dot(from, to);
    if(glm::abs(d) > 0.99999f) return glm::quat(); //return identity(do nothing)
    q.w = sqrt(from.length() * from.length() * to.length() * to.length()) + d;
    return q.Normalized();
}
