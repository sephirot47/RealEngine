#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <cstring>
#include <stdio.h>

#include "../include/RealEngine.h"

using namespace std;

const int width = 800, height = 800;

VAO *vao;
VBO *vbo;
Shader *fshader, *vshader;
ShaderProgram *program;
Texture *texture;

FrameBuffer* frameBuffer;
FrameDrawer* frameDrawer;

vector<vec3> pos, normals;
vector<vec2> uv;
bool tris;

void Init()
{
    glClearColor(0.0, 0.0, 0.3, 1.0);
    glEnable(GL_DEPTH_TEST);

    FileLoader::ReadOBJ("luigi.obj", pos, uv, normals, tris);

    vbo = new VBO();
    int size = pos.size() * sizeof(vec3) + uv.size() * sizeof(vec2) + normals.size() * sizeof(vec3);
    char *data = new char [size];
    memcpy(data, &pos[0], pos.size() * sizeof(vec3));
    memcpy(data + pos.size() * sizeof(vec3),
           &uv[0], uv.size() * sizeof(vec2));
    memcpy(data + pos.size() * sizeof(vec3) + uv.size() * sizeof(vec2),
           &normals[0], normals.size() * sizeof(vec3));

    vbo->SetData(&data[0], size);
    delete [] data;

    vao = new VAO();
    vao->AddAttribute(*vbo, 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    vao->AddAttribute(*vbo, 1, 2, GL_FLOAT, GL_FALSE, 0, pos.size() * sizeof(vec3));
    vao->AddAttribute(*vbo, 2, 3, GL_FLOAT,  GL_TRUE, 0, pos.size() * sizeof(vec3) + uv.size() * sizeof(vec2));

    Image *img = new Image(); img->LoadFromFile("luigiD.jpg");
    texture = new Texture();
    texture->SetData(img->GetData(), img->GetWidth(), img->GetHeight(), img->GetFormat(), img->GetFormat(), GL_UNSIGNED_BYTE);

    fshader = new Shader(); fshader->Create("fshader", GL_FRAGMENT_SHADER);
    vshader = new Shader(); vshader->Create("vshader", GL_VERTEX_SHADER);
    program = new ShaderProgram();
    program->AttachShader(*fshader);
    program->AttachShader(*vshader);
    program->Link();

    program->AttachTexture("tex", *texture);

    ///FRAME BUFFER STUFF////
    frameBuffer = new FrameBuffer(width, height);
    frameDrawer = new FrameDrawer(*frameBuffer);
    /////////////////////////
}

float rot = 0.0f, appTime = 0.0f;

void RenderScene()
{
    frameBuffer->Bind();
    vao->Bind();
    program->Use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mat4 model(1.0f);
    appTime += 0.1f;
   // rot += 0.03f;
    vec3 axis(.0, 1.0, 0.0), translate(0.0f, -0.3f, -1.5f), scale(0.006);
    mat4 T = glm::translate(model, translate);
    mat4 R = glm::rotate_slow(model, rot, axis);
    mat4 S = glm::scale(model, scale);
    model = T * R * S;

    mat4 projection = perspective(45.0f * 3.1415f/180.0f, 1.0f, 0.1f, 100.0f);
    program->SetUniform("projection", projection);
    program->SetUniform("time", appTime);
    program->SetUniform("model", model);

    glDrawArrays(tris ? GL_TRIANGLES : GL_QUADS, 0, pos.size());

    program->UnUse();
    vao->UnBind();
    frameBuffer->UnBind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    frameDrawer->GetProgram()->SetUniform("time", appTime);
    frameDrawer->Draw();
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
