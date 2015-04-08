#ifndef MESH_H
#define MESH_H

#include "StateManager.h"
#include "RenderTarget.h"
#include "FileLoader.h"
#include "Component.h"
#include "Shader.h"
#include "Debug.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"
#include "Material.h"
#include "glm/glm.hpp"

namespace RE
{

class Mesh : public Component
{
private:

    Mesh(const Mesh& m) { }

    VAO *vao;
    VBO *vboPos, *vboUv, *vboNormals;
    GLenum renderMode;

    int numVertices;

public:
    Mesh();
    ~Mesh();

    void LoadFromFile(const char *filepath);
    void LoadPositionsFromArray(const std::vector<glm::vec3> &positions, GLenum renderMode = GL_TRIANGLES);

    void Render(const Material &material, const glm::mat4 &model, Camera &camera);
    void Render(const Material &material, const glm::mat4 &model, const glm::mat4 &camView, const glm::mat4 &camProjection);
    void Render(const ShaderProgram &program, const glm::mat4 &model, Camera &camera);
    void Render(const ShaderProgram &program, const glm::mat4 &model, const glm::mat4 &camView, const glm::mat4 &camProjection);
    void Render(RenderTarget &renderTarget, const Material &material, const glm::mat4 &model, Camera &camera);
    void Render(RenderTarget &renderTarget, const Material &material, const glm::mat4 &model,const glm::mat4 &camView, const glm::mat4 &camProjection);
    void Render(RenderTarget &renderTarget, const ShaderProgram &program, const glm::mat4 &model, Camera &camera);
    void Render(RenderTarget &renderTarget, const ShaderProgram &program, const glm::mat4 &model, const glm::mat4 &camView, const glm::mat4 &camProjection);

    void SetRenderMode(GLenum renderMode);

    int GetNumVertices() const;
    VAO* GetVAO() const;
    VBO* GetVBOPos() const;
    VBO* GetVBOUv() const;
    VBO* GetVBONormals() const;
    GLenum GetRenderMode() const;
};

}

#endif // MESH_H
