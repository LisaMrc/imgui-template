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
    Piece*  piece{nullptr};
    GLuint  meshVAO{0};
    GLsizei indexCount;
    int     row{0}, col{0};
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