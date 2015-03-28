#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <cstring>
#include <stdio.h>

#include "../include/RealEngine.h"

using namespace std;
using namespace glm;
using namespace RE;

const float width = 1500, height = 800;

GBuffer *gbuffer;
Mesh *mesh1, *mesh2, *mesh3;
Light *light, *light2;
Material *material1, *material2, *material3;
vec3 cameraRot, cameraPos;

void Init()
{
    glClearColor(0.0, 0.0, 0.3, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDepthFunc(GL_LEQUAL);

    mesh1 = new Mesh();
    mesh1->LoadFromFile("Assets/Models/luigi.obj");
    Texture *texture1 = new Texture();
    texture1->LoadFromFile("Assets/Textures/luigiD.jpg");
    material1 = new Material();
    material1->SetTexture(*texture1);

    mesh2 = new Mesh();
    mesh2->LoadFromFile("Assets/Models/gordaco.obj");
    Texture *texture2 = new Texture();
    texture2->LoadFromFile("Assets/Textures/gordaco.bmp");
    material2 = new Material();
    material2->SetTexture(*texture2);

    mesh3 = new Mesh();
    mesh3->LoadFromFile("Assets/Models/quad.obj");
    Texture *texture3 = new Texture();
    texture3->LoadFromFile("Assets/Textures/wall.jpg");
    material3 = new Material();
    material3->SetTexture(*texture3);

    gbuffer = new GBuffer(width, height);
    gbuffer->SetFragmentFinalColorTextureName("finalColors");
    gbuffer->SetFragmentMaterialTextureTextureName("textureColors");
    gbuffer->SetFragmentPositionTextureName("pos");
    gbuffer->SetFragmentUvTextureName("uvs");
    gbuffer->SetFragmentNormalsTextureName("normals");
    gbuffer->SetFragmentDepthTextureName("depth");

    light = new Light(DirectionalLight, width, height);
    light->SetPosition(vec3(10.0f, 0.5f, 10.0f));
    light->SetDirection(-light->GetPosition());
    light->SetColor(vec3(1.0f, 1.0f, 1.0f));
    light->SetIntensity(1.0f);

    light2 = new Light(PointLight, width, height);
    light2->SetPosition(vec3(0.0f, 0.0f, 2.0f));
    light2->SetDirection(-light2->GetPosition());
    light2->SetColor(vec3(1.0f, 0.0f, 0.0f));
    light2->SetIntensity(1.0f);
    light2->SetRange(2.0f);

    cameraPos = vec3(0, 0, 7.0f);
    cameraRot = vec3(0, 0, 0.0f);
}

float rot = 0.0f, sphereRot = 0.0f, appTime = 0.0f;
bool lightMode = false;

void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    appTime += 0.03f;
    sphereRot += 0.03f;
    light->SetPosition(vec3(sin(appTime) * 7.0f, 0.5f, 10.0f));
    light->SetDirection(-light->GetPosition());

    light2->SetRange((sin(appTime)*0.5+0.5) * 10.0f);
    light2->SetDirection(-light2->GetPosition());

    mat4 model(1.0f);
    vec3 axis(.0, 1.0, 0.0), translate, scale;
    mat4 T = glm::translate(model,  translate);
    mat4 R = glm::rotate_slow(model, rot, axis);
    mat4 S = glm::scale(model, scale);

    gbuffer->Bind();
        gbuffer->ClearColorDepth();

        mat4 projection = perspective(45.0f * 3.1415f/180.0f, width/height, 1.0f, 20.0f);
        if(lightMode) projection = light2->GetProjection(width, height);

        model = mat4(1.0f);
        translate = vec3(sin(appTime) * 0.8f, 0.0f, 3.0f);
        scale = vec3(0.005f);
        T = glm::translate(model, translate);
        R = glm::rotate_slow(model, sphereRot, axis);
        S = glm::scale(model, scale);
        mesh1->SetModelMatrix(T * R * S);
        mesh1->SetNormalMatrix(R * S);

        model = mat4(1.0f);
        translate = vec3(-0.0f, -0.3f, 0.0f);
        scale = vec3(0.02f);
        T = glm::translate(model, translate);
        R = glm::rotate_slow(model, 0.0f, axis);
        S = glm::scale(model, scale);
        mesh2->SetModelMatrix(T * R * S);
        mesh2->SetNormalMatrix(R * S);

        model = mat4(1.0f);
        translate = vec3(0.0f, 0.5f, -4.0f);
        scale = vec3(0.005f);
        T = glm::translate(model, translate);
        R = glm::rotate_slow(model, 0.0f, axis);
        scale = vec3(6.0f);
        S = glm::scale(model, scale);
        mesh3->SetModelMatrix(T * R * S);
        mesh3->SetNormalMatrix(R * S);

        mat4 view = mat4(1.0f);
        T = glm::translate(view, cameraPos);
        R = glm::rotate_slow(view, rot, axis);
        view = glm::inverse(T * R);
        if(lightMode) view = light2->GetView();

        mesh1->Draw(*material1, projection, view);
        mesh2->Draw(*material2, projection, view);
        mesh3->Draw(*material3, projection, view);

        light->ClearBufferMeshShadow();
        light->BufferMeshShadow(*mesh1, width, height);
        light->BufferMeshShadow(*mesh2, width, height);
        light->BufferMeshShadow(*mesh3, width, height);

        light2->ClearBufferMeshShadow();
        light2->BufferMeshShadow(*mesh1, width, height);
        light2->BufferMeshShadow(*mesh2, width, height);
        light2->BufferMeshShadow(*mesh3, width, height);

        light->ApplyLight(*gbuffer, view, projection);
        light2->ApplyLight(*gbuffer, view, projection);

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
            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_s)) cameraPos.y -= camSpeed;
            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_w)) cameraPos.y += camSpeed;
            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_0)) lightMode = !lightMode;
            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_1)) light->SetEnabled(!light->GetEnabled());
            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_2)) light2->SetEnabled(!light2->GetEnabled());
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
