#include "../include/Mesh.h"

using namespace RE;


Mesh::Mesh()
{
    numVertices = 0;
    model = glm::mat4(1.0f);
    vao = nullptr;
    vboPos = vboUv = vboNormals = nullptr;
}

Mesh::~Mesh()
{
    if(vao) delete vao;
    if(vboPos) delete vboPos;
    if(vboUv) delete vboUv;
    if(vboNormals) delete vboNormals;
}

void Mesh::LoadFromFile(const char *filepath)
{
    StateManager::Push();

    if(vao) delete vao;
    if(vboPos) delete vboPos;
    if(vboUv) delete vboUv;
    if(vboNormals) delete vboNormals;

    std::vector<glm::vec3> pos, normals;
    std::vector<glm::vec2> uv;
    bool triangles;

    FileLoader::ReadOBJ(filepath, pos, uv, normals, triangles);

    renderMode = triangles ? GL_TRIANGLES : GL_QUADS;
    numVertices = pos.size();

    vao = new VAO();
    int index = 0;
    if(pos.size() > 0)
    {
        vboPos = new VBO();
        vboPos->SetData(&pos[0], pos.size() * sizeof(glm::vec3));
        vao->AddAttribute(*vboPos, index++, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    if(uv.size() > 0)
    {
        vboUv = new VBO();
        vboUv->SetData(&uv[0], uv.size() * sizeof(glm::vec2));
        vao->AddAttribute(*vboUv, index++, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    if(normals.size() > 0)
    {
        vboNormals = new VBO();
        vboNormals->SetData(&normals[0], normals.size() * sizeof(glm::vec3));
        vao->AddAttribute(*vboNormals, index++, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    StateManager::Pop();
}

void Mesh::Render(RenderTarget &renderTarget, const Material &material, glm::mat4 &projection, glm::mat4 &view)
{
    StateManager::Push();

    renderTarget.BindRenderTarget();
    Render(material, projection, view);
    renderTarget.UnBindRenderTarget();

    StateManager::Pop();
}

void Mesh::Render(const Material &material, glm::mat4 &projection, glm::mat4 &view)
{
    if(not vao) return;

    StateManager::Push();

    vao->Bind();
    material.Bind();

    material.GetShaderProgram()->SetUniform("projection", projection);
    material.GetShaderProgram()->SetUniform("view", view);
    material.GetShaderProgram()->SetUniform("model", model);
    material.GetShaderProgram()->SetUniform("normalMatrix", glm::transpose(glm::inverse(view * model)));
    material.GetShaderProgram()->SetUniform("PVM", projection * view * model);


    glDrawArrays(renderMode, 0, numVertices);

    material.UnBind();
    vao->UnBind();

    StateManager::Pop();
}

void Mesh::SetRenderMode(GLenum renderMode)
{
    this->renderMode = renderMode;
}

void Mesh::SetModelMatrix(glm::mat4 modelMatrix)
{
    this->model = modelMatrix;
}

void Mesh::SetNormalMatrix(glm::mat4 normalMatrix)
{
    this->normalMatrix = normalMatrix;
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

GLenum Mesh::GetRenderMode() const
{
    return renderMode;
}

glm::mat4 Mesh::GetModelMatrix() const
{
    return model;
}

glm::mat4 Mesh::GetNormalMatrix() const
{
    return normalMatrix;
}
