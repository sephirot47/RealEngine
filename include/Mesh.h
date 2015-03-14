#ifndef MESH_H
#define MESH_H

#include "FileLoader.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "RenderTarget.h"
#include "VAO.h"
#include "VBO.h"

class Mesh
{
private:

    Mesh(const Mesh& m){}

    VAO *vao;
    VBO *vboPos, *vboUv, *vboNormals;
    Shader *vshader, *fshader;
    ShaderProgram *program;
    GLenum drawingMode;

    int numVertices;

public:
    Mesh();
    ~Mesh();

    void LoadFromFile(const char *filepath);

    void Draw();
    void Draw(RenderTarget &rt);

    void SetDrawingMode(GLenum drawingMode);
    void SetShaderProgram(ShaderProgram &shaderProgram);

    const VAO* GetVAO() const;
    const VBO* GetVBOPos() const;
    const VBO* GetVBOUv() const;
    const VBO* GetVBONormals() const;
    const Shader* GetVertexShader() const;
    const Shader* GetFragmentShader() const;
    const ShaderProgram* GetShaderProgram() const;
    const GLenum* GetDrawingMode() const;
};

#endif // MESH_H
