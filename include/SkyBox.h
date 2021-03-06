#ifndef SKYBOX_H
#define SKYBOX_H

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "StateManager.h"
#include "Camera.h"
#include "Debug.h"
#include "ShaderProgram.h"
#include "CubeMap.h"
#include "Mesh.h"
#include "glm/glm.hpp"

namespace RE
{

class SkyBox
{
private:

    Mesh *cubeMesh;
    ShaderProgram *program;
    CubeMap *cubeTexture;

public:

    SkyBox();
    ~SkyBox();

    void Render(RenderTarget &renderTarget, Camera &camera);
    void Render(RenderTarget &renderTarget, glm::mat4 &camView, glm::mat4 &camProjection);
    void SetCubeMap(CubeMap &cubeTexture);

    Mesh *GetMesh() const;
    ShaderProgram *GetShaderProgram() const;
    CubeMap *GetCubeMap() const;
};

}

#endif // SKYBOX_H
