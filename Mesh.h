#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED


#include <glm/glm.hpp>
#include <vector>

#include "Program.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_position;
};

enum class TextureType {
    DIFFUSE,
    SPECULAR,
    NORMAL,
    HEIGHT
};

struct Texture {
    unsigned int id;
    TextureType type;
};

std::string textureTypeToString(TextureType type) {
    switch(type) {
    case TextureType::DIFFUSE:  return "diffuse";
    case TextureType::SPECULAR: return "specular";
    case TextureType::NORMAL: return "normal";
    case TextureType::HEIGHT: return "height";
    }

    return "";
}

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<unsigned int>& indecies,
         const std::vector<Texture> textures): m_vertices(vertices),
                                               m_indecies(indecies),
                                               m_textures(textures) {
        initMesh();
    }

    void draw(Program program) {

        int diffuseTextureIndex = 0;
        int specularTextureIndex = 0;

        for(auto i = 0u; i < m_textures.size(); ++i) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, m_textures[i].id);

            int index = 31;

            switch(m_textures[i].type) {
            case TextureType::DIFFUSE: index = diffuseTextureIndex++; break;
            case TextureType::SPECULAR: index = specularTextureIndex++; break;
            }

            std::string uniformName = "material." + textureTypeToString(m_textures[i].type) + std::to_string(index);

            glUniform1i(glGetUniformLocation(program.getProgramID(), uniformName.c_str()), i);
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, m_indecies.size(), GL_UNSIGNED_INT, NULL);
        glBindVertexArray(0);

    }

private:
    void initMesh() {

        // PREPARING
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_position));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        // EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indecies.size(), &m_indecies[0], GL_STATIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    }

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indecies;
    std::vector<Texture> m_textures;

    unsigned int VAO, VBO, EBO;
};

#endif // MESH_H_INCLUDED
