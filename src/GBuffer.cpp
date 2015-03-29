#include "../include/GBuffer.h"

using namespace RE;

const std::string GBuffer::GColorInputName = "GColor";
const std::string GBuffer::GPositionInputName = "GPosition";
const std::string GBuffer::GUvInputName = "GUv";
const std::string GBuffer::GNormalInputName = "GNormal";
const std::string GBuffer::GMaterialTextureInputName = "GMaterialTexture";
const std::string GBuffer::GDepthInputName = "GDepth";

const float GBuffer::screenMesh[12] = {1.0f, -1.0f, 0.0f,
                                       1.0f,  1.0f, 0.0f,
                                      -1.0f,  1.0f, 0.0f,
                                      -1.0f, -1.0f, 0.0f};

GBuffer::GBuffer(float width, float height) : FrameBuffer(width, height)
{
    this->width = width;
    this->height = height;

    //To render to the screen
    screenMeshVbo = new VBO();
    screenMeshVbo->SetData(GBuffer::screenMesh, sizeof(GBuffer::screenMesh));

    screenMeshVao = new VAO();
    screenMeshVao->AddAttribute(*screenMeshVbo, 0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    vshader = new Shader(); vshader->Create("Assets/Shaders/FrameBuffer/framebuffer.vert", GL_VERTEX_SHADER);
    fshader = new Shader(); fshader->Create("Assets/Shaders/FrameBuffer/framebuffer.frag", GL_FRAGMENT_SHADER);
    program = new ShaderProgram();
    program->AttachShader(*vshader);
    program->AttachShader(*fshader);
    program->Link();
    //

    //Add buffers
    AddDrawingBuffer(GColorAttachment, GL_RGBA, GL_FLOAT, GL_RGBA, GL_REPEAT, GL_LINEAR);
    AddDrawingBuffer(GPositionAttachment, GL_RGBA, GL_FLOAT, GL_RGBA32F, GL_REPEAT, GL_LINEAR);
    AddDrawingBuffer(GUvAttachment, GL_RG, GL_FLOAT, GL_RG, GL_REPEAT, GL_LINEAR);
    AddDrawingBuffer(GNormalAttachment, GL_RGB, GL_FLOAT, GL_RGBA32F, GL_REPEAT, GL_LINEAR);
    AddDrawingBuffer(GMaterialTextureAttachment, GL_RGBA, GL_FLOAT, GL_RGBA, GL_REPEAT, GL_LINEAR);
    /*
    AddDrawingBuffer(GMaterialDiffuseAttachment, GL_RGB, GL_RGB, GL_FLOAT, GL_REPEAT, GL_LINEAR);
    AddDrawingBuffer(MaterialSpecularAttachment, GL_RGB, GL_RGB, GL_FLOAT, GL_REPEAT, GL_LINEAR);
    AddDrawingBuffer(GMaterialShininessAttachment, GL_R32F, GL_R32F, GL_FLOAT, GL_REPEAT, GL_LINEAR);
    */
    AddDrawingBuffer(GDepthAttachment, GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_COMPONENT24, GL_CLAMP_TO_EDGE, GL_NEAREST);
    //

    program->AttachTexture(GColorInputName, *GetGColor());
    program->AttachTexture(GPositionInputName, *GetGPosition());
    program->AttachTexture(GUvInputName, *GetGUv());
    program->AttachTexture(GNormalInputName, *GetGNormal());
    program->AttachTexture(GMaterialTextureInputName, *GetGMaterialTexture());
    program->AttachTexture(GDepthInputName, *GetGDepth());
}

GBuffer::~GBuffer()
{
}

void GBuffer::Render() const
{
    StateManager::Push();

    screenMeshVao->Bind();
    program->Bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_QUADS, 0, 4);

    program->UnBind();
    screenMeshVao->UnBind();

    StateManager::Pop();
}

void GBuffer::RenderToScreen() const
{
    StateManager::Push();

    UnBindRenderTarget();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Render();

    StateManager::Pop();
}

void GBuffer::BindBuffersToProgram(ShaderProgram &program) const
{
    program.AttachTexture(GColorInputName, *GetGColor());
    program.AttachTexture(GPositionInputName, *GetGPosition());
    program.AttachTexture(GUvInputName, *GetGUv());
    program.AttachTexture(GNormalInputName, *GetGNormal());
    program.AttachTexture(GMaterialTextureInputName, *GetGMaterialTexture());
    program.AttachTexture(GDepthInputName, *GetGDepth());
}

ShaderProgram *GBuffer::GetShaderProgram() const
{
    return program;
}

Texture *GBuffer::GetGColor() const
{
    return GetTexture(GColorAttachment);
}

Texture *GBuffer::GetGMaterialTexture() const
{
    return GetTexture(GMaterialTextureAttachment);
}

Texture *GBuffer::GetGPosition() const
{
    return GetTexture(GPositionAttachment);
}

Texture *GBuffer::GetGNormal() const
{
    return GetTexture(GNormalAttachment);
}

Texture *GBuffer::GetGUv() const
{
    return GetTexture(GUvAttachment);
}

Texture *GBuffer::GetGDepth() const
{
    return GetTexture(GDepthAttachment);
}


float GBuffer::GetWidth() const
{
    return width;
}

float GBuffer::GetHeight() const
{
    return height;
}
