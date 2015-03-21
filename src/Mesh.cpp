#include "../include/Mesh.h"

Mesh::Mesh()
{
    numVertices = 0;
    model = mat4(1.0f);
}

Mesh::~Mesh()
{

}

void Mesh::LoadFromFile(const char *filepath)
{
    std::vector<glm::vec3> pos, normals;
    std::vector<glm::vec2> uv;
    bool triangles;

    FileLoader::ReadOBJ(filepath, pos, uv, normals, triangles);

    drawingMode = triangles ? GL_TRIANGLES : GL_QUADS;

    numVertices = pos.size();

    vao = new VAO();
    int index = 0;
    if(pos.size() > 0)
    {
        vboPos = new VBO();
        vboPos->SetData(&pos[0], pos.size() * sizeof(vec3));
        vao->AddAttribute(*vboPos, index++, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    if(uv.size() > 0)
    {
        vboUv = new VBO();
        vboUv->SetData(&uv[0], uv.size() * sizeof(vec2));
        vao->AddAttribute(*vboUv, index++, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    if(normals.size() > 0)
    {
        vboNormals = new VBO();
        vboNormals->SetData(&normals[0], normals.size() * sizeof(vec3));
        vao->AddAttribute(*vboNormals, index++, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
}

void Mesh::Draw(mat4 &projection, mat4 &view)
{
    vao->Bind();
    program->Use();

    program->SetUniform("projection", projection);
    program->SetUniform("view", view);
    program->SetUniform("model", model);
    program->SetUniform("normalMatrix", normalMatrix);
    glDrawArrays(drawingMode, 0, numVertices);

    program->UnUse();
    vao->UnBind();
}

void Mesh::SetDrawingMode(GLenum drawingMode)
{
    this->drawingMode = drawingMode;
}

void Mesh::SetModelMatrix(mat4 modelMatrix)
{
    this->model = modelMatrix;
}

void Mesh::SetNormalMatrix(mat4 normalMatrix)
{
    this->model = model;
}


void Mesh::SetShaderProgram(ShaderProgram &shaderProgram)
{
    program = &shaderProgram;
}


int Mesh::GetNumVertices() const
{
    return numVertices;
}

VAO* Mesh::GetVAO() const
{
    return vao;
}

VBO* Mesh::GetVBOPos() const
{
    return vboPos;
}

VBO* Mesh::GetVBOUv() const
{
    return vboUv;
}

VBO* Mesh::GetVBONormals() const
{
    return vboNormals;
}

Shader* Mesh::GetFragmentShader() const
{
    return fshader;
}

Shader* Mesh::GetVertexShader() const
{
    return vshader;
}

ShaderProgram* Mesh::GetShaderProgram() const
{
    return program;
}

GLenum Mesh::GetDrawingMode() const
{
    return drawingMode;
}

mat4 Mesh::GetModelMatrix() const
{
    return model;
}

mat4 Mesh::GetNormalMatrix() const
{
    return normalMatrix;
}
