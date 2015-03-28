#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

namespace RE
{

class RenderTarget
{
protected:

    RenderTarget() {}
    ~RenderTarget() {}

protected:

    virtual void BindRenderTarget() const = 0;
    virtual void UnBindRenderTarget() const = 0;
};

}

#endif // RENDERTARGET_H
