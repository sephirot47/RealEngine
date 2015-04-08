#include "../include/Light.h"

using namespace RE;


const float Light::screenMesh[12] = {1.0f, -1.0f, 0.0f,
                                     1.0f,  1.0f, 0.0f,
                                    -1.0f,  1.0f, 0.0f,
                                    -1.0f, -1.0f, 0.0f};

Light::Light(Type type, float screenWidth, float screenHeight)
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
    shadowBuffer->AddDrawingBuffer(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_COMPONENT32,
                                   GL_CLAMP_TO_EDGE, GL_LINEAR);

    color = glm::vec3(1, 1, 1);
    intensity = 1.0f;
    range = 2.0f;
    shadow = 0.5f;

    enabled = true;

    StateManager::Pop();
}

Light::~Light()
{

}

void Light::ShadowMapMesh(Mesh &m, const Transform &transform, float screenWidth, float screenHeight)
{
    StateManager::Push();

    shadowBuffer->BindRenderTarget();
        shadowProgram->Bind();

            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);

            shadowProgram->SetUniform("PVM", GetProjection(screenWidth, screenHeight) * GetView(transform) * transform.GetModelMatrix());

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

void Light::ClearShadowMap()
{
    StateManager::Push();

    shadowBuffer->BindRenderTarget();
        shadowBuffer->ClearColorDepth();
    shadowBuffer->UnBindRenderTarget();

    StateManager::Pop();
}

void Light::Render(GBuffer &gbuffer, const Transform &transform, const Camera &camera) const
{
    glm::mat4 view = camera.GetView(), projection = camera.GetProjection();
    Render(gbuffer, transform, view, projection);
}

void Light::Render(GBuffer &gbuffer, const Transform &transform, const glm::mat4 &camView, const glm::mat4 &camProjection) const
{
    if(not enabled) return;

    StateManager::Push();

    glDisable(GL_DEPTH_TEST);
    if(type == DirectionalLight || type == PointLight)
    {
        gbuffer.BindRenderTarget();
        lightVao->Bind();
        lightProgram->Bind();

        gbuffer.BindBuffersToProgram(*lightProgram);
        lightProgram->SetUniform("shadowMapWidth",  shadowBuffer->GetWidth());
        lightProgram->SetUniform("shadowMapHeight", shadowBuffer->GetHeight());
        lightProgram->AttachTexture("shadowDepthBuffer", *(shadowBuffer->GetTexture(GL_DEPTH_ATTACHMENT)));


        glm::mat4 bias = glm::mat4(0.5, 0.0, 0.0, 0.0,
                                   0.0, 0.5, 0.0, 0.0,
                                   0.0, 0.0, 0.5, 0.0,
                                   0.5, 0.5, 0.5, 1.0);

        glm::mat4 proj = GetProjection(gbuffer.GetWidth(), gbuffer.GetHeight()), view = GetView(transform);
        lightProgram->SetUniform("light.projection", proj);
        lightProgram->SetUniform("light.view", view);
        lightProgram->SetUniform("light.projectionView", proj * view);
        lightProgram->SetUniform("light.biasedProjectionView", bias * proj * view);
        lightProgram->SetUniform("light.position", transform.GetPosition());
        lightProgram->SetUniform("light.direction", transform.GetRotation().GetForward());
        lightProgram->SetUniform("light.color", color);
        lightProgram->SetUniform("light.intensity", intensity);
        lightProgram->SetUniform("light.shadow", shadow);
        lightProgram->SetUniform("light.range", range);

        glm::vec3 camPosition(camView[3][0], camView[3][1], camView[3][2]);
        lightProgram->SetUniform("camera.position", camPosition);
        lightProgram->SetUniform("camera.view", camView);

        GLenum drawBuffers[] = {GBuffer::GBufferAttachment::GColorAttachment,
                                GBuffer::GBufferAttachment::GDepthAttachment};
        gbuffer.SetDrawingBuffers(2, &drawBuffers[0]);

        glDrawArrays(GL_QUADS, 0, 4);

        lightProgram->UnBind();
        lightVao->UnBind();
        gbuffer.UnBindRenderTarget();
    }

    StateManager::Pop();
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


glm::mat4 Light::GetView(const Transform &transform) const
{
    glm::mat4 lightView = glm::mat4(1.0f);
    glm::mat4 T = glm::translate(lightView, transform.GetPosition());
    glm::vec3 lookTo = transform.GetPosition() + transform.GetRotation().GetForward() * 99.0f;
    glm::vec3 up = glm::vec3(0, 1, 0);
    Quaternion rot = Quaternion::LookAt(transform.GetPosition(), lookTo, up);
    glm::mat4 R = glm::mat4_cast(rot);
    return glm::inverse(T * R);
}

glm::mat4 Light::GetProjection(float screenWidth, float screenHeight) const
{
    float width = 800.0f;
    float height = width * screenHeight/screenWidth;
    return glm::ortho(-width, width, -height, height, 350.0f, 4000.0f);
}

