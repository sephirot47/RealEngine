#include "../include/Light.h"

const float Light::screenMesh[12] = {1.0f, -1.0f, 0.0f,
                                     1.0f,  1.0f, 0.0f,
                                    -1.0f,  1.0f, 0.0f,
                                    -1.0f, -1.0f, 0.0f};

Light::Light(LightType type, float screenWidth, float screenHeight)
{
    StateManager::Push();

    this->type = type;

    //Light stuff
       //VAO & VBO
    lightVbo = new VBO();
    lightVbo->SetData(Light::screenMesh, sizeof(Light::screenMesh));
    lightVao = new VAO();
    lightVao->AddAttribute(*lightVbo, 0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //Shaders & ShaderProgram
    if(type == DirectionalLight)
    {
        lightvshader = new Shader(); lightvshader->Create("Assets/Shaders/Light/light.vert", GL_VERTEX_SHADER);
        lightfshader = new Shader(); lightfshader->Create("Assets/Shaders/Light/Directional/directional.frag", GL_FRAGMENT_SHADER);
    }
    else
    {
        lightvshader = new Shader(); lightvshader->Create("Assets/Shaders/Light/light.vert", GL_VERTEX_SHADER);
        lightfshader = new Shader(); lightfshader->Create("Assets/Shaders/Light/Point/point.frag", GL_FRAGMENT_SHADER);
    }

    lightProgram = new ShaderProgram();
    lightProgram->AttachShader(*lightvshader);
    lightProgram->AttachShader(*lightfshader);
    lightProgram->Link();
    //

    //Shadow stuff
        //Shaders & ShaderProgram
    shadowvshader = new Shader(); shadowvshader->Create("Assets/Shaders/Light/Directional/shadow.vert", GL_VERTEX_SHADER);
    shadowfshader = new Shader(); shadowfshader->Create("Assets/Shaders/Light/Directional/shadow.frag", GL_FRAGMENT_SHADER);
    shadowProgram = new ShaderProgram();
    shadowProgram->AttachShader(*shadowvshader);
    shadowProgram->AttachShader(*shadowfshader);
    shadowProgram->Link();

       //Buffer for the depth
    DbgLog(screenWidth << ", " << screenHeight);
    shadowBuffer = new FrameBuffer(screenWidth, screenHeight);
    shadowBuffer->AddDrawingBuffer(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT,
                                   GL_FLOAT, GL_CLAMP_TO_EDGE, GL_LINEAR);
    shadowBuffer->Bind();
    glBindTexture(GL_TEXTURE_2D, shadowBuffer->GetTexture(GL_DEPTH_ATTACHMENT)->GetObject());
    glBindTexture(GL_TEXTURE_2D, 0);

    //

    pos = vec3(0, 0, 10);
    dir = vec3(0, 0, -1);
    color = vec3(1, 1, 1);
    intensity = 1.0f;
    range = 2.0f;
    shadow = 0.3f;

    enabled = true;

    StateManager::Pop();
}

Light::~Light()
{

}

//Guarda en el depthbuffer la depth de la mesh enfocada desde la luz
void Light::BufferMeshShadow(Mesh &m, float screenWidth, float screenHeight)
{
    StateManager::Push();

    shadowBuffer->Bind();
        shadowProgram->Use();

            glCullFace(GL_FRONT);

            shadowProgram->SetUniform("modelMatrix", m.GetModelMatrix());
            shadowProgram->SetUniform("lightView", GetView());
            shadowProgram->SetUniform("lightProjection", GetProjection(screenWidth, screenHeight));

            VAO *shadowVao = new VAO();
            shadowVao->AddAttribute(*m.GetVBOPos(), 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            shadowVao->Bind();
                glDrawArrays(m.GetDrawingMode(), 0, m.GetNumVertices());
            shadowVao->UnBind();
            delete shadowVao;

        shadowProgram->UnUse();
    shadowBuffer->UnBind();

    StateManager::Pop();
}

void Light::ClearBufferMeshShadow()
{
    StateManager::Push();

    shadowBuffer->Bind();
        glClearDepth(1.0);
        shadowBuffer->ClearColorDepth();
    shadowBuffer->UnBind();

    StateManager::Pop();
}


void Light::ApplyLight(GBuffer &gbuffer, const glm::mat4 &camView, const glm::mat4 &camProjection) const
{
    if(!enabled) return;

    StateManager::Push();

    glDisable(GL_DEPTH_TEST);
    if(type == DirectionalLight || type == PointLight)
    {
        gbuffer.Bind();
        lightVao->Bind();
        lightProgram->Use();

        lightProgram->AttachTexture("finalColors", *gbuffer.GetFinalColorTexture());
        lightProgram->AttachTexture("textureColors", *gbuffer.GetTextureColorTexture());
        lightProgram->AttachTexture("pos", *gbuffer.GetPositionTexture());
        lightProgram->AttachTexture("uvs", *gbuffer.GetUvTexture());
        lightProgram->AttachTexture("normals", *gbuffer.GetNormalsTexture());
        lightProgram->AttachTexture("depth", *gbuffer.GetDepthTexture());
        lightProgram->AttachTexture("shadowDepthBuffer", *shadowBuffer->GetTexture(GL_DEPTH_ATTACHMENT));

        mat4 camProjectionInverse = inverse(camProjection);
        lightProgram->SetUniform("camProjectionInverse", camProjectionInverse);
        mat4 camViewInverse = inverse(camView);
        lightProgram->SetUniform("camViewInverse", camViewInverse);

        mat4 lightView = GetView();
        lightProgram->SetUniform("lightView", lightView);
        mat4 lightProjection = GetProjection(gbuffer.GetWidth(), gbuffer.GetHeight());
        lightProgram->SetUniform("lightProjection", lightProjection);

        lightProgram->SetUniform("lightPosition", pos);
        lightProgram->SetUniform("lightRange", range);
        lightProgram->SetUniform("lightShadow", shadow);
        lightProgram->SetUniform("lightDir", dir);
        lightProgram->SetUniform("lightColor", color);
        lightProgram->SetUniform("lightIntensity", intensity);

        glDrawArrays(GL_QUADS, 0, 4);

        lightProgram->UnUse();
        lightVao->UnBind();
        gbuffer.UnBind();
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


quat LookAt(const vec3 eye, const vec3 lookTo, const vec3 up)
{
    if(eye == lookTo) return quat();

    mat4 m = lookAt(eye, lookTo, up);
    return quat_cast(transpose(m));
}
mat4 Light::GetView() const
{
    mat4 lightView = mat4(1.0f);
    mat4 T = translate(lightView, pos);
    vec3 lookTo = pos +  dir * 99.0f, up = vec3(0, 1, 0);
    quat rot = LookAt(pos, lookTo, up);
    mat4 R = mat4_cast(rot);
    return inverse(T * R);
}

mat4 Light::GetProjection(float screenWidth, float screenHeight) const
{
    float width = 10.0f;
    float height = width * screenHeight/screenWidth;
    return ortho(-width, width, -height, height, 0.0f, 40.0f);
}

