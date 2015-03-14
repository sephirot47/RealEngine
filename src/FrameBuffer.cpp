#include "../include/FrameBuffer.h"

FrameBuffer::FrameBuffer()
{
    glGenFramebuffers(1, &object);
    numBuffers = 0;

    /*
    colorTexture = new Texture();
    colorTexture->SetData(0, width, height, GL_RGB, GL_RGB, GL_FLOAT);
    colorTexture->SetWrapMode(GL_REPEAT);
    colorTexture->SetScaleMode(GL_LINEAR);

    depthTexture = new Texture();
    depthTexture->SetData(0, width, height, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT);
    depthTexture->SetWrapMode(GL_CLAMP_TO_EDGE);
    depthTexture->SetScaleMode(GL_NEAREST);
    */

    Bind();
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->GetObject(), 0);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,  GL_TEXTURE_2D, depthTexture->GetObject(), 0);
    UnBind();
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &object);
}

void FrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, object);
    glDrawBuffers(numBuffers, &drawBuffers[0]);
}

void FrameBuffer::UnBind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::SetDrawingBuffers(int size, GLenum *bufs)
{
    if(size == 0) return;

    Bind();

    drawBuffers.clear(); //Copiamos los buffers a drawBuffers
    for(GLenum *buffer = bufs; buffer < (bufs + size); ++buffer) drawBuffers.push_back(*buffer);
    numBuffers = size;

    glDrawBuffers(numBuffers, &drawBuffers[0]);

    UnBind();
}
