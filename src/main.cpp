#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <cstring>
#include <stdio.h>

#include "../include/RealEngine.h"

using namespace std;

const float width = 1500, height = 800;

GBuffer *gbuffer;
Mesh *mesh, *mesh2, *meshSphere;
Light *light;
vec3 cameraRot, cameraPos;
Texture *texture;

void Init()
{
    glClearColor(0.0, 0.0, 0.3, 1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //LUIGI THINGS
    Shader *fshader = new Shader(); fshader->Create("Assets/Shaders/Mesh/default.frag", GL_FRAGMENT_SHADER);
    Shader *vshader = new Shader(); vshader->Create("Assets/Shaders/Mesh/default.vert", GL_VERTEX_SHADER);
    ShaderProgram *program = new ShaderProgram();
    program->AttachShader(*fshader);
    program->AttachShader(*vshader);
    program->Link();
    ShaderProgram *program2 = new ShaderProgram();
    program2->AttachShader(*fshader);
    program2->AttachShader(*vshader);
    program2->Link();

    ShaderProgram *program3 = new ShaderProgram();
    program3->AttachShader(*fshader);
    program3->AttachShader(*vshader);
    program3->Link();

    Image *img = new Image(); img->LoadFromFile("Assets/Textures/luigiD.jpg");
    texture = new Texture();
    texture->SetData(img->GetData(), img->GetWidth(), img->GetHeight(), img->GetFormat(), img->GetFormat(), GL_UNSIGNED_BYTE);
    program->AttachTexture("tex", *texture);
    mesh = new Mesh();
    mesh->LoadFromFile("Assets/Models/luigi.obj");
    mesh->SetShaderProgram(*program);

    Image *img2 = new Image(); img2->LoadFromFile("Assets/Textures/gordaco.bmp");
    Texture *texture2 = new Texture();
    texture2->SetData(img2->GetData(), img2->GetWidth(), img2->GetHeight(), img2->GetFormat(), img2->GetFormat(), GL_UNSIGNED_BYTE);
    program2->AttachTexture("tex", *texture2);
    mesh2 = new Mesh();
    mesh2->LoadFromFile("Assets/Models/gordaco.obj");
    mesh2->SetShaderProgram(*program2);

    meshSphere = new Mesh();
    meshSphere->LoadFromFile("Assets/Models/quad.obj");
    meshSphere->SetShaderProgram(*program3);
    //

    Shader *finalfshader = new Shader(); finalfshader->Create("Assets/Shaders/FrameBuffer/default.frag", GL_FRAGMENT_SHADER);
    gbuffer = new GBuffer(width, height, *finalfshader);
    gbuffer->SetFragmentColorTextureName("colors");
    gbuffer->SetFragmentPositionTextureName("pos");
    gbuffer->SetFragmentUvTextureName("uvs");
    gbuffer->SetFragmentNormalsTextureName("normals");
    gbuffer->SetFragmentDepthTextureName("depth");

    light = new Light(DirectionalLight, width, height);
    light->SetPosition(vec3(0.0f, 0.0f, 1.5f));
    light->SetDirection(vec3(0.01f, -0.01f, -1.0f));
    light->SetColor(vec3(1.0f, 1.0f, 1.0f));
    light->SetIntensity(20.0f);

    cameraPos = vec3(0, 0, 0.0f);
    cameraRot = vec3(0, 0, 0.0f);
}

float rot = 0.0f, sphereRot = 0.0f, appTime = 0.0f;

void RenderScene()
{
    //light->SetPosition(cameraPos);

    light->SetPosition(vec3(0.0f, 0.1f, 5.0f));
    DbgLog(light->GetPosition());
    //light->SetDirection(-light->GetPosition());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    appTime += 0.03f;
    sphereRot += 0.03f;

    mat4 model(1.0f);
    vec3 axis(.0, 1.0, 0.0), translate, scale;
    mat4 T = glm::translate(model, translate);
    mat4 R = glm::rotate_slow(model, rot, axis);
    mat4 S = glm::scale(model, scale);

    gbuffer->Bind();
        gbuffer->ClearColorDepth();

        mat4 projection = perspective(45.0f * 3.1415f/180.0f, width/height, 0.1f, 100.0f);
        model = mat4(1.0f);
        translate = vec3(0.0f, 0.0f, 2.5f /*+ sin(appTime) * 50.0f*/ );
        scale = vec3(0.005);
        T = glm::translate(model, translate);
        S = glm::scale(model, scale);
        mesh->SetModelMatrix(T * R * S);
        mesh->SetNormalMatrix(R * S);

        model = mat4(1.0f);
        translate = vec3(-0.0f, -0.3f, -3.5f);
        scale = vec3(0.01f);
        T = glm::translate(model, translate);
        S = glm::scale(model, scale);
        mesh2->SetModelMatrix(T * R * S);
        mesh2->SetNormalMatrix(R * S);

        model = mat4(1.0f);
        translate = vec3(-0.0f, 0.0f, -4.0f);
        scale = vec3(0.01f);
        T = glm::translate(model, translate);
        R = glm::rotate_slow(model, 0.0f, axis);
        scale = vec3(5.0f);
        S = glm::scale(model, scale);
        meshSphere->SetModelMatrix(T * R * S);
        meshSphere->SetNormalMatrix(R * S);

        mat4 view = mat4(1.0f);
        T = glm::translate(view, cameraPos);
        R = glm::rotate_slow(view, rot, axis);
        view = glm::inverse(T * R);


        light->ClearBufferMeshShadow();
        light->BufferMeshShadow(*mesh, width, height);
        light->BufferMeshShadow(*mesh2, width, height);
        light->BufferMeshShadow(*meshSphere, width, height);

        gbuffer->Bind();

        mesh->Draw(projection, view);
        mesh2->Draw(projection, view);
       // meshSphere->Draw(projection, view);

        meshSphere->GetShaderProgram()->AttachTexture("tex", *texture);
        meshSphere->GetShaderProgram()->AttachTexture("tex", *light->GetShadowBuffer()->GetTexture(GL_DEPTH_ATTACHMENT));

        light->ApplyLight(*gbuffer, view);

        gbuffer->DrawToScreen();

    gbuffer->UnBind();
}

bool IsPressed(int keyCode)
{
    int n;
    const Uint8 *state = SDL_GetKeyboardState(&n);
    int i = SDL_GetScancodeFromKey(keyCode);
    if(i < n && n >= 1) return state[i] == 1;
    return false;
}

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* win;
    win = SDL_CreateWindow("Real Engine", 100, 100, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    SDL_GLContext context;
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    context = SDL_GL_CreateContext(win);

    Init();
    bool running = true;
    while(running)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            float camSpeed = 0.3f;
            if(event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_RIGHT)) cameraPos.x += camSpeed;
            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_LEFT)) cameraPos.x -= camSpeed;
            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_UP)) cameraPos.z -= camSpeed;
            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_DOWN)) cameraPos.z += camSpeed;
        }

        RenderScene();
        SDL_GL_SwapWindow(win);
        SDL_Delay(25);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
