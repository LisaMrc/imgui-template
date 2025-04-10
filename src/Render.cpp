#include "../include/Render.hpp"
#include <tiny_obj_loader.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include "glad/glad.h"

namespace fs = std::filesystem;

void RenderEngine::loadShader()
{
    shader.load_shader("basic.vert", "basic.frag");
}

void RenderEngine::setViewMatrix(const glm::mat4& view)
{
    viewMatrix = view;
}

glm::vec3 RenderEngine::convertTo3D(int row, int col)
{
    float squareSize = 1.0f;
    float x          = (col - 3.5f) * squareSize;
    float z          = (row - 3.5f) * squareSize;
    return glm::vec3(x, 0.0f, z);
}

void RenderEngine::render3DObj(const std::string& ObjectPath, int row, int col, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
    GLuint VAO, VBO;
    size_t nb_vertex = 0;

    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;
    std::string                      warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, ObjectPath.c_str());
    if (!ret)
    {
        std::cerr << "Failed to load OBJ: " << err << '\n';
        return;
    }

    std::vector<float> vertices;
    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);
        }
    }

    nb_vertex = vertices.size() / 3;

    glGenVertexArrays(1, &VAO);
    if (VAO == 0 || VBO == 0)
        std::cerr << "Erreur VAO/VBO non généré correctement\n";
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    shader.use();

    glm::mat4 model = glm::translate(glm::mat4(1.0f), convertTo3D(row, col));
    glm::mat4 mvp   = projectionMatrix * viewMatrix * model;

    shader.set_uniform_matrix_4fv("MVP", mvp);
    shader.set_uniform_matrix_4fv("model", model);     // optionnel si tu l'utilises dans le shader
    shader.set_uniform_matrix_4fv("view", viewMatrix); // idem

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, nb_vertex);
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glGetError();
}

void RenderEngine::render3DPieces(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
    const std::string        directory = "../../Assets/Objects/Pieces";
    std::vector<std::string> setPaths;

    if (!fs::exists(directory) || !fs::is_directory(directory))
    {
        std::cerr << "Wrong folder: " << directory << '\n';
        return;
    }

    for (const auto& entry : fs::directory_iterator(directory))
    {
        if (entry.path().extension() == ".obj")
        {
            setPaths.push_back(entry.path().string());
        }
    }

    if (setPaths.empty())
    {
        std::cerr << "No .obj files found in folder: " << directory << '\n';
        return;
    }

    int i = 0;
    for (const std::string& path : setPaths)
    {
        // Exemple de placement en ligne (modifie selon ton besoin)
        render3DObj(path, 0, i++, viewMatrix, projectionMatrix);
    }
}

// glm::mat4 RenderEngine::getViewMatrix() const
// {
//     if (App::get().isTrackballMode()) {
//         return m_TrackballCamera.getViewMatrix();
//     } else {
//         return Camera.getViewMatrix();
//     }
// }