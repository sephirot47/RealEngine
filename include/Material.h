#ifndef MATERIAL_H
#define MATERIAL_H

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "StateManager.h"
#include "FileLoader.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "glm/glm.hpp"

using namespace glm;

class Material
{
private:

    vec3 ambientColor, diffuseColor, specularColor;
    float shininess;

    Texture *texture;
    Shader *vshader, *fshader;
    ShaderProgram *program;

    bool programChanged;

public:

    Material();
    ~Material();

    void BindForDrawing() const;
    void UnBindForDrawing() const;

    void SetAmbientColor(vec3 color);
    void SetDiffuseColor(vec3 color);
    void SetSpecularColor(vec3 color);
    void SetTexture(Texture &t);
    void SetShininess(float shininess);
    void SetShaderProgram(ShaderProgram &program);

    vec3 GetAmbientColor() const;
    vec3 GetDiffuseColor() const;
    vec3 GetSpecularColor() const;
    float GetShininess() const;
    Texture* GetTexture() const;
    Shader* GetVertexShader() const;
    Shader* GetFragmentShader() const;
    ShaderProgram *GetShaderProgram() const;
};

#endif // MATERIAL_H
