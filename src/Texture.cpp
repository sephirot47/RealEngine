#include "../include/Texture.h"

using namespace RE;

void Texture::Init()
{
    this->width = 0;
    this->height = 0;

    glGenTextures(1, &object);
    SetWrapMode(GL_REPEAT);
    SetScaleMode(GL_LINEAR);

    framebuffer = 0;
}

void Texture::SetDataForCubeMap(GLenum faceTarget, const void *data, int width, int height,
                                GLint format, GLenum type, GLint internalFormat)
{
    StateManager::Push();

    this->width = width;
    this->height = height;
    glTexImage2D(faceTarget, 0, internalFormat, width, height, 0, format, type, data);

    StateManager::Pop();
}

Texture::Texture(Target target)
{
    this->target = target;
    Init();
}

Texture::Texture() : Texture(Target::Texture2D) {}

Texture::Texture(const std::string filepath, Target target) : Texture(target)
{
    LoadFromFile(filepath);
}

Texture::Texture(int width, int height, Target target) : Texture(target)
{
    this->width = width;
    this->height = height;
    CreateEmpty(width, height);
}

Texture::~Texture()
{
    glDeleteTextures(1, &object);
}

void Texture::SetWrapMode(GLenum mode)
{
    StateManager::Push();

    Bind();
    glTexParameteri(target, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, mode);
    if(target == Target::TextureCubeMap) glTexParameteri(target, GL_TEXTURE_WRAP_R, mode);
    UnBind();

    StateManager::Pop();
}

void Texture::SetScaleMode(GLenum mode)
{
    StateManager::Push();

    Bind();
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, mode);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, mode);
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

        GLuint depthRenderBuffer;
        glGenRenderbuffers(1, &depthRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLenum drawbuffer = GL_COLOR_ATTACHMENT0; glDrawBuffers(1, &drawbuffer);
}

void Texture::UnBindRenderTarget() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Texture::SetData(const void *data, int width, int height, GLint format, GLenum type, GLint internalFormat)
{
    StateManager::Push();

    Bind();
    this->width = width;
    this->height = height;
    glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, data);
    UnBind();

    StateManager::Pop();
}

void Texture::LoadFromFile(const std::string filepath)
{
    Image *img = new Image(); img->LoadFromFile(filepath);
    SetData(img->GetData(), img->GetWidth(), img->GetHeight(), img->GetFormat(), GL_UNSIGNED_BYTE, img->GetFormat());
    delete img;
}

void Texture::CreateEmpty(int width, int height)
{
    this->width = width;
    this->height = height;
    SetData(nullptr, width, height, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
}

void Texture::CreateEmpty(int width, int height, GLint format, GLenum type, GLint internalFormat)
{
    this->width = width;
    this->height = height;
    SetData(nullptr, width, height, format, type, internalFormat);
}

void Texture::Bind() const
{
    glBindTexture(target, object);
}

void Texture::UnBind() const
{
    glBindTexture(target, 0);
}

void Texture::Bind(GLuint slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(target, object);
}

void Texture::UnBind(GLuint slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(target, 0);
}
