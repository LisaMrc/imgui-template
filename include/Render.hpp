#pragma once
#include <imgui.h>
#include <tiny_obj_loader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "../include/Piece.hpp"
#include "glad/glad.h"

class RenderEngine {
public:
    glm::mat4 viewMatrix; // Camera view matrix
public:
    std::vector<GLuint> loadedMeshes; // Stocker les IDs des VAOs des objets
    GLuint              shaderProgram{};

    void      loadMeshes();
    void      loadShader();
    glm::vec3 convertTo3D(int row, int col);
    void      render3DObj(const std::string& ObjectPath, int row, int col);
    void      render3DPieces();
    void      render3DBoard();
    void      renderSkybox();
    void      renderUpdate();
    void      setViewMatrix(const glm::mat4& view);
};

class obj3D {
public:
    Piece* piece{nullptr};
    GLuint VAO{0}, VBO{0}, EBO{0}; // Stocker directement les buffers OpenGL

    void setupBuffers();
};

class VAO {
public:
    explicit VAO();
    ~VAO();
    void init();
    void bind() const;
    void unbind() const;

private:
    GLuint m_id{};
};

class VBO {
public:
    explicit VBO();
    ~VBO();
    void init();
    void bind() const;
    void unbind() const;
    void set_data(const void* data, GLsizeiptr size);

private:
    GLuint m_id{};
};

class EBO {
public:
    explicit EBO();
    ~EBO();
    void init();
    void bind() const;
    void unbind() const;
    void set_data(const void* data, GLsizeiptr size);

private:
    GLuint m_id{};
};