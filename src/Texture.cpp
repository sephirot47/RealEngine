#include "../include/Texture.h"

using namespace RE;

Texture::Texture()
{
    glGenTextures(1, &object);
    SetWrapMode(GL_REPEAT);
    SetScaleMode(GL_LINEAR);
    framebuffer = 0;
}

Texture::~Texture()
{
    glDeleteTextures(1, &object);
}

void Texture::SetWrapMode(GLenum mode) const
{
    StateManager::Push();

    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
    UnBind();

    StateManager::Pop();
}

void Texture::SetScaleMode(GLenum mode) const
{
    StateManager::Push();

    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
    UnBind();

    StateManager::Pop();
}

void Texture::BindRenderTarget() const
{
    if(framebuffer == 0)
    {
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, object, 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLenum drawbuffer = GL_COLOR_ATTACHMENT0; glDrawBuffers(1, &drawbuffer);
}

void Texture::UnBindRenderTarget() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Texture::SetData(const void *data, int width, int height, GLint format, GLenum type, GLint internalFormat) const
{
    StateManager::Push();

    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    UnBind();

    StateManager::Pop();
}

void Texture::LoadFromFile(const std::string filepath) const
{
    Image *img = new Image(); img->LoadFromFile(filepath);
    SetData(img->GetData(), img->GetWidth(), img->GetHeight(), img->GetFormat(), GL_UNSIGNED_BYTE, img->GetFormat());
    delete img;
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, object);
}

void Texture::UnBind() const
{
    glBindTexture(GL_TEXTURE_2D, object);
}

void Texture::Bind(GLuint slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, object);
}

void Texture::UnBind(GLuint slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}
