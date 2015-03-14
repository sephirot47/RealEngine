#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#define GL_GLEXT_PROTOTYPES

#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "Debug.h"

class FrameBuffer
{
private:

    std::vector<GLenum> drawBuffers;
    GLuint numBuffers, object;

    FrameBuffer(const FrameBuffer&) {}

public:

    FrameBuffer();
    ~FrameBuffer();

    void Bind() const;
    void UnBind() const;

    void SetDrawingBuffers(int size, GLenum *bufs);

    GLuint GetObject() const { return object; }
};

#endif // FRAMEBUFFER_H
