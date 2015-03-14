#ifndef TEXTURE_H
#define TEXTURE_H
#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "FrameBuffer.h"
#include "Debug.h"

class Texture
{
private:
    FrameBuffer *framebuffer;
    GLuint object;

    void Bind() const;
    void UnBind() const;


public:
    Texture();
    ~Texture();

    void SetWrapMode(GLenum mode) const;
    void SetScaleMode(GLenum mode) const;
    void SetData(const void *data, int width, int height, GLenum format, GLenum internalFormat, GLenum type) const;

    void BindFrameBuffer(GLenum target);
    void UnBindFrameBuffer() const;

    void Bind(GLuint slot) const;
    static void UnBind(GLuint slot);

    GLuint GetObject() const { return object; }
};

#endif // TEXTURE_H
