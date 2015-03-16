#ifndef GBUFFER_H
#define GBUFFER_H

#include <vector>
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>

#include "stb_image.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "FrameBuffer.h"
#include "Texture.h"
#include "Debug.h"
#include "glm/glm.hpp"

class GBuffer : public FrameBuffer
{
private:

    std::string colorName, posName, uvName, normalsName, depthName;

    static const float screenMesh[12];
    static const std::string vshaderSource;

    enum
    {
        ColorAttachment = GL_COLOR_ATTACHMENT0,
        PositionAttachment = GL_COLOR_ATTACHMENT1,
        UvAttachment = GL_COLOR_ATTACHMENT2,
        NormalsAttachment = GL_COLOR_ATTACHMENT3,
        DepthAttachment = GL_COLOR_ATTACHMENT4
    };

    ShaderProgram *program;

public:

    GBuffer(int width, int height);
    ~GBuffer();

    void SetColorTextureName(std::string name);
    void SetPositionTextureName(std::string name);
    void SetNormalsTextureName(std::string name);
    void SetUvTextureName(std::string name);
    void SetDepthTextureName(std::string name);

    Texture *GetColorTexture();
    Texture *GetPositionTexture();
    Texture *GetNormalsTexture();
    Texture *GetUvTexture();
};

#endif // GBUFFER_H
