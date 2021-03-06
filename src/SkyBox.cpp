#include "../include/SkyBox.h"

using namespace RE;

SkyBox::SkyBox()
{
    Shader *vshader = new Shader(); vshader->Create("Assets/Shaders/SkyBox/skybox.vert", GL_VERTEX_SHADER);
    Shader *fshader = new Shader(); fshader->Create("Assets/Shaders/SkyBox/skybox.frag", GL_FRAGMENT_SHADER);
    program = new ShaderProgram();
    program->AttachShader(*fshader);
    program->AttachShader(*vshader);
    program->Link();

    cubeTexture = nullptr;

    std::vector<glm::vec3> cubePositions = std::vector<glm::vec3>(24);

    //BACK
    cubePositions[0] = glm::vec3( 1, -1,  1 );
    cubePositions[1] = glm::vec3( 1,  1,  1 );
    cubePositions[2] = glm::vec3(-1,  1,  1 );
    cubePositions[3] = glm::vec3(-1, -1,  1 );

    //FRONT
    cubePositions[4] = glm::vec3(-1, -1, -1 );
    cubePositions[5] = glm::vec3(-1,  1, -1 );
    cubePositions[6] = glm::vec3( 1,  1, -1 );
    cubePositions[7] = glm::vec3( 1, -1, -1 );

    //RIGHT
    cubePositions[8]  = glm::vec3( 1, -1, -1 );
    cubePositions[9]  = glm::vec3( 1,  1, -1 );
    cubePositions[10] = glm::vec3( 1,  1,  1 );
    cubePositions[11] = glm::vec3( 1, -1,  1 );

    //LEFT
    cubePositions[12] = glm::vec3(-1, -1,  1 );
    cubePositions[13] = glm::vec3(-1,  1,  1 );
    cubePositions[14] = glm::vec3(-1,  1, -1 );
    cubePositions[15] = glm::vec3(-1, -1, -1 );

    //TOP
    cubePositions[16] = glm::vec3( 1,  1,  1 );
    cubePositions[17] = glm::vec3( 1,  1, -1 );
    cubePositions[18] = glm::vec3(-1,  1, -1 );
    cubePositions[19] = glm::vec3(-1,  1,  1 );

    //BOTTOM
    cubePositions[20] = glm::vec3(-1, -1,  1 );
    cubePositions[21] = glm::vec3(-1, -1, -1 );
    cubePositions[22] = glm::vec3( 1, -1, -1 );
    cubePositions[23] = glm::vec3( 1, -1,  1 );

    cubeMesh = new Mesh();
    cubeMesh->LoadPositionsFromArray(cubePositions, GL_QUADS);
    //cubeMesh->LoadFromFile("Assets/Models/cube.obj");
}

SkyBox::~SkyBox()
{
    delete program;
}

void SkyBox::Render(RenderTarget &renderTarget, Camera &camera)
{
    glm::mat4 view = camera.GetView(), projection = camera.GetProjection();
    Render(renderTarget, view, projection);
}

void SkyBox::Render(RenderTarget &renderTarget, glm::mat4 &camView, glm::mat4 &camProjection)
{
    if(not cubeTexture)
    {
        DbgError("You can't render a SkyBox without a CubeMap. Try setting it using SetCubeMap");
        return;
    }

    StateManager::Push();

    //#1111 SetDrawingBuffers, solo al buffer del color
    program->AttachTexture("cubemap", *cubeTexture);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glm::mat4 model;
    cubeMesh->Render(renderTarget, *program, model, camView, camProjection);

    StateManager::Pop();
}

void SkyBox::SetCubeMap(CubeMap &cubeTexture)
{
    this->cubeTexture = &cubeTexture;
}

Mesh *SkyBox::GetMesh() const
{
    return cubeMesh;
}


ShaderProgram *SkyBox::GetShaderProgram() const
{
    return program;
}

CubeMap *SkyBox::GetCubeMap() const
{
    return cubeTexture;
}
