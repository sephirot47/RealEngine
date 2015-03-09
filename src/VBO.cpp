#include "../include/VBO.h"

VBO::VBO(GLenum target)
{
    glGenBuffers(1, &object);
    this->target = target;
}

VBO::~VBO()
{
    glDeleteBuffers(1, &object);
}

void VBO::SetData(const void *data, int size) const
{
    glBindBuffer(target, object);
    glBufferData(target, size, data, GL_STATIC_DRAW);
    glBindBuffer(target, 0);
}

void VBO::Bind() const
{
    glBindBuffer(target, object);
}

void VBO::UnBind() const
{
    glBindBuffer(target, 0);
}
