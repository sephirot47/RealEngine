#include "../include/RenderTarget.h"

void RenderTarget::BindFrameBuffer() const
{
    if(framebuffer) framebuffer->Bind();
}

void RenderTarget::UnBindFrameBuffer() const
{
    if(framebuffer) framebuffer->UnBind();
}
