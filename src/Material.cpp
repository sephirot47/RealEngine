#include "../include/Material.h"

using namespace RE;


Material::Material() : Component("Material")
{
    shininess = 32.0f;
    texture = nullptr;

    vshader = new Shader(); vshader->Create("Assets/Shaders/Material/material.vert", GL_VERTEX_SHADER);
    fshader = new Shader(); fshader->Create("Assets/Shaders/Material/material.frag", GL_FRAGMENT_SHADER);
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

void Material::Bind() const
{
    program->Bind();
    program->SetUniform("material.shininess", shininess);
    if(texture) program->AttachTexture("material.texture", *texture);
}

void Material::UnBind() const
{
    program->UnBind();
}

void Material::SetTexture(Texture &t)
{
    this->texture = &t;
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
