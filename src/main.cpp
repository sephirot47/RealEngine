#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <cstring>
#include <stdio.h>

#include "../include/RealEngine.h"

using namespace std;

const float width = 1500, height = 800;

GBuffer *gbuffer;
Mesh *mesh, *mesh2;
Light *light;
vec3 cameraRot, cameraPos;

void Init()
{
    glClearColor(0.0, 0.0, 0.3, 1.0);
    glEnable(GL_DEPTH_TEST);

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

    Image *img = new Image(); img->LoadFromFile("Assets/Textures/luigiD.jpg");
    Texture *texture = new Texture();
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
    //

    Shader *finalfshader = new Shader(); finalfshader->Create("Assets/Shaders/FrameBuffer/default.frag", GL_FRAGMENT_SHADER);
    gbuffer = new GBuffer(width, height, *finalfshader);
    gbuffer->SetFragmentColorTextureName("colors");
    gbuffer->SetFragmentPositionTextureName("pos");
    gbuffer->SetFragmentUvTextureName("uvs");
    gbuffer->SetFragmentNormalsTextureName("normals");
    gbuffer->SetFragmentDepthTextureName("depth");

    light = new Light(DirectionalLight, width, height);
    light->SetPosition(vec3(0.0f, 0.0f, -100.0f));
    light->SetDirection(vec3(1.0f, 0.0f, -1.0f));
    light->SetColor(vec3(0.0f, 1.0f, 0.0f));
    light->SetIntensity(20.0f);

    cameraPos = vec3(0, 0, -1.0f);
    cameraRot = vec3(0, 0,  0.0f);
}

float rot = 0.0f, luigiRot = 0.0f, appTime = 0.0f;

void RenderScene()
{
    glDepthFunc(GL_LEQUAL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 model(1.0f), normalMatrix(1.0f);
    appTime += 0.03f;
    //luigiRot += 0.03f;
    vec3 axis(.0, 1.0, 0.0), translate(-0.3f, -0.5f, -1.5f), scale(0.005);
    mat4 T = glm::translate(model, translate);
    mat4 R = glm::rotate_slow(model, rot, axis);
    mat4 S = glm::scale(model, scale);
    model = T * R * S;
    normalMatrix = R * S;

    mat4 view = mat4(1.0f);
    T = glm::translate(view, cameraPos);
    R = glm::translate(view, cameraRot);
    view = glm::inverse(T * R);

    mat4 projection = perspective(45.0f * 3.1415f/180.0f, width/height, 0.1f, 100.0f);
    mesh->GetShaderProgram()->SetUniform("projection", projection);
    mesh->GetShaderProgram()->SetUniform("time", appTime);
    mesh->GetShaderProgram()->SetUniform("model", model);
    mesh->GetShaderProgram()->SetUniform("view", view);
    mesh->GetShaderProgram()->SetUniform("normalMatrix", normalMatrix);

    mesh2->GetShaderProgram()->SetUniform("projection", projection);
    mesh2->GetShaderProgram()->SetUniform("time", appTime);
    model = mat4(1.0f);
    translate = vec3(0.0f, -0.4f, -3.5f);
    scale = vec3(0.01f);
    T = glm::translate(model, translate);
    S = glm::scale(model, scale);
    model = T * R * S;
    mesh2->GetShaderProgram()->SetUniform("model", model);
    mesh2->GetShaderProgram()->SetUniform("view", view);
    mesh2->GetShaderProgram()->SetUniform("normalMatrix", normalMatrix);

    gbuffer->Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gbuffer->GetShaderProgram()->SetUniform("time", appTime);
        mesh->Draw();
        mesh2->Draw();
    gbuffer->UnBind();

    light->ApplyLight(*gbuffer, view);

    gbuffer->DrawToScreen();
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
