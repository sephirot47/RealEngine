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

class Texture : public RenderTarget
{

friend class ShaderProgram;
friend class CubeMap; //Para que pueda llamar a SetDataForCubeMap

public:
    enum Target
    {
        Texture2D = GL_TEXTURE_2D,
        TextureCubeMap = GL_TEXTURE_CUBE_MAP
    };

private:

    GLuint object;
    mutable Target target;
    mutable GLuint framebuffer;

    int width, height;

    void Bind() const;
    void UnBind() const;
    void Bind(GLuint slot) const;
    void UnBind(GLuint slot) const;

    void Init();

    //Igual que SetData, pero sin hacer bind y unbind de this, Y ESPECIFICANDO LA TARGET DE LA FACE.
    void SetDataForCubeMap(GLenum faceTarget, const void *data, int width, int height,
                           GLint format, GLenum type, GLint internalFormat);

public:

    Texture();
    Texture(Target target);
    Texture(const std::string filepath, Target target = Target::Texture2D);
    Texture(int width, int height, Target target = Target::Texture2D);
    ~Texture();

    void LoadFromFile(const std::string filepath);
    void CreateEmpty(int width, int height);
    void CreateEmpty(int width, int height, GLint format, GLenum type, GLint internalFormat);
    void SetData(const void *data, int width, int height, GLint format, GLenum type, GLint internalFormat);

    void SetWrapMode(GLenum mode);
    void SetScaleMode(GLenum mode);

    void BindRenderTarget() const;
    void UnBindRenderTarget() const;

    GLuint GetObject() const { return object; }
    Target GetTarget() const { return target; }
};

}

#endif // TEXTURE_H
