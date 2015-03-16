#include "../include/GBuffer.h"

const float GBuffer::screenMesh[12] = {1.0f, -1.0f, 0.0f,
                                       1.0f,  1.0f, 0.0f,
                                      -1.0f,  1.0f, 0.0f,
                                      -1.0f, -1.0f, 0.0f};

const std::string GBuffer::vshaderSource =
    "#version 130\n\r"
    "a"
    "in vec3 pos;"
    "out vec2 uv;"
    ""
    "void main()"
    "{"
        "gl_Position = vec4(pos, 1.0);"
        "uv = (pos.xy + vec2(1.0, 1.0))/2.0;"
    "}";

GBuffer::GBuffer(int width, int height) : FrameBuffer(width, height)
{
    // Create the vertex shader
    Shader *vshader = new Shader(); vshader->CreateFromSourceCode(vshaderSource, GL_VERTEX_SHADER);
    //

    //Add depth buffer, this is mandatory
    AddDrawingBuffer(DepthAttachment, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT,
                     GL_CLAMP_TO_EDGE, GL_NEAREST);
    //
}

GBuffer::~GBuffer()
{
}

void GBuffer::SetColorTextureName(std::string name)
{
    if(not ExistsDrawingBuffer(ColorAttachment))
    {
        AddDrawingBuffer(ColorAttachment, GL_RGB, GL_RGB, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_LINEAR);
        colorName = name;
    }
}

void GBuffer::SetPositionTextureName(std::string name)
{
    if(not ExistsDrawingBuffer(PositionAttachment))
    {
        AddDrawingBuffer(PositionAttachment, GL_RGB, GL_RGB, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_LINEAR);
        posName = name;
    }
}

void GBuffer::SetNormalsTextureName(std::string name)
{
    if(not ExistsDrawingBuffer(NormalsAttachment))
    {
        AddDrawingBuffer(NormalsAttachment, GL_RGB, GL_RGB, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_LINEAR);
        normalsName = name;
    }
}

void GBuffer::SetUvTextureName(std::string name)
{
    if(not ExistsDrawingBuffer(UvAttachment))
    {
        AddDrawingBuffer(UvAttachment, GL_RG, GL_RG, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_LINEAR);
        uvName = name;
    }
}

void GBuffer::SetDepthTextureName(string name)
{
    if(not ExistsDrawingBuffer(DepthAttachment))
    {
        AddDrawingBuffer(DepthAttachment, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT,
                         GL_CLAMP_TO_EDGE, GL_NEAREST);
        depthName = name;
    }
}

void GBuffer::SetFragmentShader(Shader *fshader)
{
    program = new ShaderProgram();
    program->AttachShader(*vshader);
    program->AttachShader(*fshader);
    program->Link();
}


Texture *GBuffer::GetColorTexture()
{
    return GetTexture(ColorAttachment);
}

Texture *GBuffer::GetPositionTexture()
{
    return GetTexture(PositionAttachment);
}

Texture *GBuffer::GetNormalsTexture()
{
    return GetTexture(NormalsAttachment);
}

Texture *GBuffer::GetUvTexture()
{
    return GetTexture(UvAttachment);
}
