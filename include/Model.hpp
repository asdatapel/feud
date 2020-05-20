#ifndef FEUD_MODEL_HPP
#define FEUD_MODEL_HPP

#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Model
{

    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        std::vector<glm::vec2> uvs;
    };
    struct Mesh
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        unsigned int uvCount;
    };

    std::vector<Model::Mesh> meshes;

    static Model load(std::string filename)
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        }

        std::vector<Mesh> meshes;
        std::queue<aiNode *> children;

        children.push(scene->mRootNode);

        while (!children.empty())
        {
            aiNode *node = children.front();
            children.pop();
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(processMesh(mesh));
            }
            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                children.push(node->mChildren[i]);
            }
        }

        return Model{meshes};
    };

    static Model::Mesh processMesh(aiMesh *mesh)
    {
        std::vector<Model::Vertex> vertices;
        std::vector<unsigned int> indices;

        unsigned int numUvLayers = 0;
        while (mesh->mTextureCoords[numUvLayers])
        {
            numUvLayers += 1;
        }

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Model::Vertex vertex;
            glm::vec3 vector;

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;

            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;

            for (int uvLayer = 0; uvLayer < numUvLayers; ++uvLayer)
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[uvLayer][i].x;
                vec.y = mesh->mTextureCoords[uvLayer][i].y;
                vertex.uvs.push_back(vec);
            }

            vertices.push_back(vertex);
        }

        return Model::Mesh{vertices, indices, numUvLayers};
    }
};

#endif //FEUD_MODEL_HPP
