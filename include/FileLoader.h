#ifndef FILELOADER_H
#define FILELOADER_H

#include <vector>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include "stdlib.h"

#include "Debug.h"
/*
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>*/
#include "stb_image.h"
#include "glm/glm.hpp"


namespace RE
{

class FileLoader
{

public:

    static unsigned char* ReadImage(const char *filepath, int &components, int &width, int &height);
    static bool ReadMesh(const char *filepath,
                         std::vector<glm::vec3> &vertexPos,
                         std::vector<glm::vec2> &vertexUvs,
                         std::vector<glm::vec3> &vertexNormals,
                         bool &triangles);


    static void GetOBJFormat(const char *filepath, bool &uvs, bool &normals, bool &triangles);
    static bool ReadOBJ(const char *filepath,
                        std::vector<glm::vec3> &vertexPos,
                        std::vector<glm::vec2> &vertexUvs,
                        std::vector<glm::vec3> &vertexNormals,
                        bool &triangles);
};

}

#endif // FILELOADER_H
