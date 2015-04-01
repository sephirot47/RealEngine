#ifndef CUBETEXTURE_H
#define CUBETEXTURE_H

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <vector>

#include "StateManager.h"
#include "RenderTarget.h"
#include "Texture.h"
#include "Debug.h"
#include "Image.h"

namespace RE
{

class CubeTexture
{

friend class ShaderProgram;

public:

    enum Face
    {
        PositiveX = 0,
        NegativeX,
        PositiveY,
        NegativeY,
        PositiveZ,
        NegativeZ
    };

private:

    GLuint object;

    void Bind() const;
    void UnBind() const;
    void Bind(GLuint slot) const;
    static void UnBind(GLuint slot);

    std::vector<Texture*> faceTextures;

public:

    CubeTexture();
    ~CubeTexture();

    void SetFaceTexture(Face face, Image &image);

    void SetWrapMode(GLenum mode);
    void SetScaleMode(GLenum mode);

    Texture *GetFaceTexture(Face face);
    GLuint GetObject() const { return object; }
};

}

#endif // CUBETEXTURE_H
