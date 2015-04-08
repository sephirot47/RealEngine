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
                if(go->HasComponent<Transform>())
                    model = go->GetComponent<Transform>()->GetModelMatrix();

                if(camera)
                {
                    mesh->Render(*gbuffer, *material, model, *camera);
                }
                else
                {
                    glm::mat4 view, projection;
                    mesh->Render(*gbuffer, *material, model, view, projection);
                }
            }
        }

        // CREATE SHADOW MAP ///////////////////////////////////////
        if(go->HasComponent<Light>())
        {
            Light *light = go->GetComponent<Light>();
            light->ClearShadowMap();
            for(auto it2 = gameObjects.begin(); it2 != gameObjects.end(); ++it2)
            {
                GameObject *go2 = it2->second;
                if(go != go2)
                {
                    if(go2->HasComponent<Mesh>())
                    {
                        Transform *transform;
                        if(go2->HasComponent<Transform>()) transform = go2->GetComponent<Transform>();
                        else transform = new Transform();

                        light->ShadowMapMesh(*go2->GetComponent<Mesh>(), *transform, width, height);

                        if(not go2->HasComponent<Transform>()) delete transform;
                    }
                }
            }
        }
        // END CREATE SHADOW MAP ////////////////////////////////////


    }
    // END FIRST PASS /////////////////////////////////////////



    // SECOND PASS: LIGHT EVERYTHING //////////////////////////
    for(auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
    {
        GameObject *go = it->second;
        if(go->HasComponent<Light>())
        {
           Light *light = go->GetComponent<Light>();

           Transform *transform;
           if(go->HasComponent<Transform>()) transform = go->GetComponent<Transform>();
           else transform = new Transform();

           light->Render(*gbuffer, *transform, *camera);

           if(not go->HasComponent<Transform>()) delete transform;
        }
    }
    // END SECOND PASS /////////////////////////////////////////


    //RENDER
    gbuffer->RenderToScreen();
}

void Scene::SetCamera(Camera &camera)
{
    this->camera = &camera;
}
