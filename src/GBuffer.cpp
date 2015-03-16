#include "../include/GBuffer.h"

GBuffer::GBuffer(int width, int height) : FrameBuffer(width, height)
{

}

GBuffer::~GBuffer()
{
}

void GBuffer::SetColorTextureName(std::string name)
{

}

void GBuffer::SetPositionTextureName(std::string name)
{

}

void GBuffer::SetNormalsTextureName(std::string name)
{

}

void GBuffer::SetUvTextureName(std::string name)
{

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
