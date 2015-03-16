#include "../include/GBuffer.h"

const float GBuffer::screenMesh[12] = {1.0f, -1.0f, 0.0f,
                                       1.0f,  1.0f, 0.0f,
                                      -1.0f,  1.0f, 0.0f,
                                      -1.0f, -1.0f, 0.0f};

const std::string GBuffer::vshaderSource =
    "#version 130\n\r"
    "a"
    "in vec3 pos;"
    "out vec2 sceneuv;"
    ""
    "void main()"
    "{"
        "gl_Position = vec4(pos, 1.0);"
        "uv = (pos.xy + vec2(1.0, 1.0))/2.0;"
    "}";

GBuffer::GBuffer(int width, int height, Shader &fshader) : FrameBuffer(width, height)
{
    // Create the vertex shader
    vshader = new Shader(); vshader->CreateFromSourceCode(vshaderSource, GL_VERTEX_SHADER);
    this->fshader = &fshader;
    program = new ShaderProgram();
    program->AttachShader(*vshader);
    program->AttachShader(*(this->fshader));
    program->Link();

    screenMeshVbo = new VBO();
    screenMeshVbo->SetData(GBuffer::screenMesh, sizeof(GBuffer::screenMesh));

    vao = new VAO();

    //Add depth buffer, this is mandatory
    AddDrawingBuffer(DepthAttachment, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT,
                     GL_CLAMP_TO_EDGE, GL_NEAREST);
    //
}

GBuffer::~GBuffer()
{
}

void GBuffer::Draw() const
{
    if(!fshader)
    {
        DbgError("You must attach a created fragment shader to the frame drawer in order to draw something");
        return;
    }

    UnBind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vao->Bind();
    program->Use();

    program->SetUniform("width",  float(width));
    program->SetUniform("height", float(height));

    glDrawArrays(GL_QUADS, 0, 4);

    program->UnUse();
    vao->UnBind();
}


void GBuffer::SetVertexColorLocation(int location, VBO &vbo)
{
    vao->AddAttribute(vbo, location, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void GBuffer::SetVertexPositonLocation(int location, VBO &vbo)
{
    vao->AddAttribute(vbo, location, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void GBuffer::SetVertexUvLocation(int location, VBO &vbo)
{
    vao->AddAttribute(vbo, location, 2, GL_FLOAT, GL_FALSE, 0, 0);
}


void GBuffer::SetVertexNormalsLocation(int location, VBO &vbo)
{
    vao->AddAttribute(vbo, location, 3, GL_FLOAT, GL_FALSE, 0, 0);
}


void GBuffer::SetFragmentColorTextureName(std::string name)
{
    if(not ExistsDrawingBuffer(ColorAttachment))
    {
        AddDrawingBuffer(ColorAttachment, GL_RGB, GL_RGB, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_LINEAR);
        program->AttachTexture(colorsName, *GetColorTexture());
        colorsName = name;
    }
}

void GBuffer::SetFragmentPositionTextureName(std::string name)
{
    if(not ExistsDrawingBuffer(PositionAttachment))
    {
        AddDrawingBuffer(PositionAttachment, GL_RGB, GL_RGB, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_LINEAR);
        posName = name;
        program->AttachTexture(colorsName, *GetColorTexture());
    }
}

void GBuffer::SetFragmentNormalsTextureName(std::string name)
{
    if(not ExistsDrawingBuffer(NormalsAttachment))
    {
        AddDrawingBuffer(NormalsAttachment, GL_RGB, GL_RGB, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_LINEAR);
        normalsName = name;
        program->AttachTexture(normalsName, *GetNormalsTexture());
    }
}

void GBuffer::SetFragmentUvTextureName(std::string name)
{
    if(not ExistsDrawingBuffer(UvAttachment))
    {
        AddDrawingBuffer(UvAttachment, GL_RG, GL_RG, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_LINEAR);
        uvName = name;
        program->AttachTexture(uvName, *GetUvTexture());
    }
}

void GBuffer::SetFragmentDepthTextureName(std::string name)
{
    if(not ExistsDrawingBuffer(DepthAttachment))
    {
        AddDrawingBuffer(DepthAttachment, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT,
                         GL_CLAMP_TO_EDGE, GL_NEAREST);
        depthName = name;
        program->AttachTexture(depthName, *GetDepthTexture());
    }
}

void GBuffer::SetVertexShader(Shader *vshader)
{
    this->vshader = vshader;

    if(fshader != nullptr)
    {
        program = new ShaderProgram();
        program->AttachShader(*vshader);
        program->AttachShader(*fshader);
        program->Link();
    }
}

Texture *GBuffer::GetColorTexture() const
{
    return GetTexture(ColorAttachment);
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
