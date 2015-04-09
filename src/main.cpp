
#include "../include/RealEngine.h"

using namespace std;
using namespace glm;
using namespace RE;

const float width = 1500, height = 800;

GBuffer *gbuffer;
Mesh *mesh1, *mesh2, *mesh3;
Light *light;
Material *material1, *material2, *material3;

Camera *camera;
SkyBox *skybox;

Scene *scene;
GameObject *go1, *go2, *go3;

void Init()
{
    mesh1 = new Mesh();
    mesh1->LoadFromFile("Assets/Models/luigi.obj");
    Texture *texture1 = new Texture("Assets/Textures/luigiD.jpg");
    material1 = new Material();
    material1->SetTexture(*texture1);

    mesh2 = new Mesh();
    mesh2->LoadFromFile("Assets/Models/gordaco.obj");
    Texture *texture2 = new Texture("Assets/Textures/gordaco.bmp");
    material2 = new Material();
    material2->SetTexture(*texture2);

    mesh3 = new Mesh();
    mesh3->LoadFromFile("Assets/Models/quad.obj");
    Texture *texture3 = new Texture("Assets/Textures/wall.jpg");
    material3 = new Material();
    material3->SetTexture(*texture3);

    camera = new Camera();

    //Creamos el cielo
    Image *cm1, *cm2, *cm3, *cm4, *cm5, *cm6;
    cm1 = new Image("Assets/Textures/sky2/posx.JPG");
    cm2 = new Image("Assets/Textures/sky2/negx.JPG");
    cm3 = new Image("Assets/Textures/sky2/posy.JPG");
    cm4 = new Image("Assets/Textures/sky2/negy.JPG");
    cm5 = new Image("Assets/Textures/sky2/posz.JPG");
    cm6 = new Image("Assets/Textures/sky2/negz.JPG");

    CubeMap *skyCubeMap = new CubeMap();
    skyCubeMap->SetFaceTexture(CubeMap::Face::PositiveX, *cm1);
    skyCubeMap->SetFaceTexture(CubeMap::Face::NegativeX, *cm2);
    skyCubeMap->SetFaceTexture(CubeMap::Face::PositiveY, *cm3);
    skyCubeMap->SetFaceTexture(CubeMap::Face::NegativeY, *cm4);
    skyCubeMap->SetFaceTexture(CubeMap::Face::PositiveZ, *cm5);
    skyCubeMap->SetFaceTexture(CubeMap::Face::NegativeZ, *cm6);

    skybox = new SkyBox();
    skybox->SetCubeMap(*skyCubeMap);
    //

    gbuffer = new GBuffer(width, height);

    GameObject *lightGo1 = new GameObject();
    lightGo1->GetTransform()->SetPosition(vec3(0.0f, 0.0f, 50.0f));
    lightGo1->GetTransform()->SetRotation(Quaternion::LookAt( lightGo1->GetTransform()->GetPosition(),
                                                              glm::vec3(0), glm::vec3(0,1,0) )  );
    light = new Light(Light::Type::DirectionalLight, width, height);
    lightGo1->AddComponent(*light);

    camera->SetMode(Camera::Mode::Perspective);
    camera->SetPosition(vec3(0, 50, 500));
    camera->SetPerspective(45.0f, width/height, 300.0f, 1500.0f);

    //SCENE STUFF

    go1 = new GameObject();
    go1->AddComponent(*mesh1);
    go1->AddComponent(*material1);
    go1->GetTransform()->SetPosition(vec3(0, -10, -200));

    go2 = new GameObject();
    go2->AddComponent(*mesh2);
    go2->AddComponent(*material2);
    go2->GetTransform()->SetPosition(vec3(0, -10, -450));
    go2->GetTransform()->SetScale(vec3(2));

    go3 = new GameObject();
    go3->AddComponent(*mesh3);
    go3->AddComponent(*material3);
    go3->GetTransform()->SetPosition(vec3(0, 0, -850));
    go3->GetTransform()->SetScale(vec3(500));

    scene = new Scene();
    scene->AddGameObject(*lightGo1);
    scene->AddGameObject(*go1);
    //scene->AddGameObject(*go2);
    scene->AddGameObject(*go3);
    scene->SetCamera(*camera);

    /*
    go3->GetComponent<Material>()->SetTexture(
                *lightGo1->GetComponent<Light>()->GetShadowBuffer()->GetTexture(GL_DEPTH_ATTACHMENT) );
    */
}

void RenderScene()
{    
    scene->Render();
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
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
    context = SDL_GL_CreateContext(win);

    float totalTime = 0.0f;
    int frames = 0;

    Init();
    bool running = true;
    while(running)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            float camRot   = 0.05f;
            float camSpeed = 30.0f;

            if(event.type == SDL_QUIT) running = false;

            if (event.type == SDL_KEYDOWN && (IsPressed(SDLK_RIGHT) || IsPressed(SDLK_d)) )
                camera->SetPosition(camera->GetPosition() + camSpeed * camera->GetRight());
            if (event.type == SDL_KEYDOWN && (IsPressed(SDLK_LEFT) || IsPressed(SDLK_a)) )
                camera->SetPosition(camera->GetPosition() - camSpeed * camera->GetRight());

            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_UP))
                camera->SetPosition(camera->GetPosition() + camSpeed * camera->GetForward());
            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_DOWN))
                camera->SetPosition(camera->GetPosition() - camSpeed * camera->GetForward());

            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_w))
                camera->SetRotation(camera->GetRotation() * Quaternion::AxisAngle(vec3(1,0,0), camRot));
            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_s))
                camera->SetRotation(camera->GetRotation() * Quaternion::AxisAngle(vec3(1,0,0), -camRot));


            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_a))
                camera->SetRotation(camera->GetRotation() * Quaternion::AxisAngle(vec3(0,1,0), camRot));
            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_d))
                camera->SetRotation(camera->GetRotation() * Quaternion::AxisAngle(vec3(0,1,0), -camRot));

            if (event.type == SDL_KEYDOWN && IsPressed(SDLK_1)) light->SetEnabled(!light->GetEnabled());
        }

        float t = Time::GetMiliseconds();

        RenderScene();
        SDL_GL_SwapWindow(win);

        totalTime += Time::GetMiliseconds() - t;
        float fps = float(++frames)/totalTime;
       // if(frames % 3 == 0) DbgLog(int(fps * 1000.0f) << " fps" );
        if(frames > 100) frames = totalTime = 0;
        //SDL_Delay(10);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
