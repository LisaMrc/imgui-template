#pragma once
#include <imgui.h>
#include <tiny_obj_loader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>
#include <vector>
#include "../../lib/quick_imgui/lib/glad/stb_image.h"
#include "../include/Piece.hpp"
#include "ShaderLoader.hpp"

class obj3D {
public:
    Piece*  piece = nullptr;
    GLuint  meshVAO{0};
    GLuint  meshVBO{0};
    GLuint  meshEBO{0};
    GLsizei indexCount;
    int     row{0}, col{0};

    void setupBuffers(const std::vector<float>& vertices, const std::vector<GLuint>& indices)
    {
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

    bool operator==(const obj3D& other) const
    {
        return (this->piece == other.piece); // Compare relevant members
    }
};

struct MeshData {
    GLuint  vao;
    GLsizei indexCount;
};

struct MaterialMeshGroup {
    GLuint    vao;
    GLuint    vbo;
    GLuint    ebo;
    GLsizei   indexCount;
    glm::vec3 diffuseColor;
};

struct Material {
    glm::vec3 diffuseColor;
};

struct Face {
    std::vector<unsigned int> vertexIndices;
    std::string materialName;
};

class RenderEngine {
public:
    GLuint                                    shaderProgram{};
    std::unordered_map<std::string, MeshData> meshMap;
    std::vector<obj3D>                        gameObjects;
    glm::mat4                                 projectionMatrix;
    glm::mat4                                 viewMatrix;
    std::vector<GLuint>                       vaoList;
    std::vector<GLuint>                       vboList;
    std::vector<GLuint>                       eboList;
    GLuint                                    chessboardTexture;
    glm::vec3                                 chessboardColor;
    std::vector<MaterialMeshGroup>            chessboardGroups;

    GLuint
         loadTexture(const std::string& path);
    void loadShader();
    void loadMeshes();
    void create3DObjects();
    void link3DObjectsToPieces(Board& board);
    void linkMeshesTo3DObjects();

    void      setViewMatrix(const glm::mat4& view);
    glm::vec3 convertTo3D(int row, int col);
    void      render3DPieces(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
    void      renderSkybox();
    void      render3DObj(const std::string& ObjectPath, int row, int col, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
    // glm::mat4 getViewMatrix() const;
    void renderAll(Board&);
    void cleanUp();
    ~RenderEngine();

private:
    glmax::Shader shader;
    // glm::mat4     viewMatrix = glm::mat4(1.0f);
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
