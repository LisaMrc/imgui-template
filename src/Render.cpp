#include "../include/Render.hpp"
#include <imgui.h>
#include <tiny_obj_loader.h>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <vector>
#include "glad/glad.h"
#include "miniaudio.h"

namespace fs = std::filesystem;

void render3DObj(std::string const& ObjectPath, int row, int col)
{
    static GLuint VAO         = 0;
    static GLuint VBO         = 0;
    static bool   initialized = false;
    static size_t nb_vertex   = 0;

    if (!initialized)
    {
        const std::string&               inputfile = ObjectPath;
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

    // FIXME
    // Apply transformation before rendering
    // glm::mat4 model = glm::mat4(1.0f);
    // model           = glm::translate(model, glm::vec3(row, 0.0f, col)); // Move piece to correct position

    // GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, nb_vertex);
    // glBindVertexArray(0);
}

void render3DPieces()
{
    const std::string directory = "../../Assets/Objects/Pieces";

    std::vector<std::string> setPaths;

    if (!fs::exists(directory) || !fs::is_directory(directory))
    {
        std::cout << "Wrong folder" << '\n';
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
        std::cout << "No objects in the folder" << '\n';
        return;
    }

    for (std::string path : setPaths)
    {
        // render3DObj(path);

        // Need to change the coordinates ?
        // // Render pieces
        // for (auto& piece : Board::pieces)
        // {
        //     ImVec2 piece_pos(p.x + piece->col * tile_size + tile_size * 0.35f, p.y + piece->row * tile_size + tile_size * 0.35f);
        //     char   symbol = piece->getSymbol();

        // }
    }
}