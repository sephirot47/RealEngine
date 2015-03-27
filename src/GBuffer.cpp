#include "../include/GBuffer.h"

const float GBuffer::screenMesh[12] = {1.0f, -1.0f, 0.0f,
                                       1.0f,  1.0f, 0.0f,
                                      -1.0f,  1.0f, 0.0f,
                                      -1.0f, -1.0f, 0.0f};

GBuffer::GBuffer(float width, float height, Shader &fshader) : FrameBuffer(width, height)
{
    this->width = width;
    this->height = height;

    // Create the vertex shader
    vshader = new Shader(); vshader->Create("Assets/Shaders/FrameBuffer/default.vert", GL_VERTEX_SHADER);
    this->fshader = &fshader;
    program = new ShaderProgram();
    program->AttachShader(*vshader);
    program->AttachShader(*(this->fshader));
    program->Link();

    screenMeshVbo = new VBO();
    screenMeshVbo->SetData(GBuffer::screenMesh, sizeof(GBuffer::screenMesh));

    screenMeshVao = new VAO();
    screenMeshVao->AddAttribute(*screenMeshVbo, 0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //Add buffers
    AddDrawingBuffer(FinalColorAttachment, GL_RGBA, GL_RGBA, GL_FLOAT, GL_REPEAT, GL_LINEAR);
    AddDrawingBuffer(TextureColorAttachment, GL_RGBA, GL_RGBA, GL_FLOAT, GL_REPEAT, GL_LINEAR);
    AddDrawingBuffer(PositionAttachment, GL_RGBA32F, GL_RGB, GL_FLOAT, GL_REPEAT, GL_LINEAR);
    AddDrawingBuffer(UvAttachment, GL_RGB, GL_RGB, GL_FLOAT, GL_REPEAT, GL_LINEAR);
    AddDrawingBuffer(NormalsAttachment, GL_RGBA32F, GL_RGB, GL_FLOAT, GL_REPEAT, GL_LINEAR);
    AddDrawingBuffer(DepthAttachment, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_NEAREST);
    //
}

GBuffer::~GBuffer()
{
}

void GBuffer::Draw() const
{
    UnBind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    screenMeshVao->Bind();
    program->Use();

    program->SetUniform("width",  float(width));
    program->SetUniform("height", float(height));

    glDrawArrays(GL_QUADS, 0, 4);

    program->UnUse();
    screenMeshVao->UnBind();
}

void GBuffer::DrawToScreen() const
{
    Draw();
}


void GBuffer::SetFragmentFinalColorTextureName(std::string name)
{
    program->AttachTexture(name, *GetFinalColorTexture());
}

void GBuffer::SetFragmentTextureColorTextureName(std::string name)
{
    program->AttachTexture(name, *GetTextureColorTexture());
}

void GBuffer::SetFragmentPositionTextureName(std::string name)
{
    program->AttachTexture(name, *GetPositionTexture());
}

void GBuffer::SetFragmentUvTextureName(std::string name)
{
    program->AttachTexture(name, *GetUvTexture());
}

void GBuffer::SetFragmentNormalsTextureName(std::string name)
{
    program->AttachTexture(name, *GetNormalsTexture());
}


void GBuffer::SetFragmentDepthTextureName(std::string name)
{
    program->AttachTexture(name, *GetDepthTexture());
}

ShaderProgram *GBuffer::GetShaderProgram() const
{
    return program;
}

Texture *GBuffer::GetFinalColorTexture() const
{
    return GetTexture(FinalColorAttachment);
}

Texture *GBuffer::GetTextureColorTexture() const
{
    return GetTexture(TextureColorAttachment);
}

Texture *GBuffer::GetPositionTexture() const
{
    return GetTexture(PositionAttachment);
}

Texture *GBuffer::GetNormalsTexture() const
{
    return GetTexture(NormalsAttachment);
}

Texture *GBuffer::GetUvTexture() const
{
    return GetTexture(UvAttachment);
}

Texture *GBuffer::GetDepthTexture() const
{
    return GetTexture(DepthAttachment);
}


float GBuffer::GetWidth() const
{
    return width;
}

float GBuffer::GetHeight() const
{
    return height;
}
