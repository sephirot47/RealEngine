#include "../include/CubeMap.h"

using namespace RE;

CubeMap::CubeMap()
{
    glGenTextures(1, &object);
    SetWrapMode(GL_CLAMP_TO_EDGE);
    SetScaleMode(GL_LINEAR);

    faceTextures = std::vector<Texture*>(6, new Texture(Texture::Target::TextureCubeMap));
}

CubeMap::~CubeMap()
{
    glDeleteTextures(1, &object);
}

void CubeMap::SetWrapMode(GLenum mode)
{
    StateManager::Push();

    Bind();
    glTexParameteri(Texture::Target::TextureCubeMap, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(Texture::Target::TextureCubeMap, GL_TEXTURE_WRAP_T, mode);
    glTexParameteri(Texture::Target::TextureCubeMap, GL_TEXTURE_WRAP_R, mode);
    UnBind();

    StateManager::Pop();
}

void CubeMap::SetScaleMode(GLenum mode)
{
    StateManager::Push();

    Bind();
    glTexParameteri(Texture::Target::TextureCubeMap, GL_TEXTURE_MIN_FILTER, mode);
    glTexParameteri(Texture::Target::TextureCubeMap, GL_TEXTURE_MAG_FILTER, mode);
    UnBind();

    StateManager::Pop();
}


void CubeMap::SetFaceTexture(Face face, Image &image)
{
    StateManager::Push();

    Bind();

    faceTextures[face]->SetDataForCubeMap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                                          image.GetData(), image.GetWidth(), image.GetHeight(),
                                          image.GetFormat(), GL_UNSIGNED_BYTE, GL_RGB);

    /*glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGB, image.GetWidth(), image.GetHeight(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, image.GetData());
                 */
    UnBind();

    StateManager::Pop();
}

void CubeMap::Bind() const
{
    glBindTexture(Texture::Target::TextureCubeMap, object);
}

void CubeMap::UnBind() const
{
    glBindTexture(Texture::Target::TextureCubeMap, 0);
}

void CubeMap::Bind(GLuint slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(Texture::Target::TextureCubeMap, object);
}

void CubeMap::UnBind(GLuint slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(Texture::Target::TextureCubeMap, 0);
}


Texture *CubeMap::GetFaceTexture(Face face)
{
    return faceTextures[face];
}
