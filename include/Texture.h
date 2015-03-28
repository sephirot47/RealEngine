#ifndef TEXTURE_H
#define TEXTURE_H
#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "StateManager.h"
#include "RenderTarget.h"
#include "Debug.h"
#include "Image.h"

namespace RE
{

class Texture
{

friend class ShaderProgram;

private:

    GLuint object;
    mutable GLuint framebuffer;

    void Bind() const;
    void UnBind() const;
    void Bind(GLuint slot) const;
    static void UnBind(GLuint slot);

public:
    Texture();
    ~Texture();

    void LoadFromFile(const std::string filepath) const;
    void SetData(const void *data, int width, int height, GLint format, GLenum type, GLint internalFormat) const;

    void SetWrapMode(GLenum mode) const;
    void SetScaleMode(GLenum mode) const;

    void BindRenderTarget() const;
    void UnBindRenderTarget() const;

    GLuint GetObject() const { return object; }
};

}

#endif // TEXTURE_H
