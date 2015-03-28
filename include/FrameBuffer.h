#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#define GL_GLEXT_PROTOTYPES

#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "StateManager.h"
#include "Texture.h"
#include "Debug.h"

namespace RE
{

class FrameBuffer
{
private:

    std::vector<GLenum> drawBuffers;
    std::vector<Texture*> textures;
    GLuint numBuffers, object;

    FrameBuffer(const FrameBuffer&) {}

protected:
    int width, height;

public:

    FrameBuffer(int width, int height);
    virtual ~FrameBuffer();

    void Bind() const;
    void UnBind() const;

    void AddDrawingBuffer(GLenum attachment,
                          GLenum format,
                          GLenum type,
                          GLint internalFormat,
                          GLenum wrapMode = GL_CLAMP_TO_EDGE,
                          GLenum scaleMode = GL_LINEAR);

    bool ExistsDrawingBuffer(GLenum attachment);
    void DeleteDrawingBuffer(GLenum attachment);

    void ClearColor() const;
    void ClearDepth() const;
    void ClearColorDepth() const;

    Texture *GetTexture(GLenum attachment) const;
    GLuint GetObject() const { return object; }
};

}

#endif // FRAMEBUFFER_H
