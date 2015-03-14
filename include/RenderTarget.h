#ifndef RENDERTARGET_H
#define RENDERTARGET_H
#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "FrameBuffer.h"
#include "Debug.h"

class RenderTarget
{
private:

    FrameBuffer *framebuffer;

protected:

    RenderTarget() { framebuffer = new FrameBuffer(); }
    virtual ~RenderTarget() { if(framebuffer) { delete framebuffer; framebuffer = 0; } }

public:

    void BindFrameBuffer() const;
    void UnBindFrameBuffer() const;
};

#endif // RENDERTARGET_H
