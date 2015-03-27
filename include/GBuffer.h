#ifndef GBUFFER_H
#define GBUFFER_H

#include <vector>
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>

#include "StateManager.h"
#include "stb_image.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "FrameBuffer.h"
#include "Texture.h"
#include "Debug.h"
#include "VAO.h"
#include "VBO.h"
#include "glm/glm.hpp"

enum
{
    FinalColorAttachment = GL_COLOR_ATTACHMENT0,
    TextureColorAttachment = GL_COLOR_ATTACHMENT1,
    PositionAttachment = GL_COLOR_ATTACHMENT2,
    UvAttachment = GL_COLOR_ATTACHMENT3,
    NormalsAttachment = GL_COLOR_ATTACHMENT4,
    DepthAttachment = GL_DEPTH_ATTACHMENT
};

class GBuffer : public FrameBuffer
{
private:

    static const float screenMesh[12];

    VAO *screenMeshVao;
    VBO *screenMeshVbo;
    ShaderProgram *program;
    Shader *vshader, *fshader;
    float width, height;

public:

    GBuffer(float width, float height);
    ~GBuffer();

    void Draw() const;
    void DrawToScreen() const;

    void SetVertexColorLocation(int location, VBO &vbo);
    void SetVertexPositonLocation(int location, VBO &vbo);
    void SetVertexUvLocation(int location, VBO &vbo);
    void SetVertexNormalsLocation(int location, VBO &vbo);

    void SetFragmentFinalColorTextureName(std::string name);
    void SetFragmentTextureColorTextureName(std::string name);
    void SetFragmentPositionTextureName(std::string name);
    void SetFragmentUvTextureName(std::string name);
    void SetFragmentNormalsTextureName(std::string name);
    void SetFragmentDepthTextureName(std::string name);

    VAO *GetVAO() const;
    ShaderProgram *GetShaderProgram() const;
    Texture *GetFinalColorTexture() const;
    Texture *GetTextureColorTexture() const;
    Texture *GetPositionTexture() const;
    Texture *GetNormalsTexture() const;
    Texture *GetUvTexture() const;
    Texture *GetDepthTexture() const;

    float GetWidth() const;
    float GetHeight() const;
};

#endif // GBUFFER_H
