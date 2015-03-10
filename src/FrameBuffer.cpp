#include "../include/FrameBuffer.h"

const float FrameBuffer::screenMesh[12] = {1.0f, -1.0f, 0.0f,
                                           1.0f,  1.0f, 0.0f,
                                          -1.0f,  1.0f, 0.0f,
                                          -1.0f, -1.0f, 0.0f};

FrameBuffer::FrameBuffer(int width, int height)
{
    glGenFramebuffers(1, &object);

    colorTexture = new Texture();
    colorTexture->SetData(0, width, height, GL_RGB, GL_RGB, GL_FLOAT);
    colorTexture->SetWrapMode(GL_REPEAT);
    colorTexture->SetScaleMode(GL_LINEAR);

    depthTexture = new Texture();
    depthTexture->SetData(0, width, height, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT);
    depthTexture->SetWrapMode(GL_CLAMP_TO_EDGE);
    depthTexture->SetScaleMode(GL_NEAREST);

    Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->GetObject(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, depthTexture->GetObject(), 0);
    UnBind();
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &object);
    delete colorTexture;
    delete depthTexture;
}

void FrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, object);
}

void FrameBuffer::UnBind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
