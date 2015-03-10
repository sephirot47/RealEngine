#include "../include/FrameDrawer.h"

const float FrameDrawer::screenMesh[12] = {1.0f, -1.0f, 0.0f,
                                           1.0f,  1.0f, 0.0f,
                                          -1.0f,  1.0f, 0.0f,
                                          -1.0f, -1.0f, 0.0f};

const char *FrameDrawer::vshaderSource =
    "#version 130"
    ""
    "in vec3 pos;"
    "out vec2 uv;"
    ""
    "void main()"
    "{"
        "gl_Position = vec4(pos, 1.0);"
        "uv = (pos.xy + vec2(1.0, 1.0))/2.0;"
    "}";

FrameDrawer::FrameDrawer(FrameBuffer &fb)
{
    frameBuffer = &fb;

    vbo = new VBO();
    vbo->SetData(FrameDrawer::screenMesh, sizeof(FrameDrawer::screenMesh));

    vao = new VAO();
    vao->AddAttribute(*vbo, 0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    fshader = new Shader(); fshader->Create("fbfshader", GL_FRAGMENT_SHADER);
    vshader = new Shader(); vshader->Create("fbvshader", GL_VERTEX_SHADER);
    program = new ShaderProgram();
    program->AttachShader(*fshader);
    program->AttachShader(*vshader);
    program->Link();
}

FrameDrawer::~FrameDrawer()
{
}

void FrameDrawer::Draw() const
{
    frameBuffer->UnBind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vao->Bind();
    program->Use();
    frameBuffer->GetColorTexture()->Bind(0);
    frameBuffer->GetDepthTexture()->Bind(1);

    program->SetUniform("scene", 0);
    program->SetUniform("depth", 1);

    glDrawArrays(GL_QUADS, 0, 4);

    frameBuffer->GetColorTexture()->UnBind(0);
    frameBuffer->GetDepthTexture()->UnBind(1);
    program->UnUse();
    vao->UnBind();
}

