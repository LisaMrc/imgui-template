#include "../include/Render.hpp"
#include <imgui.h>
#include <tiny_obj_loader.h>
#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "../include/Board.hpp"
#include "glad/glad.h"
#include "miniaudio.h"

void RenderEngine::loadShader()
{
    // Open shader files
    std::ifstream vShaderFile("../../Shaders/vertex_shader.glsl");
    std::ifstream fShaderFile("../../Shaders/fragment_shader.glsl");

    if (!vShaderFile || !fShaderFile)
    {
        std::cerr << "Failed to open shader files.\n";
        return;
    }

    // Read file content
    std::stringstream vShaderStream;
    std::stringstream fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    std::string vertexCode    = vShaderStream.str();
    std::string fragmentCode  = fShaderStream.str();
    const char* vShaderSource = vertexCode.c_str();
    const char* fShaderSource = fragmentCode.c_str();

    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLint success = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed:\n"
                  << infoLog << '\n';
        return;
    }

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed:\n"
                  << infoLog << '\n';
        return;
    }

    // Link shaders into program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed:\n"
                  << infoLog << '\n';
        return;
    }

    // Cleanup
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    shaderProgram = program;

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader linking failed:\n"
                  << infoLog << std::endl;
    }

    std::cout << "Shaders : loaded" << '\n';
}

