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
            Mesh *mesh = go->GetComponent<Mesh>();
            if(go->HasComponent<Material>())
            {
                Material *material = go->GetComponent<Material>();
                if(camera)
                {
                    if(go->HasComponent<Transform>())
                    {
                        Transform *transform = go->GetComponent<Transform>();

                        glm::mat4 model;
                        glm::mat4 T = glm::translate(model, transform->position);
                        glm::mat4 R = glm::mat4_cast(transform->rotation);
                        glm::mat4 S = glm::scale(model, transform->scale);

                        model = T * R * S;
                        mesh->SetModelMatrix(model);
                    }

                    mesh->Render(*gbuffer, *material, *camera);
                }
                else
                {
                    glm::mat4 view, projection;
                    mesh->Render(*gbuffer, *material, view, projection);
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
