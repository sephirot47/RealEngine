#ifndef MESH_H
#define MESH_H

#include "FileLoader.h"
#include "Shader.h"
#include "ShaderProgram.h"
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
    mat4 model, normalMatrix;

    int numVertices;

public:
    Mesh();
    ~Mesh();

    void LoadFromFile(const char *filepath);

    void Draw(mat4 &projection, mat4 &view);

    void SetDrawingMode(GLenum drawingMode);
    void SetNormalMatrix(mat4 normalMatrix);
    void SetModelMatrix(mat4 modelMatrix);
    void SetShaderProgram(ShaderProgram &shaderProgram);

    int GetNumVertices() const;
    VAO* GetVAO() const;
    VBO* GetVBOPos() const;
    VBO* GetVBOUv() const;
    VBO* GetVBONormals() const;
    Shader* GetVertexShader() const;
    Shader* GetFragmentShader() const;
    ShaderProgram* GetShaderProgram() const;
    GLenum GetDrawingMode() const;
    mat4 GetModelMatrix() const;
    mat4 GetNormalMatrix() const;
};

#endif // MESH_H
