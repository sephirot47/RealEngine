#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <cstring>
#include <stdio.h>

#include "../include/RealEngine.h"

using namespace std;

const int width = 800, height = 800;

FrameBuffer *framebuffer;
GBuffer *gbuffer;

Texture *texture;
Mesh *mesh, *gordacoMesh;

Light *light, *light2;

void Init()
{
    glClearColor(0.0, 0.0, 0.3, 1.0);
    glEnable(GL_DEPTH_TEST);

    //LUIGI THINGS
    Shader *fshader = new Shader(); fshader->Create("fshader", GL_FRAGMENT_SHADER);
    Shader *vshader = new Shader(); vshader->Create("vshader", GL_VERTEX_SHADER);
    ShaderProgram *program = new ShaderProgram();
    program->AttachShader(*fshader);
    program->AttachShader(*vshader);
    program->Link();

    Image *img = new Image(); img->LoadFromFile("gordaco.bmp");
    texture = new Texture();
    texture->SetData(img->GetData(), img->GetWidth(), img->GetHeight(), img->GetFormat(), img->GetFormat(), GL_UNSIGNED_BYTE);

    program->AttachTexture("tex", *texture);

    mesh = new Mesh();
    mesh->LoadFromFile("gordaco.obj");
    mesh->SetShaderProgram(*program);

    img->LoadFromFile("luigiD.jpg");
    //

    Shader *finalfshader = new Shader(); finalfshader->Create("fbfshader", GL_FRAGMENT_SHADER);
    gbuffer = new GBuffer(width, height, *finalfshader);
    gbuffer->SetFragmentColorTextureName("colors");
    gbuffer->SetFragmentPositionTextureName("pos");
    gbuffer->SetFragmentUvTextureName("uvs");
    gbuffer->SetFragmentNormalsTextureName("normals");
    gbuffer->SetFragmentDepthTextureName("depth");

    light = new Light(DirectionalLight);
    light->SetDirection(vec3(1.0f, 0.0f, -1.0f));
    light->SetColor(vec3(1.0f, 0.0f, 0.0f));
    light->SetIntensity(1.0f);

    light2 = new Light(DirectionalLight);
    light2->SetDirection(vec3(-1.0f, 0.0f, -1.0f));
    light2->SetColor(vec3(0.0f, 1.0f, 0.0f));
    light2->SetIntensity(0.5f);
}

float rot = 0.0f, luigiRot = 0.0f, appTime = 0.0f;

void RenderScene()
{
    glDepthFunc(GL_LEQUAL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 model(1.0f), normalMatrix(1.0f);
    appTime += 0.1f;
    //luigiRot += 0.03f;
    vec3 axis(.0, 1.0, 0.0), translate(-0.3f, -0.3f, -1.5f), scale(0.004);
    mat4 T = glm::translate(model, translate);
    mat4 R = glm::rotate_slow(model, rot, axis);
    mat4 S = glm::scale(model, scale);
    model = T * R * S;
    normalMatrix = R * S;

    mat4 projection = perspective(45.0f * 3.1415f/180.0f, 1.0f, 0.1f, 100.0f);
    mesh->GetShaderProgram()->SetUniform("projection", projection);
    mesh->GetShaderProgram()->SetUniform("time", appTime);
    mesh->GetShaderProgram()->SetUniform("model", model);
    mesh->GetShaderProgram()->SetUniform("normalMatrix", normalMatrix);

    gbuffer->Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gbuffer->GetShaderProgram()->SetUniform("time", appTime);
        mesh->Draw();
    gbuffer->UnBind();

    light->Apply(*gbuffer);
    light2->Apply(*gbuffer);
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
            if(event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_RIGHT)) rot += 0.03;
            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_LEFT)) rot -= 0.03;
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