void RenderEngine::loadMeshes()
{
    const std::string objFolder = "../../Assets/Objects";

    if (!std::filesystem::exists(objFolder))
    {
        std::cerr << "Directory not found: " << objFolder << '\n';
        return;
    }

    for (const auto& entry : std::filesystem::recursive_directory_iterator(objFolder))
    {
        if (!entry.is_regular_file() || entry.path().extension() != ".obj")
            continue;

        tinyobj::attrib_t                attrib;
        std::vector<tinyobj::shape_t>    shapes;
        std::vector<tinyobj::material_t> materials;
        std::string                      warn;
        std::string                      err;

        bool ret = tinyobj::LoadObj(
            &attrib, &shapes, &materials, &warn, &err,
            entry.path().string().c_str(), nullptr, true
        );

        if (!ret)
        {
            std::cerr << "Failed to load OBJ: " << entry.path() << "\n" << err << '\n';
            continue;
        }

        std::vector<float>  vertices;
        std::vector<GLuint> indices;
        GLuint              indexOffset = 0;

        for (const auto& shape : shapes)
        {
            for (const auto& idx : shape.mesh.indices)
            {
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

                if (idx.normal_index >= 0)
                {
                    vertices.push_back(attrib.normals[3 * idx.normal_index + 0]);
                    vertices.push_back(attrib.normals[3 * idx.normal_index + 1]);
                    vertices.push_back(attrib.normals[3 * idx.normal_index + 2]);
                }
                else
                {
                    vertices.push_back(0.0f);
                    vertices.push_back(1.0f);
                    vertices.push_back(0.0f);
                }

                indices.push_back(indexOffset++);
            }
        }

        if (indices.empty())
        {
            std::cerr << "Mesh without indices: " << entry.path() << '\n';
            continue;
        }

        GLuint vao = 0, vbo = 0, ebo = 0;
        glGenVertexArrays(1, &vao); vaoList.push_back(vao);
        glGenBuffers(1, &vbo); vboList.push_back(vbo);
        glGenBuffers(1, &ebo); eboList.push_back(ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        MeshData mesh{};
        mesh.vao        = vao;
        mesh.indexCount = static_cast<GLsizei>(indices.size());

        std::string meshName = entry.path().stem().string(); // e.g. "Pawn" from "Pawn.obj"

        if (meshMap.contains(meshName))
        {
            std::cerr << "Duplicate mesh name detected: " << meshName << '\n';
            continue;
        }

        meshMap[meshName] = mesh;
    }

    std::cout << "Meshes: loaded (" << meshMap.size() << " in total)\n";
}

void RenderEngine::create3DObjects()
{
    gameObjects.clear();

    obj3D boardObj;
    boardObj.row = 0;
    boardObj.col = 0;
    boardObj.piece = nullptr;
    gameObjects.push_back(boardObj);

    for (int i = 0; i < 32; ++i)
    {
        obj3D obj;
        obj.row = 0;
        obj.col = 0;
        obj.piece = nullptr;
        gameObjects.push_back(obj);
    }

    std::cout << "Created " << gameObjects.size() << " objects (1 board + 32 pieces)\n";
}

void RenderEngine::link3DObjectsToPieces(Board& board)
{
    if (gameObjects.size() < 33)
    {
        std::cerr << "Error: expected at least 33 gameObjects (1 board + 32 pieces)\n";
        return;
    }

    if (board.pieces.size() != 32)
    {
        std::cerr << "Error: expected 32 pieces in board\n";
        return;
    }

    for (size_t i = 0; i < 32; ++i)
    {
        gameObjects[i + 1].piece = board.pieces[i].get(); // +1 to skip board object
    }

    std::cout << "Linked 32 3D objects to their respective pieces.\n";
}

void RenderEngine::linkMeshesToPieces()
{
    if (gameObjects.size() < 33)
    {
        std::cerr << "Error: not enough objects to assign meshes\n";
        return;
    }

    for (size_t i = 1; i < gameObjects.size(); ++i) // skip index 0 (the board)
    {
        obj3D& obj = gameObjects[i];

        if (!obj.piece)
        {
            std::cerr << "Warning: obj3D at index " << i << " has no linked piece.\n";
            continue;
        }

        char symbol = obj.piece->getSymbol(); // Example: 'P', 'K', 'N', etc.
        std::string key;

        switch (symbol)
        {
            case 'P': key = "Pawn";   break;
            case 'R': key = "Rook";   break;
            case 'N': key = "Knight"; break;
            case 'B': key = "Bishop"; break;
            case 'Q': key = "Queen";  break;
            case 'K': key = "King";   break;
            default:
                std::cerr << "Unknown symbol: " << symbol << "\n";
                continue;
        }

        if (meshMap.find(key) != meshMap.end())
        {
            obj.meshVAO    = meshMap[key].vao;
            obj.indexCount = meshMap[key].indexCount;
        }
        else
        {
            std::cerr << "Mesh not found for: " << key << "\n";
        }
    }

    std::cout << "Assigned meshes to all 3D pieces.\n";
}

void RenderEngine::renderAll()
{
    glUseProgram(shaderProgram);

    // Example light direction: pointing down and toward the camera
    GLint lightDirLoc = glGetUniformLocation(shaderProgram, "lightDir");
    glUniform3f(lightDirLoc, 0.0f, 1.0f, 1.0f); // Feel free to tweak this

    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    for (const auto& obj : gameObjects)
    {
        int       row   = obj.piece ? obj.piece->row : obj.row;
        int       col   = obj.piece ? obj.piece->col : obj.col;
        glm::mat4 model = glm::translate(glm::mat4(1.0f), convertTo3D(row, col));

        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        if (obj.indexCount == 0)
        {
            std::cerr << "Erreur : indexCount = 0\n";
            continue;
        }

        glBindVertexArray(obj.meshVAO); // Chaque objet a son propre VAO

        glDrawElements(GL_TRIANGLES, obj.indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); // Unbind VAO
    }
}

VAO::VAO()
    : m_id(0) {};

VAO::~VAO()
{
    // glDeleteVertexArrays(1, &m_id);
}

void VAO::init()
{
    glGenVertexArrays(1, &m_id);
}

void VAO::bind() const
{
    glBindVertexArray(m_id);
}

void VAO::unbind() const
{
    glBindVertexArray(0);
}

VBO::VBO()
{
    // Optional: Initialize anything here, or leave empty
}

VBO::~VBO()
{
    // glDeleteBuffers(1, &m_id);
}

void VBO::init()
{
    glGenBuffers(1, &m_id);
}

void VBO::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VBO::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::set_data(const void* data, GLsizeiptr size)
{
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

EBO::EBO()
    : m_id(0) {}

EBO::~EBO()
{
    // glDeleteBuffers(1, &m_id);
}

void EBO::init()
{
    glGenBuffers(1, &m_id);
}

void EBO::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void EBO::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::set_data(const void* data, GLsizeiptr size)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

glm::vec3 RenderEngine::convertTo3D(int row, int col)
{
    float squareSize = 1.0f;                      // Size of one board tile
    float x          = (col - 3.5f) * squareSize; // Center board at (0,0)
    float z          = (row - 3.5f) * squareSize;
    float y          = -5.f; // Push down model a bit (adjust if needed)
    return glm::vec3(x, y, z);
}

void RenderEngine::cleanUp()
{
    // Supprimer tous les VAO
    glDeleteVertexArrays(vaoList.size(), vaoList.data());
    vaoList.clear();

    // Supprimer tous les VBO
    glDeleteBuffers(vboList.size(), vboList.data());
    vboList.clear();

    // Supprimer tous les EBO
    glDeleteBuffers(eboList.size(), eboList.data());
    eboList.clear();

    // Supprimer le programme de shaders
    if (shaderProgram)
    {
        glDeleteProgram(shaderProgram);
        shaderProgram = 0; // Nullify the shader program to prevent use after deletion
    }

    std::cout << "Cleanup: All resources deleted.\n";
}

RenderEngine::~RenderEngine()
{
    cleanUp();
}