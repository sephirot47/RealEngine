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

    // FIRST PASS: FILL GBUFFER and FILL SHADOW MAP OF EVERY LIGHT ///////////
    for(auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
    {
        GameObject *go = it->second;
        if(go->HasComponent<Mesh>())
        {
            Mesh *mesh = go->GetComponent<Mesh>();
            if(go->HasComponent<Material>())
            {
                Material *material = go->GetComponent<Material>();

                glm::mat4 model;

                Transform *transform;
                if(go->HasComponent<Transform>())
                {
                    transform = go->GetComponent<Transform>();
                    model = transform->GetModelMatrix();
                }
                else transform = new Transform();

                if(camera)
                {
                    mesh->Render(*gbuffer, *material, model, *camera);
                }
                else
                {
                    glm::mat4 view, projection;
                    mesh->Render(*gbuffer, *material, model, view, projection);
                }

                if(not go->HasComponent<Transform>()) delete transform;
            }
        }

        if(go->HasComponent<Light>())
        {
            Light *light = go->GetComponent<Light>();
            for(auto it2 = gameObjects.begin(); it2 != gameObjects.end(); ++it2)
            {
                GameObject *go2 = it2->second;
                if(go == go2) continue;

                Transform *transform;
                glm::mat4 model;
                if(go->HasComponent<Transform>())
                {
                    transform = go->GetComponent<Transform>();
                    model = transform->GetModelMatrix();
                }
                else transform = new Transform();

                light->ShadowMapMesh(*go2->GetComponent<Mesh>(), model, *transform, width, height);

                if(not go->HasComponent<Transform>()) delete transform;
            }
        }
    }
    // END FIRST PASS /////////////////////////////////////////

    // SECOND PASS: LIGHT EVERYTHING //////////////////////////
    for(auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
    {
        GameObject *go = it->second;
        if(go->HasComponent<Light>())
        {

           Light *light = go->GetComponent<Light>();
           light->ClearShadowMap();


           Transform *transform;
           if(go->HasComponent<Transform>()) transform = go->GetComponent<Transform>();
           else transform = new Transform();

           light->Render(*gbuffer, *transform, *camera);

           if(not go->HasComponent<Transform>()) delete transform;
        }
    }
    // END SECOND PASS /////////////////////////////////////////

    gbuffer->RenderToScreen();
}

void Scene::SetCamera(Camera &camera)
{
    this->camera = &camera;
}
