#include "../include/GBuffer.h"

const float GBuffer::screenMesh[12] = {1.0f, -1.0f, 0.0f,
                                       1.0f,  1.0f, 0.0f,
                                      -1.0f,  1.0f, 0.0f,
                                      -1.0f, -1.0f, 0.0f};

GBuffer::GBuffer(float width, float height) : FrameBuffer(width, height)
{
    this->width = width;
    this->height = height;

    // Create the vertex shader
    vshader = new Shader(); vshader->Create("Assets/Shaders/FrameBuffer/framebuffer.vert", GL_VERTEX_SHADER);
    fshader = new Shader(); fshader->Create("Assets/Shaders/FrameBuffer/framebuffer.frag", GL_FRAGMENT_SHADER);
    program = new ShaderProgram();
    program->AttachShader(*vshader);
    program->AttachShader(*fshader);
    program->Link();

    screenMeshVbo = new VBO();
    screenMeshVbo->SetData(GBuffer::screenMesh, sizeof(GBuffer::screenMesh));

    screenMeshVao = new VAO();
    screenMeshVao->AddAttribute(*screenMeshVbo, 0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //Add buffers
    AddDrawingBuffer(FinalColorAttachment, GL_RGBA, GL_FLOAT, GL_RGBA, GL_REPEAT, GL_LINEAR);
    AddDrawingBuffer(PositionAttachment, GL_RGBA, GL_FLOAT, GL_RGBA32F, GL_REPEAT, GL_LINEAR);
    AddDrawingBuffer(UvAttachment, GL_RG, GL_FLOAT, GL_RG, GL_REPEAT, GL_LINEAR);
    AddDrawingBuffer(NormalsAttachment, GL_RGB, GL_FLOAT, GL_RGBA32F, GL_REPEAT, GL_LINEAR);
    AddDrawingBuffer(MaterialTextureAttachment, GL_RGBA, GL_FLOAT, GL_RGBA, GL_REPEAT, GL_LINEAR);
    AddDrawingBuffer(DepthAttachment, GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_COMPONENT24, GL_CLAMP_TO_EDGE, GL_NEAREST);
    //

    /*
    AddDrawingBuffer(MaterialAmbientAttachment, GL_RGB, GL_RGB, GL_FLOAT, GL_REPEAT, GL_LINEAR);
    AddDrawingBuffer(MaterialDiffuseAttachment, GL_RGB, GL_RGB, GL_FLOAT, GL_REPEAT, GL_LINEAR);
    AddDrawingBuffer(MaterialSpecularAttachment, GL_RGB, GL_RGB, GL_FLOAT, GL_REPEAT, GL_LINEAR);
    AddDrawingBuffer(MaterialShininessAttachment, GL_R32F, GL_R32F, GL_FLOAT, GL_REPEAT, GL_LINEAR);
    */
}

GBuffer::~GBuffer()
{
}

void GBuffer::DrawToScreen() const
{
    StateManager::Push();

    UnBind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    screenMeshVao->Bind();
    program->Use();

    glDrawArrays(GL_QUADS, 0, 4);

    program->UnUse();
    screenMeshVao->UnBind();

    StateManager::Pop();
}


void GBuffer::SetFragmentFinalColorTextureName(std::string name)
{
    program->AttachTexture(name, *GetFinalColorTexture());
}

void GBuffer::SetFragmentMaterialTextureTextureName(std::string name)
{
    program->AttachTexture(name, *GetMaterialTextureTexture());
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

Texture *GBuffer::GetMaterialTextureTexture() const
{
    return GetTexture(MaterialTextureAttachment);
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
