#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <map>
#include <string>
#include <vector>

using std::map;
using std::vector;
using std::string;

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Mesh.h"

class Model {
public:
    Model(const string& path) {
        loadModel(path);
    }

    void draw(Program program) {
        for(auto i = 0u; i < m_meshes.size(); ++i) {
            m_meshes[i].draw(program);
        }
    }

private:
    void loadModel(const string& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "Could not load " << path << "\n";
            return;
        }

        m_directory = path.substr(0, path.find_first_of("/"));

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode* node, const aiScene* scene) {
        std::cout << "Processing: " << node->mName.C_Str() << " ... \n";
        for(unsigned int i = 0;i < node->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene);
        }

        for(unsigned int i = 0;i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], scene);
        }
    }

    void processMesh(aiMesh* mesh, const aiScene* scene) {
        vector<Vertex> vertices;
        vector<unsigned int> indecies;
        vector<Texture> textures;


        glm::vec2 texture_position(0.0f, 0.0f);
        if(mesh->mTextureCoords[0]) {
            texture_position.x = mesh->mTextureCoords[0]->x;
            texture_position.y = mesh->mTextureCoords[0]->y;
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
            aiFace face = mesh->mFaces[i];
            for(unsigned int f = 0; f < face.mNumIndices; ++f) {
                indecies.push_back(face.mIndices[f]);
            }
        }

        if(mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            vector<Texture> diffuseTextures = loadTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
            textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

            vector<Texture> specularTextures = loadTextures(material, aiTextureType_SPECULAR, TextureType::DIFFUSE);
            textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());
        }

        m_meshes.push_back(Mesh(vertices, indecies, textures));
    }

    vector<Texture> loadTextures(aiMaterial* mat, aiTextureType type, TextureType ttype) {
        vector<Texture> textures;
        for(unsigned int i = 0;i < mat->GetTextureCount(type); ++i) {
            Texture texture;
            aiString path;
            mat->GetTexture(type, i, &path);
            auto iter = m_loadedTextures.find(path.C_Str());
            if(iter != m_loadedTextures.end()) {
                texture = iter->second;
            } else {
                texture.id = loadText(path.C_Str());
                texture.type = ttype;

                m_loadedTextures[path.C_Str()] = texture;
            }

            textures.push_back(texture);
        }

        return textures;
    }

    unsigned int loadText(const char* path) {
        string fullPath = m_directory + "/" + path;

        int width, height, channels;
        unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &channels, 0);
        if(data == NULL) {
            return 0;
        }

        unsigned int target;
        glGenTextures(1, &target);
        glBindTexture(GL_TEXTURE_2D, target);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLint internalFormat;

        if(channels == 1) {
            internalFormat = GL_RED;
        } else if(channels == 2) {
            internalFormat= GL_RG;
        } else if(channels == 3) {
            internalFormat = GL_RGB;
        } else if(channels == 4) {
            internalFormat = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);


        stbi_image_free(data);
        return target;
    }


    vector<Mesh> m_meshes;
    map<string, Texture> m_loadedTextures;
    string m_directory;
};

#endif // MODEL_H_INCLUDED
