#include "../include/Material.h"


Material::Material()
{
    ambientColor = vec3(0.1, 0.1, 0.1);
    diffuseColor = vec3(0.5, 0.5, 0.5);
    specularColor = vec3(0.5, 0.5, 0.5);
    shininess = 60.0f;
    texture = nullptr;

    vshader = new Shader(); vshader->Create("Assets/Shaders/Mesh/default.vert", GL_VERTEX_SHADER);
    fshader = new Shader(); fshader->Create("Assets/Shaders/Mesh/default.frag", GL_FRAGMENT_SHADER);
    program = new ShaderProgram();
    program->AttachShader(*fshader);
    program->AttachShader(*vshader);
    program->Link();

    programChanged = false;
}

Material::~Material()
{
    if(vshader) delete vshader;
    if(fshader) delete fshader;
    if(program and not programChanged) delete program;
}

void Material::BindForDrawing() const
{
    if(not program) return;

    program->Use();
    program->SetUniform("matAmbientColor", ambientColor);
    program->SetUniform("matDiffuseColor", diffuseColor);
    program->SetUniform("matSpecularColor", specularColor);
    program->SetUniform("matShininess", shininess);

    if(texture)
        program->AttachTexture("matTexture", *texture);
}

void Material::UnBindForDrawing() const
{
    if(not program) return;
    program->UnUse();
}




void Material::SetAmbientColor(vec3 color)
{
    ambientColor = color;
}

void Material::SetDiffuseColor(vec3 color)
{
    diffuseColor = color;
}

void Material::SetSpecularColor(vec3 color)
{
    specularColor = color;
}

void Material::SetTexture(Texture &t)
{
    texture = &t;
}

void Material::SetShininess(float shininess)
{
    this->shininess = shininess;
}

void Material::SetShaderProgram(ShaderProgram &program)
{
    this->program = &program;
    programChanged = true;
}




vec3 Material::GetAmbientColor() const
{
    return ambientColor;
}

vec3 Material::GetDiffuseColor() const
{
    return diffuseColor;
}

vec3 Material::GetSpecularColor() const
{
    return specularColor;
}

Texture *Material::GetTexture() const
{
    return texture;
}

Shader *Material::GetVertexShader() const
{
    return vshader;
}

Shader *Material::GetFragmentShader() const
{
    return fshader;
}

ShaderProgram *Material::GetShaderProgram() const
{
    return program;
}

float Material::GetShininess() const
{
    return shininess;
}
