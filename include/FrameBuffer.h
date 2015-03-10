#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "Texture.h"
#include "Debug.h"

class FrameBuffer
{
private:
    Texture *colorTexture, *depthTexture;
    GLuint object;

    FrameBuffer(const FrameBuffer&) {}

public:

    static const float screenMesh[12];

    FrameBuffer(int width, int height);
    ~FrameBuffer();

    void Bind() const;
    void UnBind() const;

    Texture* GetDepthTexture() const { return depthTexture; }
    Texture* GetColorTexture() const { return colorTexture; }
    GLuint GetObject() const { return object; }
};

#endif // FRAMEBUFFER_H
