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
VAO *fbvao;
VBO *fbvbo;
Shader *fbfshader, *fbvshader;
ShaderProgram *fbProgram;

vector<vec3> pos, normals;
vector<vec2> uv;
bool tris;

void Init()
{
    glClearColor(0.0, 0.0, 0.3, 1.0);
    glEnable(GL_DEPTH_TEST);

    ReadOBJ("luigi.obj", pos, uv, normals, tris);

    vbo = new VBO();
    int size = pos.size() * sizeof(vec3) + uv.size() * sizeof(vec2);
    char *data = new char [size];
    memcpy(data, &pos[0], pos.size() * sizeof(vec3));
    memcpy(data + pos.size() * sizeof(vec3), &uv[0], uv.size() * sizeof(vec2));
    vbo->SetData(&data[0], size);
    delete [] data;

    vao = new VAO();
    vao->AddAttribute(*vbo, 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    vao->AddAttribute(*vbo, 1, 2, GL_FLOAT, GL_FALSE, 0, pos.size() * sizeof(vec3));

    Image *img = new Image(); img->LoadFromFile("luigiD.jpg");
    texture = new Texture();
    texture->SetData(img->GetData(), img->GetWidth(), img->GetHeight(), img->GetFormat(), img->GetFormat(), GL_UNSIGNED_BYTE);

    fshader = new Shader(); fshader->Create("fshader", GL_FRAGMENT_SHADER);
    vshader = new Shader(); vshader->Create("vshader", GL_VERTEX_SHADER);
    program = new ShaderProgram();
    program->AttachShader(*fshader);
    program->AttachShader(*vshader);
    program->Link();

    ///FRAME BUFFER STUFF////
    frameBuffer = new FrameBuffer(width, height);

    fbvbo = new VBO();
    fbvbo->SetData(FrameBuffer::screenMesh, sizeof(FrameBuffer::screenMesh));

    fbvao = new VAO();
    fbvao->AddAttribute(*fbvbo, 0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    fbfshader = new Shader(); fbfshader->Create("fbfshader", GL_FRAGMENT_SHADER);
    fbvshader = new Shader(); fbvshader->Create("fbvshader", GL_VERTEX_SHADER);
    fbProgram = new ShaderProgram();
    fbProgram->AttachShader(*fbfshader);
    fbProgram->AttachShader(*fbvshader);
    fbProgram->Link();
    ///////////////////////////
}

void RenderScene()
{
    frameBuffer->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    vao->Bind();
    program->Use();
    texture->Bind(0);

    mat4 model(1.0f);
    static float rot, time;
    time += 0.1f;
    rot += (float)(rand()%1000)/2000.0f;
    vec3 axis(sin(time), 1.0, cos(time)), translate(sin(time) * 0.1f, -0.2, cos(time) * 0.1f - 1.0f), scale(0.004);
    mat4 T = glm::translate(model, translate);
    mat4 R = glm::rotate_slow(model, rot, axis);
    mat4 S = glm::scale(model, scale);
    model = T * R * S;

    mat4 projection = perspective(45.0f * 3.1415f/180.0f, 1.0f, 0.1f, 100.0f);

    program->SetUniform("tex", 0);
    program->SetUniform("time", time);
    program->SetUniform("model", model);
    program->SetUniform("projection", projection);

    glDrawArrays(tris ? GL_TRIANGLES : GL_QUADS, 0, pos.size());

    texture->UnBind(0);
    program->UnUse();
    vao->UnBind();
    frameBuffer->UnBind();

    //Render to screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fbvao->Bind();
    fbProgram->Use();
    frameBuffer->GetColorTexture()->Bind(0);

    fbProgram->SetUniform("scene", 0);
    glDrawArrays(GL_QUADS, 0, 4);

    frameBuffer->GetColorTexture()->UnBind(0);
    fbProgram->UnUse();
    fbvao->UnBind();
    //
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
