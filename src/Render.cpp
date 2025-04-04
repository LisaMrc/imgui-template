#include "../include/Render.hpp"
#include <imgui.h>
#include <tiny_obj_loader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "../include/App.hpp"
#include "../include/ShaderLoader.hpp"
#include "glad/glad.h"


void App::display3DObj()
{
    static GLuint VAO         = 0;
    static GLuint VBO         = 0;
    static bool   initialized = false;
    static size_t nb_vertex   = 0;

    if (!initialized)
    {
        std::string                      inputfile = "../../Assets/Objects/Pawn.obj";
        tinyobj::attrib_t                attrib;
        std::vector<tinyobj::shape_t>    shapes;
        std::vector<tinyobj::material_t> materials;
        std::string                      warn;
        std::string                      err;

        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());

        if (!ret)
        {
            std::cout << "Failed to load OBJ: " << err << '\n';
            return;
        }

        std::vector<float> vertices;
        for (const auto& shape : shapes)
        {
            for (const auto& index : shape.mesh.indices)
            {
                vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]); // x
                vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]); // y
                vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]); // z
            }
        }

        nb_vertex = vertices.size();

        // OpenGL : Création des buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        initialized = true;
    }

    camera.init_mat_proj();

    glmax::Shader* shader = manager.getShaderLoader()->getProgram();
    shader->use();
    shader->set_uniform_matrix_4fv("uMVPMatrix", camera.get_MVP());
    shader->set_uniform_matrix_4fv("uMVMatrix", camera.get_MVMatrix());
    shader->set_uniform_matrix_4fv("uNormalMatrix", camera.get_NormalMatrix());

    // Affichage OpenGL
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, nb_vertex); // Modifier selon ton modèle
    glBindVertexArray(0);
}

// void Renderer::applyViewMatrix(const glm::mat4& viewMatrix, GLuint shaderProgram)
// {
//     glUseProgram(shaderProgram);
//     GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
//     if (viewLoc == -1)
//     {
//         std::cerr << "Uniform 'view' not found in shader!" << std::endl;
//         return;
//     }
//     glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix[0][0]);
// }