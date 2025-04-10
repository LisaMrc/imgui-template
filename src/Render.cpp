#include "../include/Render.hpp"
#include <tiny_obj_loader.h>
#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include "../include/Board.hpp"
#include "glad/glad.h"

namespace fs = std::filesystem;

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

    for (const auto& entry : std::filesystem::directory_iterator(objFolder))
    {
        if (entry.path().extension() != ".obj")
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
            std::cerr << "Failed to load OBJ: " << entry.path() << "\n"
                      << err << '\n';
            continue;
        }

        std::vector<float>  vertices;
        std::vector<GLuint> indices;
        GLuint              indexOffset = 0;

        for (const auto& shape : shapes)
        {
            for (const auto& idx : shape.mesh.indices)
            {
                // Position
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

                // Normal
                if (idx.normal_index >= 0)
                {
                    vertices.push_back(attrib.normals[3 * idx.normal_index + 0]);
                    vertices.push_back(attrib.normals[3 * idx.normal_index + 1]);
                    vertices.push_back(attrib.normals[3 * idx.normal_index + 2]);
                }
                else
                {
                    // Push a default normal (e.g., up)
                    vertices.push_back(0.0f);
                    vertices.push_back(1.0f);
                    vertices.push_back(0.0f);
                }

                indices.push_back(indexOffset++);
            }
        }

        // Création de VAO, VBO, et EBO pour chaque mesh
        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        vaoList.push_back(vao); // Ajouter le VAO à la liste

        GLuint vbo = 0;
        glGenBuffers(1, &vbo);
        vboList.push_back(vbo); // Ajouter le VBO à la liste

        GLuint ebo = 0;
        glGenBuffers(1, &ebo);
        eboList.push_back(ebo); // Ajouter l'EBO à la liste

        // Liaison et remplissage des buffers
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0); // Unbind VAO after configuration

        // Stocker les informations du mesh
        MeshData mesh{};
        mesh.vao        = vao;
        mesh.indexCount = static_cast<GLsizei>(indices.size());

        if (indices.empty())
        {
            std::cerr << "Mesh sans indices : " << entry.path() << '\n';
            continue;
        }

        loadedMeshes.push_back(mesh); // Ajouter le mesh chargé à la liste
    }

    std::cout << "Meshes : loaded" << '\n';
}
void RenderEngine::setViewMatrix(const glm::mat4& view)
{
    viewMatrix = view;
}

void RenderEngine::create3DObj(Board& board)
{
    if (loadedMeshes.empty())
    {
        std::cerr << "Error: No meshes loaded.\n";
        return;
    }

    obj3D whitePawn;
    whitePawn.row = 6;
    whitePawn.col = 0;

    whitePawn.meshVAO    = loadedMeshes[0].vao;
    whitePawn.indexCount = loadedMeshes[0].indexCount;

    // Assuming it's the first pawn in the list
    whitePawn.piece = board.pieces[0].get();

    gameObjects.push_back(whitePawn);

    std::cout << "Object created for piece at ("
              << whitePawn.piece->row << ", "
              << whitePawn.piece->col << ")\n";
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
