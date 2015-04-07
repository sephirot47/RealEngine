#include "../include/Scene.h"

using namespace RE;

Scene::Scene()
{
    StateManager::Push();

    width = 1500;
    height = 800;

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDepthFunc(GL_LEQUAL);

    camera = nullptr;
    gbuffer = new GBuffer(width, height);
}

Scene::~Scene()
{
    StateManager::Pop();
}

void Scene::AddGameObject(GameObject &go)
{
    gameObjects.insert( GameObjectPair(go.GetId(), &go) );
}

void Scene::RemoveGameObject(GameObject &go)
{
    auto it = gameObjects.find(go.GetId());
    if(it != gameObjects.end()) gameObjects.erase(it);
}

void Scene::Render()
{
    gbuffer->ClearColorDepth();

    for(auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
    {
        GameObject *go = it->second;
        if(go->HasComponent<Mesh>())
        {
            if(go->HasComponent<Material>())
            {
                if(camera)
                {
                    go->GetComponent<Mesh>()->Render(*gbuffer, *go->GetComponent<Material>(), *camera);
                }
                else
                {
                    glm::mat4 view, projection;
                    go->GetComponent<Mesh>()->Render(*gbuffer, *go->GetComponent<Material>(), view, projection);
                }
            }
        }
    }

    gbuffer->RenderToScreen();
}

void Scene::SetCamera(Camera &camera)
{
    this->camera = &camera;
}
