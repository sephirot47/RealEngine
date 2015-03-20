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
    shadowBuffer->AddDrawingBuffer(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT,
                                   GL_FLOAT, GL_CLAMP_TO_EDGE, GL_NEAREST);
    //

    pos = vec3(0, 0, 10);
    dir = vec3(0, 0, -1);
    color = vec3(1, 1, 1);
    intensity = 1.0f;
}

Light::~Light()
{

}

//Give the quaternion of the rotation from one direction to another(NOT POSITIONS pene)
quat QuaternionFromTo(vec3 from, vec3 to)
{
    from = normalize(from);
    to = normalize(to);
    vec3 a = cross(from, to);
    quat q;
    q.x = a.x; q.y = a.y; q.z = a.z;
    float d = dot(from, to);
    if(abs(d) > 0.99999f) return quat(); //return identity(do nothing)
    q.w = sqrt(from.length() * from.length() * to.length() * to.length()) + d;
    return normalize(q);
}

//Guarda en el depthbuffer la depth de la mesh enfocada desde la luz
void Light::BufferMeshShadow(Mesh &m, float screenWidth, float screenHeight)
{
    shadowBuffer->Bind();
        shadowProgram->Use();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            mat4 lightProjection = ortho(-100.0f, 100.0f, -100.0f, 100.0f, .1f, 9999.0f); //near & far must be > 0 (eventhough openGL uses < 0)
            //mat4 lightProjection = perspective(45.0f * 3.1415f/180.0f, screenWidth/screenHeight, 0.1f, 100.0f); //near & far must be > 0 (eventhough openGL uses < 0)
            shadowProgram->SetUniform("lightProjection", lightProjection);

            mat4 lightView = mat4(1.0f);
            mat4 T = translate(lightView, pos);
            quat rot = QuaternionFromTo(vec3(0.01), vec3(0.01) + dir * 999.0f);
            mat4 R = mat4_cast(rot);

            lightView = T * R;
            lightView = inverse(lightView);

            ////
            ////
            ////FALTA MULTIPLICAR POR EL MODEL DE LA MESH, HACE FALTA SETMODEL EN MESH Y TAL pene
            ////
            ////

            shadowProgram->SetUniform("lightView", lightView);

            VAO *shadowVao = new VAO();
            shadowVao->AddAttribute(*m.GetVBOPos(), 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            shadowVao->Bind();
                glDrawArrays(m.GetDrawingMode(), 0, m.GetNumVertices()); //Draw in order to get the depth from the light view
            shadowVao->UnBind();
            delete shadowVao;

        shadowProgram->UnUse();
    shadowBuffer->UnBind();
}

void Light::ApplyLight(GBuffer &gbuffer, glm::mat4 &camView) const
{
    glDisable(GL_DEPTH_TEST);
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

        lightProgram->SetUniform("camView", camView);

        mat4 camViewInverse = inverse(camView);
        lightProgram->SetUniform("camViewInverse", camViewInverse);

        mat4 lightModel = mat4(1.0f);
        lightModel = translate(lightModel, pos);
        lightProgram->SetUniform("lightModel", lightModel);

        lightProgram->SetUniform("lightPosition", pos);
        lightProgram->SetUniform("lightDir", dir);
        lightProgram->SetUniform("lightColor", color);
        lightProgram->SetUniform("lightIntensity", intensity);

        glDrawArrays(GL_QUADS, 0, 4);

        lightProgram->UnUse();
        lightVao->UnBind();
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

