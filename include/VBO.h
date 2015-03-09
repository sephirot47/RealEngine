#ifndef VBO_H
#define VBO_H

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

class VBO
{
private:
    VBO(VBO &vbo) {}
    VBO& operator=(const VBO &rhs) { return *this; }
    GLuint object;
    GLenum target;

public:
    VBO(GLuint target);
    ~VBO();

    void SetData(const void *data, int size) const;
    void Bind() const;
    void UnBind() const;

    GLuint GetObject() const { return object; }
    GLuint GetTarget() const { return target; }
};

#endif // VBO_H
