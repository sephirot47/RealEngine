#ifndef LIGHT_H
#define LIGHT_H

#include "ShaderProgram.h"
#include "StateManager.h"
#include "Transform.h"
#include "Component.h"
#include "GBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Debug.h"
#include "Mesh.h"
#include "VAO.h"
#include "VBO.h"

#include "stb_image.h"
#include "glm/glm.hpp"

namespace RE
{

class Light : public Component
{

public:

    enum Type
    {
        DirectionalLight,
        PointLight
    };

private:

    static const float screenMesh[12];
    static const std::string vshaderSource;

    VAO *lightVao;
    VBO *lightVbo;
    ShaderProgram *lightProgram, *shadowProgram;
    Shader *lightvshader, *shadowvshader,
           *lightfshader, *shadowfshader;
    FrameBuffer *shadowBuffer;

    glm::vec3 color;
    float intensity, range;
    float shadow;

    bool enabled;

    Type type;

public:

    Light(Type type, float screenWidth, float screenHeight);
    ~Light();

    void ClearShadowMap();
    void ShadowMapMesh(Mesh &m,   const Transform &transform, float screenWidth, float screenHeight);
    void Render(GBuffer &gbuffer, const Transform &transform, const Camera &camera) const;
    void Render(GBuffer &gbuffer, const Transform &transform, const glm::mat4 &camView, const glm::mat4 &camProjection) const;

    void SetColor(glm::vec3 color);
    void SetIntensity(float intensity);
    void SetRange(float range);
    void SetShadow(float shadow);
    void SetEnabled(bool enabled);

    FrameBuffer *GetShadowBuffer() const;
    glm::vec3 GetColor() const;
    float GetIntensity() const;
    float GetRange() const;
    float GetShadow() const;
    bool GetEnabled() const;

    glm::mat4 GetView(const Transform &transform) const;
    glm::mat4 GetProjection(float screenWidth, float screenHeight) const;
};

}

#endif // LIGHT_H
