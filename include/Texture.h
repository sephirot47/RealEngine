#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "Image.h"
#include "Debug.h"


class Texture {
private:
    GLuint object;

public:
    Texture();
    ~Texture();

    void SetWrapMode(GLenum mode) const;
    void SetScaleMode(GLenum mode) const;
    void SetData(const void *data, int width, int height, GLenum format) const;
    void LoadFromImage(const Image &img) const;

    void Bind() const;
    void UnBind() const;

    void Bind(GLuint slot) const;
    void UnBind(GLuint slot) const;
};

#endif // TEXTURE_H