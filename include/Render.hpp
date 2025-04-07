#pragma once
#include <imgui.h>
#include <tiny_obj_loader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "../include/Piece.hpp"
#include "glad/glad.h"

class obj3D {
public:
    Piece* piece{nullptr};
    GLuint meshVAO{0};
    GLuint meshVBO{0};
    GLuint meshEBO{0};
    GLsizei indexCount;
    int row{0}, col{0};

    void setupBuffers(const std::vector<float>& vertices, const std::vector<GLuint>& indices) {
        // Création et liaison du VAO
        glGenVertexArrays(1, &meshVAO);
        glBindVertexArray(meshVAO);

        // Création et liaison du VBO
        glGenBuffers(1, &meshVBO);
        glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // Création et liaison du EBO
        glGenBuffers(1, &meshEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        // Définir le format des attributs du VBO
        glEnableVertexAttribArray(0); // position only
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        // Délié VAO après configuration
        glBindVertexArray(0);
    }
};

struct MeshData {
    GLuint  vao;
    GLsizei indexCount;
};

class RenderEngine {
public:
    GLuint                shaderProgram{};
    std::vector<MeshData> loadedMeshes;
    std::vector<obj3D>    gameObjects;
    glm::mat4             projectionMatrix;
    glm::mat4             viewMatrix;
    std::vector<GLuint>   vaoList;
    std::vector<GLuint>   vboList;
    std::vector<GLuint>   eboList;

    void loadShader();
    void loadMeshes();
    void create3DObj();
    // TODO(🚀) : group everything into an init function

    void      setViewMatrix(const glm::mat4& view);
    glm::vec3 convertTo3D(int row, int col);

    void renderAll();
    void cleanUp();
    ~RenderEngine();
};

class VAO {
public:
    explicit VAO();
    ~VAO();
    void   init();
    void   bind() const;
    void   unbind() const;
    GLuint m_id{};
};

class VBO {
public:
    explicit VBO();
    ~VBO();
    void   init();
    void   bind() const;
    void   unbind() const;
    void   set_data(const void* data, GLsizeiptr size);
    GLuint m_id{};
};

class EBO {
public:
    explicit EBO();
    ~EBO();
    void   init();
    void   bind() const;
    void   unbind() const;
    void   set_data(const void* data, GLsizeiptr size);
    GLuint m_id{};
};