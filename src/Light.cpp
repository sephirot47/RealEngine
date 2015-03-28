#include "../include/Light.h"

using namespace RE;


const float Light::screenMesh[12] = {1.0f, -1.0f, 0.0f,
                                     1.0f,  1.0f, 0.0f,
                                    -1.0f,  1.0f, 0.0f,
                                    -1.0f, -1.0f, 0.0f};

Light::Light(LightType type, float screenWidth, float screenHeight)
{
    StateManager::Push();

    this->type = type;

    lightVbo = new VBO();
    lightVbo->SetData(Light::screenMesh, sizeof(Light::screenMesh));
    lightVao = new VAO();
    lightVao->AddAttribute(*lightVbo, 0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    lightvshader = new Shader(); lightvshader->Create("Assets/Shaders/Light/light.vert", GL_VERTEX_SHADER);

    if(type == DirectionalLight)
    {
        lightfshader = new Shader(); lightfshader->Create("Assets/Shaders/Light/Directional/directional.frag", GL_FRAGMENT_SHADER);
    }
    else
    {
        lightfshader = new Shader(); lightfshader->Create("Assets/Shaders/Light/Point/point.frag", GL_FRAGMENT_SHADER);
    }

    lightProgram = new ShaderProgram();
    lightProgram->AttachShader(*lightvshader);
    lightProgram->AttachShader(*lightfshader);
    lightProgram->Link();

    shadowvshader = new Shader(); shadowvshader->Create("Assets/Shaders/Light/Directional/shadow.vert", GL_VERTEX_SHADER);
    shadowfshader = new Shader(); shadowfshader->Create("Assets/Shaders/Light/Directional/shadow.frag", GL_FRAGMENT_SHADER);
    shadowProgram = new ShaderProgram();
    shadowProgram->AttachShader(*shadowvshader);
    shadowProgram->AttachShader(*shadowfshader);
    shadowProgram->Link();

    shadowBuffer = new FrameBuffer(screenWidth, screenHeight);
    shadowBuffer->AddDrawingBuffer(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_COMPONENT16,
                                   GL_CLAMP_TO_EDGE, GL_LINEAR);

    pos = glm::vec3(0, 0, 10);
    dir = glm::vec3(0, 0, -1);
    color = glm::vec3(1, 1, 1);
    intensity = 1.0f;
    range = 2.0f;
    shadow = 0.3f;

    enabled = true;

    StateManager::Pop();
}

Light::~Light()
{

}

void Light::ShadowMapMesh(Mesh &m, float screenWidth, float screenHeight)
{
    StateManager::Push();

    shadowBuffer->BindRenderTarget();
        shadowProgram->Bind();

            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);

            shadowProgram->SetUniform("modelMatrix", m.GetModelMatrix());
            shadowProgram->SetUniform("lightView", GetView());
            shadowProgram->SetUniform("lightProjection", GetProjection(screenWidth, screenHeight));

            VAO *shadowVao = new VAO();
            shadowVao->AddAttribute(*m.GetVBOPos(), 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            shadowVao->Bind();
                glDrawArrays(m.GetRenderMode(), 0, m.GetNumVertices());
            shadowVao->UnBind();
            delete shadowVao;

        shadowProgram->UnBind();
    shadowBuffer->UnBindRenderTarget();

    StateManager::Pop();
}

void Light::ClearBufferMeshShadow()
{
    StateManager::Push();

    shadowBuffer->BindRenderTarget();
        shadowBuffer->ClearColorDepth();
    shadowBuffer->UnBindRenderTarget();

    StateManager::Pop();
}


void Light::ApplyLight(GBuffer &gbuffer, const glm::mat4 &camView, const glm::mat4 &camProjection) const
{
    if(!enabled) return;

    StateManager::Push();

    glDisable(GL_DEPTH_TEST);
    if(type == DirectionalLight || type == PointLight)
    {
        gbuffer.BindRenderTarget();
        lightVao->Bind();
        lightProgram->Bind();
        lightProgram->AttachTexture("finalColors", *gbuffer.GetFinalColorTexture());
        lightProgram->AttachTexture("pos", *gbuffer.GetPositionTexture());
        lightProgram->AttachTexture("uvs", *gbuffer.GetUvTexture());
        lightProgram->AttachTexture("normals", *gbuffer.GetNormalsTexture());
        lightProgram->AttachTexture("textureColors", *gbuffer.GetMaterialTextureTexture());
        lightProgram->AttachTexture("depth", *gbuffer.GetDepthTexture());
        lightProgram->AttachTexture("shadowDepthBuffer", *(shadowBuffer->GetTexture(GL_DEPTH_ATTACHMENT)));

        glm::mat4 camProjectionInverse = inverse(camProjection);
        lightProgram->SetUniform("camProjectionInverse", camProjectionInverse);
        glm::mat4 camViewInverse = inverse(camView);
        lightProgram->SetUniform("camViewInverse", camViewInverse);

        glm::mat4 lightView = GetView();
        lightProgram->SetUniform("lightView", lightView);
        glm::mat4 lightProjection = GetProjection(gbuffer.GetWidth(), gbuffer.GetHeight());
        lightProgram->SetUniform("lightProjection", lightProjection);

        lightProgram->SetUniform("lightPosition", pos);
        lightProgram->SetUniform("lightRange", range);
        lightProgram->SetUniform("lightShadow", shadow);
        lightProgram->SetUniform("lightDir", dir);
        lightProgram->SetUniform("lightColor", color);
        lightProgram->SetUniform("lightIntensity", intensity);

        glDrawArrays(GL_QUADS, 0, 4);

        lightProgram->UnBind();
        lightVao->UnBind();
        gbuffer.UnBindRenderTarget();
    }
    StateManager::Pop();
}

void Light::SetPosition(glm::vec3 position)
{
    pos = position;
}

void Light::SetDirection(glm::vec3 direction)
{
    dir = direction;
}

void Light::SetColor(glm::vec3 color)
{
    this->color = color;
}

void Light::SetIntensity(float intensity)
{
    this->intensity = intensity;
}

void Light::SetRange(float range)
{
    this->range = range;
}

void Light::SetShadow(float shadow)
{
    this->shadow = shadow;
}

void Light::SetEnabled(bool enabled)
{
    this->enabled = enabled;
}



FrameBuffer *Light::GetShadowBuffer() const
{
    return shadowBuffer;
}

glm::vec3 Light::GetPosition() const
{
    return pos;
}

glm::vec3 Light::GetDirection() const
{
    return dir;
}

glm::vec3 Light::GetColor() const
{
    return color;
}

float Light::GetIntensity() const
{
    return intensity;
}

float Light::GetRange() const
{
    return range;
}

float Light::GetShadow() const
{
    return shadow;
}

bool Light::GetEnabled() const
{
    return enabled;
}


glm::quat LookAt(const glm::vec3 eye, const glm::vec3 lookTo, const glm::vec3 up)
{
    if(eye == lookTo) return glm::quat();

    glm::mat4 m = lookAt(eye, lookTo, up);
    return glm::quat_cast( glm::transpose(m) );
}

glm::mat4 Light::GetView() const
{
    glm::mat4 lightView = glm::mat4(1.0f);
    glm::mat4 T = glm::translate(lightView, pos);
    glm::vec3 lookTo = pos +  dir * 99.0f;
    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::quat rot = LookAt(pos, lookTo, up);
    glm::mat4 R = glm::mat4_cast(rot);
    return glm::inverse(T * R);
}

glm::mat4 Light::GetProjection(float screenWidth, float screenHeight) const
{
    float width = 10.0f;
    float height = width * screenHeight/screenWidth;
    return glm::ortho(-width, width, -height, height, 0.0f, 40.0f);
}

