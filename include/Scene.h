#ifndef SCENE_H
#define SCENE_H

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <map>
#include <vector>

#include "Debug.h"
#include "GBuffer.h"
#include "Transform.h"
#include "GameObject.h"
#include "StateManager.h"

#include "glm/glm.hpp"
#include "stb_image.h"

using namespace std;

namespace RE
{

class Scene
{
private:

    int width, height;

    typedef std::pair<int, GameObject*> GameObjectPair;
    typedef std::map<int, GameObject*> GameObjectMap;
    GameObjectMap gameObjects;

    Camera *camera;

    GBuffer *gbuffer;

public:
    Scene();
    ~Scene();

    void AddGameObject(GameObject &go);
    void RemoveGameObject(GameObject &go);

    void Render();

    void SetCamera(Camera &camera);
};

}

#endif // SCENE_H
