#include "../include/Light.h"

const float Light::screenMesh[12] = {1.0f, -1.0f, 0.0f,
                                     1.0f,  1.0f, 0.0f,
                                    -1.0f,  1.0f, 0.0f,
                                    -1.0f, -1.0f, 0.0f};

Light::Light(LightType type, float screenWidth, float screenHeight)
{
    this->type = type;

    //Light stuff
       //VAO & VBO
    lightVbo = new VBO();
    lightVbo->SetData(Light::screenMesh, sizeof(Light::screenMesh));
    lightVao = new VAO();
    lightVao->AddAttribute(*lightVbo, 0, 3, GL_FLOAT, GL_FALSE, 0, 0);

       //Shaders & ShaderProgram
    lightvshader = new Shader(); lightvshader->Create("Assets/Shaders/Light/light.vert", GL_VERTEX_SHADER);
    lightfshader = new Shader(); lightfshader->Create("Assets/Shaders/Light/Directional/light.frag", GL_FRAGMENT_SHADER);
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
    shadowBuffer = new FrameBuffer(screenWidth, screenHeight);
    shadowBuffer->AddDrawingBuffer(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT,
                                   GL_FLOAT, GL_CLAMP_TO_EDGE, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, shadowBuffer->GetTexture(GL_DEPTH_ATTACHMENT)->GetObject());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, 0);

    //

    pos = vec3(0, 0, 10);
    dir = vec3(0, 0, -1);
    color = vec3(1, 1, 1);
    intensity = 1.0f;
}

Light::~Light()
{

}

//Guarda en el depthbuffer la depth de la mesh enfocada desde la luz
void Light::BufferMeshShadow(Mesh &m, float screenWidth, float screenHeight)
{
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

            glCullFace(GL_BACK);

        shadowProgram->UnUse();
    shadowBuffer->UnBind();
}

void Light::ClearBufferMeshShadow()
{
    shadowBuffer->Bind();
        glClearDepth(1.0);
        shadowBuffer->ClearColorDepth();
    shadowBuffer->UnBind();
}


void Light::ApplyLight(GBuffer &gbuffer, const glm::mat4 &camView, const glm::mat4 &camProjection) const
{
    //glDisable(GL_DEPTH_TEST);
    if(type == DirectionalLight)
    {
        gbuffer.Bind();
        lightVao->Bind();
        lightProgram->Use();

        lightProgram->AttachTexture("colors", *gbuffer.GetColorTexture());
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
        lightProgram->SetUniform("lightDir", dir);
        lightProgram->SetUniform("lightColor", color);
        lightProgram->SetUniform("lightIntensity", intensity);

        glDrawArrays(GL_QUADS, 0, 4);

        lightProgram->UnUse();
        lightVao->UnBind();
    }
    //glEnable(GL_DEPTH_TEST);
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

float Light::GetShadow() const
{
    return shadow;
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
    float width = 14.0f;
    float height = width * screenHeight/screenWidth;
    //return infinitePerspective(45.0f * 3.1415f/180.0f, screenWidth/screenHeight, -10.0f);
    return ortho(-width, width, -height, height, 0.9f, 20.0f);
}

