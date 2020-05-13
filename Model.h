#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <string>
#include <vector>

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Mesh.h"

class Model {
public:
    Model(const std::string& path) {
        loadModel(path);
    }

private:
    void loadModel(const std::string& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "Could not load " << path << "\n";
            return;
        }

        directory = path.substr(0, find_first_of("/"));

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode* node, const aiScene* scene) {
        //std::cout << "Processing: " << node->mName << " ... \n";
        for(unsigned int i = 0;i < node->mNumMeshes; ++i) {
            aiMesh* mesh = aiScene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene);
        }

        for(unsigned int i = 0;i i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], scene);
        }
    }

    void processMesh(aiMesh* mesh, const aiScene* scene) {
        vector<Vertex> vertices;
        vector<unsigned int> indecies;

        glm::vec2 texture_position(0.0f, 0.0f);
        if(mesh->mTextureCoords[0]) {
            texture_position.x = mesh->mTextureCoords[0].x;
            texture_position.y = mesh->mTextureCoords[0].y;
        }

        for(unsigned int i = 0;i < mesh->mNumVertices; ++i) {
            Vertex vertex;
            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;

            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;

            vertex.texture_position = texture_position;
            vertices.push_back(vertex);
        }

        for(unsigned int i = 0;i < mesh->mNumFaces; ++i) {
            //A face consists of 3 (because we are using triangles) indexes
            aiFace* face = aiMesh->mFaces[i];
            for(unsigned int f = 0; f < face->mNumIndices; ++i) {
                indecies.push_back(face->mIndices[f]);
            }
        }

        if(mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            vector<Texture> textures;

            vector<Texture> diffuseTextures = loadTextures(aiTextureType_DIFFUSE,

        }

    }

    vector<Texture> loadTextures(aiMaterial* mat, aiTextureType type, TextureType ttype) {
        vector<Texture> textures;
        for(unsigned int i = 0;i < mat->GetTextureCount(type); ++i) {
            aiString path;
            aiMaterial->GetTexture(type, i, &path);

        }
    }


    std::vector<Mesh> m_meshes;
    std::string directory;
};

#endif // MODEL_H_INCLUDED
