#include "../include/Render.hpp"
#include <imgui.h>
#include <tiny_obj_loader.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "glad/glad.h"
#include "miniaudio.h"
namespace fs = std::filesystem;
#include <fstream>

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
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
                indices.push_back(indexOffset++);
            }
        }

        // Setup OpenGL buffers
        VAO vao;
        vao.init();
        vao.bind();

        VBO vbo;
        vbo.init();
        vbo.bind();
        vbo.set_data(vertices.data(), vertices.size() * sizeof(float));

        EBO ebo;
        ebo.init();
        ebo.bind();
        ebo.set_data(indices.data(), indices.size() * sizeof(GLuint));

        glEnableVertexAttribArray(0); // position only
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        vao.unbind();
        vbo.unbind();
        ebo.unbind();

        // Store only the VAO handle — vbo/ebo could be tracked too if needed
        loadedMeshes.push_back(vao.m_id); // Make sure VAO::m_id is public or add getter
    }

    std::cout << "Meshes : loaded" << '\n';
}

void RenderEngine::create3DObj()
{
    obj3D whitePawn;
    whitePawn.row = 1;
    whitePawn.col = 0;
    whitePawn.meshVAO = loadedMeshes[0]; // TEMP: pick first mesh

    gameObjects.push_back(whitePawn);

    std::cout << "3D objects : created" << '\n';
}

void RenderEngine::renderAll(const glm::mat4& projection)
{
    glUseProgram(shaderProgram);

    for (const auto& obj : gameObjects)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), convertTo3D(obj.row, obj.col));
        glm::mat4 MVP = projection * viewMatrix * model;

        GLint mvpLoc = glGetUniformLocation(shaderProgram, "MVP");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(MVP));

        glBindVertexArray(obj.meshVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // TODO: fix with index count per mesh
        glBindVertexArray(0);
    }
}

VAO::VAO()
    : m_id(0) {};

VAO::~VAO()
{
    glDeleteVertexArrays(1, &m_id);
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
    glDeleteBuffers(1, &m_id);
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
    glDeleteBuffers(1, &m_id);
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
    float squareSize = 1.0f;                      // Adjust this based on your board scale
    float x          = (col - 3.5f) * squareSize; // Center board at (0,0)
    float z          = (row - 3.5f) * squareSize;
    return glm::vec3(x, 0.0f, z);
}