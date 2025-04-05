// #include "../include/Render.hpp"
// #include <imgui.h>
// #include <tiny_obj_loader.h>
// #include <cstdlib>
// #include <ctime>
// #include <filesystem>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <iostream>
// #include <vector>
// #include "glad/glad.h"
// #include "miniaudio.h"
// namespace fs = std::filesystem;
// #include <fstream>

// void RenderEngine::loadShader()
// {
//     // Read shader source files
//     std::ifstream     vShaderFile("../Shaders/vertex_shader.glsl");
//     std::ifstream     fShaderFile("../Shaders/fragment_shader.glsl");
//     std::stringstream vShaderStream;
//     std::stringstream fShaderStream;
//     vShaderStream << vShaderFile.rdbuf();
//     fShaderStream << fShaderFile.rdbuf();
//     std::string vertexCode    = vShaderStream.str();
//     std::string fragmentCode  = fShaderStream.str();
//     const char* vShaderSource = vertexCode.c_str();
//     const char* fShaderSource = fragmentCode.c_str();

//     // Compile vertex shader
//     GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//     glShaderSource(vertexShader, 1, &vShaderSource, nullptr);
//     glCompileShader(vertexShader);

//     // Compile fragment shader
//     GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//     glShaderSource(fragmentShader, 1, &fShaderSource, nullptr);
//     glCompileShader(fragmentShader);

//     // Link shaders
//     GLuint shaderProgram = glCreateProgram();
//     glAttachShader(shaderProgram, vertexShader);
//     glAttachShader(shaderProgram, fragmentShader);
//     glLinkProgram(shaderProgram);

//     // Cleanup
//     glDeleteShader(vertexShader);
//     glDeleteShader(fragmentShader);

//     RenderEngine::shaderProgram = shaderProgram;
// }

// glm::vec3 RenderEngine::convertTo3D(int row, int col)
// {
//     float squareSize = 1.0f;                      // Adjust this based on your board scale
//     float x          = (col - 3.5f) * squareSize; // Center board at (0,0)
//     float z          = (row - 3.5f) * squareSize;
//     return glm::vec3(x, 0.0f, z);
// }

// void RenderEngine::render3DObj(std::string const& ObjectPath, int row, int col, GLuint shaderProgram)
// {
//     static GLuint VAO         = 0;
//     static GLuint VBO         = 0;
//     static bool   initialized = false;
//     static size_t nb_vertex   = 0;

//     if (!initialized)
//     {
//         tinyobj::attrib_t                attrib;
//         std::vector<tinyobj::shape_t>    shapes;
//         std::vector<tinyobj::material_t> materials;
//         std::string                      warn, err;

//         bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, ObjectPath.c_str());

//         if (!ret)
//         {
//             std::cout << "Failed to load OBJ: " << err << '\n';
//             return;
//         }

//         std::vector<float> vertices;
//         for (const auto& shape : shapes)
//         {
//             for (const auto& index : shape.mesh.indices)
//             {
//                 vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]); // x
//                 vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]); // y
//                 vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]); // z
//             }
//         }

//         nb_vertex = vertices.size() / 3;

//         glGenVertexArrays(1, &VAO);
//         glGenBuffers(1, &VBO);

//         glBindVertexArray(VAO);
//         glBindBuffer(GL_ARRAY_BUFFER, VBO);
//         glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

//         glEnableVertexAttribArray(0);
//         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

//         glBindBuffer(GL_ARRAY_BUFFER, 0);
//         glBindVertexArray(0);

//         initialized = true;
//     }

//     // **Apply transformation before rendering**
//     glm::mat4 model = glm::mat4(1.0f);
//     model           = glm::translate(model, convertTo3D(row, col)); // Move piece to correct position

//     GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
//     glUseProgram(shaderProgram);
//     glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

//     // **Render the piece**
//     glBindVertexArray(VAO);
//     glDrawArrays(GL_TRIANGLES, 0, nb_vertex);
//     glBindVertexArray(0);
// }

// void RenderEngine::render3DPieces()
// {
//     const std::string directory = "../../Assets/Objects/Pieces";

//     std::vector<std::string> setPaths;

//     if (!fs::exists(directory) || !fs::is_directory(directory))
//     {
//         std::cout << "Wrong folder" << '\n';
//         return;
//     }

//     for (const auto& entry : fs::directory_iterator(directory))
//     {
//         if (entry.path().extension() == ".obj")
//         {
//             setPaths.push_back(entry.path().string());
//         }
//     }

//     if (setPaths.empty())
//     {
//         std::cout << "No objects in the folder" << '\n';
//         return;
//     }

//     for (std::string path : setPaths)
//     {
//         // render3DObj(path);

//         // Need to change the coordinates ?
//         // // Render pieces
//         // for (auto& piece : Board::pieces)
//         // {
//         //     ImVec2 piece_pos(p.x + piece->col * tile_size + tile_size * 0.35f, p.y + piece->row * tile_size + tile_size * 0.35f);
//         //     char   symbol = piece->getSymbol();

//         // }
//     }
// }