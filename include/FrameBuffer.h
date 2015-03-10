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
    int width, height;
    GLuint object;

    FrameBuffer(const FrameBuffer&) {}

public:

    FrameBuffer(int width, int height);
    ~FrameBuffer();

    void Bind() const;
    void UnBind() const;

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    Texture* GetDepthTexture() const { return depthTexture; }
    Texture* GetColorTexture() const { return colorTexture; }
    GLuint GetObject() const { return object; }
};

#endif // FRAMEBUFFER_H
