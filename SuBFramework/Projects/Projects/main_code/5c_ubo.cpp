#include <iostream>

#include "sb7.h"
#include "Program.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class UBO_App: public sb7::application {
public:

  virtual void startup() {

    m_program1.loadShaders("Shaders/Chapter5/ubo.vert", "Shaders/Chapter5/ubo_1.frag");
    m_program2.loadShaders("Shaders/Chapter5/ubo.vert", "Shaders/Chapter5/ubo_2.frag");
    m_program3.loadShaders("Shaders/Chapter5/ubo.vert", "Shaders/Chapter5/ubo_3.frag");
    m_program4.loadShaders("Shaders/Chapter5/ubo.vert", "Shaders/Chapter5/ubo_4.frag");

    glCreateVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    float vertices[] = {
     -0.5f, -0.5f, -0.5f,
     -0.5f,  0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,

      0.5f,  0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
     -0.5f, -0.5f, -0.5f
    };

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(float) * 3);
    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 0);
    glVertexArrayAttribBinding(m_vao, 0, 0);
    glEnableVertexArrayAttrib(m_vao, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenBuffers(1, &m_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STATIC_DRAW);
    //    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glGenBuffers(1, &m_sharedLayoutUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_sharedLayoutUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 512, NULL, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_sharedLayoutUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    unsigned int program1_block_index = glGetUniformBlockIndex(m_program1.getProgramID(), "Matrices");
    glUniformBlockBinding(m_program1.getProgramID(), program1_block_index, 0);

    unsigned int program2_block_index = glGetUniformBlockIndex(m_program2.getProgramID(), "Matrices");
    glUniformBlockBinding(m_program2.getProgramID(), program2_block_index, 0);

    unsigned int program3_block_index = glGetUniformBlockIndex(m_program3.getProgramID(), "Matrices");
    glUniformBlockBinding(m_program3.getProgramID(), program3_block_index, 0);

    unsigned int program4_block_index = glGetUniformBlockIndex(m_program4.getProgramID(), "Matrices");
    glUniformBlockBinding(m_program4.getProgramID(), program4_block_index, 0);


  }

  virtual void render(double currentTime) {

    GLfloat color[] = {1.0, 1.0, 1.0, 1.0};

    glClearBufferfv(GL_COLOR, 0, color);


    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::mat4(1.0f);

    glm::mat4 models[4] = {
      glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.5f, 0.0f)),
      glm::translate(glm::mat4(1.0f), glm::vec3( 0.5f, 0.5f, 0.0f)),
      glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f,-0.5f, 0.0f)),
      glm::translate(glm::mat4(1.0f), glm::vec3( 0.5f,-0.5f, 0.0f))
    };


    // STANDARD LAYOUT STYLE
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // SHARED LAYOUT STYLE

    glBindBuffer(GL_UNIFORM_BUFFER, m_sharedLayoutUBO);

    static const GLchar* uniformNames[2] = {
		          "Matrices.projection",
		          "Matrices.view"
    };

    GLuint uniformIndices[2];
    
    glGetUniformIndices(m_program1.getProgramID(), 2, uniformNames, uniformIndices);

    GLint uniformOffsets[2];
    GLint matrixStrides[2];

    glGetActiveUniformsiv(m_program1.getProgramID(), 2, uniformIndices,
	          GL_UNIFORM_OFFSET, uniformOffsets);

    glGetActiveUniformsiv(m_program1.getProgramID(), 2, uniformIndices,
	          GL_UNIFORM_MATRIX_STRIDE, matrixStrides);

    for(int c = 0; c < 4; ++c) {
      int projOffset = uniformOffsets[0] + matrixStrides[0] * c;
      int viewOffset = uniformOffsets[1] + matrixStrides[1] * c;
      for(int r = 0; r < 4; ++r) {
        glBufferSubData(GL_UNIFORM_BUFFER, projOffset, sizeof(float), (void*)&projection[c][r]);
        glBufferSubData(GL_UNIFORM_BUFFER, viewOffset, sizeof(float), (void*)&view[c][r]);

        projOffset += sizeof(float);
        viewOffset += sizeof(float);
      }
    }

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    glBindVertexArray(m_vao);

    m_program1.useProgram();
    glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(models[0]));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    m_program2.useProgram();
    glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(models[1]));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    m_program3.useProgram();
    glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(models[2]));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    m_program4.useProgram();
    glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(models[3]));
    glDrawArrays(GL_TRIANGLES, 0, 6);

  }

  virtual void shutdown() {

    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);

  }


private:
  unsigned int m_sharedLayoutUBO;
  unsigned int m_ubo;
  unsigned int m_vao;
  unsigned int m_vbo;

  Program      m_program1;
  Program      m_program2;
  Program      m_program3;
  Program      m_program4;
};

DECLARE_MAIN(UBO_App);
