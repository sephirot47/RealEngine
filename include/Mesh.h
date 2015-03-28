#ifndef MESH_H
#define MESH_H

#include "StateManager.h"
#include "RenderTarget.h"
#include "FileLoader.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "VAO.h"
#include "VBO.h"
#include "Material.h"

namespace RE
{

class Mesh
{
private:

    Mesh(const Mesh& m){}

    VAO *vao;
    VBO *vboPos, *vboUv, *vboNormals;
    GLenum renderMode;
    glm::mat4 model, normalMatrix;

    int numVertices;

public:
    Mesh();
    ~Mesh();

    void LoadFromFile(const char *filepath);

    void Render(const Material &material, glm::mat4 &projection, glm::mat4 &view);
    void Render(RenderTarget &renderTarget, const Material &material, glm::mat4 &projection, glm::mat4 &view);

    void SetRenderMode(GLenum renderMode);
    void SetNormalMatrix(glm::mat4 normalMatrix);
    void SetModelMatrix(glm::mat4 modelMatrix);

    int GetNumVertices() const;
    VAO* GetVAO() const;
    VBO* GetVBOPos() const;
    VBO* GetVBOUv() const;
    VBO* GetVBONormals() const;
    GLenum GetRenderMode() const;
    glm::mat4 GetModelMatrix() const;
    glm::mat4 GetNormalMatrix() const;
};

}

#endif // MESH_H
