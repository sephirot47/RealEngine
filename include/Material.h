#ifndef MATERIAL_H
#define MATERIAL_H

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "Bindable.h"
#include "StateManager.h"
#include "FileLoader.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "glm/glm.hpp"

namespace RE
{

class Material : public Bindable
{
private:

    float shininess;

    Texture *texture;
    Shader *vshader, *fshader;
    ShaderProgram *program;

    bool programChanged;

public:

    Material();
    ~Material();

    void Bind() const;
    void UnBind() const;

    void SetTexture(Texture &t);
    void SetShininess(float shininess);
    void SetShaderProgram(ShaderProgram &program);

    float GetShininess() const;
    Texture* GetTexture() const;
    Shader* GetVertexShader() const;
    Shader* GetFragmentShader() const;
    ShaderProgram *GetShaderProgram() const;
};

}

#endif // MATERIAL_H
