#include "../include/FileLoader.h"

using namespace RE;


unsigned char* FileLoader::ReadImage(const char *filepath, int &components, int &width, int &height)
{
    unsigned char* data = stbi_load(filepath, &width, &height, &components, 0);
    if(data == nullptr)
    {
        DbgError("Error loading the texture '" << filepath << "', couldn't open/read the file.");
        return data;
    }
    return data;
}

bool FileLoader::ReadMesh(const char *filepath, std::vector<glm::vec3> &vertexPos, std::vector<glm::vec2> &vertexUvs, std::vector<glm::vec3> &vertexNormals, bool &triangles)
{
    return true;
    /*
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filepath, 0);
    aiMesh *mesh = scene->mMeshes[0];

    int numVerts = mesh->mNumFaces*3;

    vertexPos     = std::vector<glm::vec3>(numVerts);
    vertexUvs     = std::vector<glm::vec2>(numVerts);
    vertexNormals = std::vector<glm::vec3>(numVerts);

    for(unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace& face = mesh->mFaces[i];

        for(int j = 0; j < 3; ++j)
        {
            int index = face.mIndices[j];
            aiVector3D uv = mesh->mTextureCoords[0][index];
            vertexUvs.push_back( glm::vec2(uv.x, uv.y) );

            aiVector3D normal = mesh->mNormals[index];
            vertexNormals.push_back( glm::vec3(normal.x, normal.y, normal.z) );

            aiVector3D pos = mesh->mVertices[index];
            vertexPos.push_back( glm::vec3(pos.x, pos.y, pos.z) );
        }
    }
    */
}

void FileLoader::GetOBJFormat(const char *filepath, bool &uvs, bool &normals, bool &triangles)
{
    std::FILE *f;
    f = fopen(filepath, "r");
    if(!f)
    {
        DbgError("Error trying to open '" << filepath << "'");
        return;
    }

    fseek(f, -3, SEEK_END);
    int n = 1;

    char c, lastChar;
    while(ftell(f) > 0)
    {
        lastChar = fgetc(f);
        c = fgetc(f);
        if((lastChar == '\n' || lastChar == '\r') && c == 'f')
        {
            int foo;
            while(fgetc(f) == ' '); //Leemos espacios despues de 'f'
            fseek(f, -1, SEEK_CUR);
            foo = fscanf(f, "%d", &foo); //Leemos primer indice
            if(fgetc(f) == ' ') uvs = normals = false; //Solo un indice, sin barras
            else //Hay algo tal que asi:  5/*
            {
                uvs = (fgetc(f) != '/');
                if(!uvs) normals = true; //Es tal que asi 5//8
                if(uvs) //Es algo tal que asi 5/8*
                {
                    fseek(f, -1, SEEK_CUR);
                    foo = fscanf(f, "%d", &foo); //Leemos segundo indice
                    if(fgetc(f) == '/') //Es algo tal que asi 5/8/11
                    {
                        fseek(f, -1, SEEK_CUR);
                        foo = fscanf(f, "%d", &foo); //Leemos ultimo indice
                        normals = true;
                    }
                    else  normals = false;
                }
            }

            //Son triangulos o quads?
            lastChar = c;
            while(!feof(f) && (c = fgetc(f)) != '\n')
            {
                if(lastChar == ' ' && c != ' ') ++n;
                lastChar = c;
            }
            triangles = (n <= 3);
            break;
        }
        fseek(f, -3, SEEK_CUR);
    }
    fclose(f);
}

bool FileLoader::ReadOBJ(const char *filepath, std::vector<glm::vec3> &vertexPos,
                         std::vector<glm::vec2> &vertexUvs,
                         std::vector<glm::vec3> &vertexNormals,
                         bool &triangles)
{
    std::vector<glm::vec3> disorderedVertexPos, disorderedVertexNormals;
    std::vector<glm::vec2> disorderedVertexUvs;
    std::vector<unsigned int> vertexPosIndexes, vertexUvsIndexes, vertexNormIndexes;
    bool hasUvs, hasNormals;

    GetOBJFormat(filepath, hasUvs, hasNormals, triangles);

    std::ifstream f(filepath, std::ios::in);
    if(!f.is_open()) DbgError("Error opening the mesh file");
    std::string line;

    while(getline(f, line))
    {
        std::stringstream ss(line);
        std::string lineHeader;
        if(!(ss >> lineHeader)) continue;
        if(lineHeader == "v")
        {
            glm::vec3 pos;
            ss >> pos.x;
            ss >> pos.y;
            ss >> pos.z;
            disorderedVertexPos.push_back(pos);
        }
        else if(hasUvs && lineHeader == "vt") //Cargamos uvs
        {
            glm::vec2 uv;
            ss >> uv.x;
            ss >> uv.y;
            disorderedVertexUvs.push_back(uv);
        }
        else if(hasNormals && lineHeader == "vn") //Cargamos normals
        {
            glm::vec3 normal;
            ss >> normal.x;
            ss >> normal.y;
            ss >> normal.z;
            disorderedVertexNormals.push_back(normal);
        }
        else if(lineHeader == "f")
        {
            int n = triangles ? 3 : 4;
            unsigned int index;
            char c;

            for (int i = 0; i < n; ++i)
            {
                ss >> index;
                vertexPosIndexes.push_back(index);

                if(hasUvs)
                {
                    ss >> c;  //Read the '/'
                    ss >> index;
                    vertexUvsIndexes.push_back(index);

                    if (hasNormals)
                    {
                        ss >> c;
                        ss >> index;
                        vertexNormIndexes.push_back(index);
                    }
                }
                else
                {
                    if (hasNormals)
                    {
                        ss >> c;
                        ss >> index;
                        vertexNormIndexes.push_back(index);
                    }
                }
            }
        }
    }

    for(int i = 0; i < vertexPosIndexes.size(); ++i)
    {
        vertexPos.push_back(disorderedVertexPos[vertexPosIndexes[i]-1]);
    }

    if(hasUvs)
    {
        for(int i = 0; i < vertexUvsIndexes.size(); ++i)
        {
            vertexUvs.push_back(disorderedVertexUvs[vertexUvsIndexes[i]-1]);
        }
    }

    if(hasNormals)
    {
        for(int i = 0; i < vertexNormIndexes.size(); ++i)
        {
            vertexNormals.push_back(disorderedVertexNormals[vertexNormIndexes[i]-1]);
        }
    }

    return true;
}

