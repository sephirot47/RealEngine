#include "../include/Light.h"

const float Light::screenMesh[12] = {1.0f, -1.0f, 0.0f,
                                     1.0f,  1.0f, 0.0f,
                                    -1.0f,  1.0f, 0.0f,
                                    -1.0f, -1.0f, 0.0f};

Light::Light(LightType type, float screenWidth, float screenHeight)
{
    // Create the vertex shader
    vshader = new Shader(); vshader->Create("Assets/Shaders/Light/default.vert", GL_VERTEX_SHADER);
    fshader = new Shader(); fshader->Create("Assets/Shaders/Light/Directional/default.frag", GL_FRAGMENT_SHADER);

    program = new ShaderProgram();
    program->AttachShader(*vshader);
    program->AttachShader(*fshader);
    program->Link();

    shadowBuffer = new FrameBuffer(screenWidth, screenHeight);
    shadowBuffer->AddDrawingBuffer(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT,
                                   GL_FLOAT, GL_CLAMP_TO_EDGE, GL_NEAREST);

    screenMeshVbo = new VBO();
    screenMeshVbo->SetData(Light::screenMesh, sizeof(Light::screenMesh));

    screenMeshVao = new VAO();
    screenMeshVao->AddAttribute(*screenMeshVbo, 0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    this->type = type;

    pos = vec3(-100, 100, 100);
    dir = vec3(1, -1, -1);
    color = vec3(1, 1, 1);
    intensity = 1.0f;
}

Light::~Light()
{

}

quat QuaternionFromTo(const vec3 &from, const vec3 &to)
{
    quat q;
    vec3 a = cross(from, to);
    q.x = a.x; q.y = a.y; q.z = a.z;
    float d = dot(from, to);
    if(abs(d) > 0.99999f) return quat(); //return identity(do nothing)
    q.w = sqrt(from.length() * from.length() * to.length() * to.length()) + d;
    return normalize(q);
}

//Guarda en el depthbuffer la depth de la mesh enfocada desde la luz
void Light::BufferShadow(Mesh &m, float screenWidth, float screenHeight)
{
    shadowBuffer->Bind();

    mat4 projection = ortho(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f);
    m.GetShaderProgram()->SetUniform("projection", projection);

    mat4 lightView = mat4(1.0f);
    lightView = translate(lightView, pos);
    quat rot = QuaternionFromTo(pos, pos + dir*9999.0f);
    mat4 rotMat = mat4_cast(rot);
    lightView = rotMat * lightView;
    lightView = inverse(lightView);

    m.GetShaderProgram()->SetUniform("lightView", lightView);

    m.Draw();

    shadowBuffer->UnBind();
}

void Light::ApplyLight(GBuffer &gbuffer, glm::mat4 &camView) const
{
    glDisable(GL_DEPTH_TEST);
    if(type == DirectionalLight)
    {
        gbuffer.Bind();
        screenMeshVao->Bind();
        program->Use();

        program->AttachTexture("colors", *gbuffer.GetColorTexture());
        program->AttachTexture("pos", *gbuffer.GetPositionTexture());
        program->AttachTexture("uvs", *gbuffer.GetUvTexture());
        program->AttachTexture("normals", *gbuffer.GetNormalsTexture());
        program->AttachTexture("depth", *gbuffer.GetDepthTexture());

        program->SetUniform("camView", camView);

        mat4 invView = inverse(camView);
        program->SetUniform("camViewInverse", invView);

        mat4 lightModel = mat4(1.0f);
        lightModel = translate(lightModel, pos);
        program->SetUniform("lightModel", lightModel);

        program->SetUniform("lightPosition", pos);
        program->SetUniform("lightDir", dir);
        program->SetUniform("lightColor", color);
        program->SetUniform("lightIntensity", intensity);
        glDrawArrays(GL_QUADS, 0, 4);

        program->UnUse();
        screenMeshVao->UnBind();
    }
    glEnable(GL_DEPTH_TEST);
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

void Light::SetShadow(float shadow)
{
    this->shadow = shadow;
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

float Light::GetShadow() const
{
    return shadow;
}

