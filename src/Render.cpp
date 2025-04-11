#include "../include/Render.hpp"
#include <tiny_obj_loader.h>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include "../include/Board.hpp"
#include "glad/glad.h"

namespace fs = std::filesystem;

GLuint RenderEngine::loadTexture(const std::string& path)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture: " << path << '\n';
    }

    stbi_image_free(data);
    return textureID;
}

void RenderEngine::loadShader()
{
    std::ifstream vShaderFile("../../Shaders/vertex_shader.glsl");
    std::ifstream fShaderFile("../../Shaders/fragment_shader.glsl");

    if (!vShaderFile || !fShaderFile)
    {
        std::cerr << "Failed to open shader files.\n";
        return;
    }

    std::stringstream vShaderStream;
    std::stringstream fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    std::string vertexCode    = vShaderStream.str();
    std::string fragmentCode  = fShaderStream.str();
    const char* vShaderSource = vertexCode.c_str();
    const char* fShaderSource = fragmentCode.c_str();

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

    lightColorLocation = glGetUniformLocation(shaderProgram, "lightColor");

    std::cout << "Shaders: loaded" << '\n';
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
        std::string                      warn, err;

        std::string mtlBaseDir = entry.path().parent_path().string() + "/";

        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, entry.path().string().c_str(), mtlBaseDir.c_str(), true);

        if (!ret)
        {
            std::cerr << "Failed to load OBJ: " << entry.path() << "\n"
                      << err << '\n';
            continue;
        }

        std::string meshName = entry.path().stem().string();

        if (meshName == "Bishop" || meshName == "Queen" || meshName == "King") {
            std::cout << "Fixing normals for: " << meshName << '\n';
        
            // Loop over all normals and flip them if needed
            for (size_t i = 0; i < attrib.normals.size(); i += 3) {
                float& nx = attrib.normals[i + 0];
                float& ny = attrib.normals[i + 1];
                float& nz = attrib.normals[i + 2];
        
                // Flip the direction if needed (common fix for inward normals)
                nx = -nx;
                ny = -ny;
                nz = -nz;
            }
        }

        if (meshMap.contains(meshName))
        {
            std::cerr << "Duplicate mesh name detected: " << meshName << '\n';
            continue;
        }

        if (meshName == "Chessboard")
        {
            chessboardGroups.clear();
            for (const auto& shape : shapes)
            {
                std::unordered_map<int, std::vector<float>>  materialVertices;
                std::unordered_map<int, std::vector<GLuint>> materialIndices;
                std::unordered_map<int, glm::vec3>           materialColors;

                GLuint globalIndex  = 0;
                size_t index_offset = 0;

                for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
                {
                    int fv          = shape.mesh.num_face_vertices[f];
                    int material_id = shape.mesh.material_ids[f];

                    if (material_id < 0 || material_id >= materials.size()) {
                        std::cerr << "Invalid material ID at face " << f << "\n";
                        continue;
                    }                    

                    if (materialColors.find(material_id) == materialColors.end())
                    {
                        auto& mat                   = materials[material_id];
                        std::cout << "Assigning material: " << mat.name << "\n";
                        if (mat.name == "Black")
                            materialColors[material_id] = glm::vec3(0.2f, 0.2f, 0.2f); // dark gray
                        else
                            materialColors[material_id] = glm::vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);

                    }

                    std::vector<float>& vertBuffer = materialVertices[material_id];
                    std::vector<GLuint>& idxBuffer = materialIndices[material_id];

                    GLuint baseVertexIndex = vertBuffer.size() / 8; // 8 floats per vertex

                    for (size_t v = 0; v < fv; ++v)
                    {
                        tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                        float vx = attrib.vertices[3 * idx.vertex_index + 0];
                        float vy = attrib.vertices[3 * idx.vertex_index + 1];
                        float vz = attrib.vertices[3 * idx.vertex_index + 2];

                        float nx = (idx.normal_index >= 0) ? attrib.normals[3 * idx.normal_index + 0] : 0.0f;
                        float ny = (idx.normal_index >= 0) ? attrib.normals[3 * idx.normal_index + 1] : 1.0f;
                        float nz = (idx.normal_index >= 0) ? attrib.normals[3 * idx.normal_index + 2] : 0.0f;

                        float tx = (idx.texcoord_index >= 0) ? attrib.texcoords[2 * idx.texcoord_index + 0] : 0.0f;
                        float ty = (idx.texcoord_index >= 0) ? attrib.texcoords[2 * idx.texcoord_index + 1] : 0.0f;

                        vertBuffer.insert(vertBuffer.end(), {vx, vy, vz, nx, ny, nz, tx, ty});
                        idxBuffer.push_back(baseVertexIndex + v);
                    }

                    index_offset += fv;
                }

                for (auto& [matID, verts] : materialVertices)
                {
                    const auto& inds  = materialIndices[matID];
                    const auto& color = materialColors[matID];

                    GLuint vao, vbo, ebo;
                    glGenVertexArrays(1, &vao);
                    glGenBuffers(1, &vbo);
                    glGenBuffers(1, &ebo);

                    glBindVertexArray(vao);

                    glBindBuffer(GL_ARRAY_BUFFER, vbo);
                    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds.size() * sizeof(GLuint), inds.data(), GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
                    glEnableVertexAttribArray(0);

                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
                    glEnableVertexAttribArray(1);

                    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
                    glEnableVertexAttribArray(2);

                    std::cout << "Face uses material: " << materials[matID].name << "\n";

                    MaterialMeshGroup group;
                    group.vao        = vao;
                    group.vbo        = vbo;
                    group.ebo        = ebo;
                    group.indexCount = static_cast<GLsizei>(inds.size());
                    std::cout << materials[matID].name << "\n";
                    group.diffuseColor = color;
                    chessboardGroups.push_back(group);
                }

                for (auto& [matID, verts] : materialVertices)
                    std::cout << "Material: " << materials[matID].name << " has " << (verts.size() / 8) << " vertices\n";

            }
        
            std::cout << "Chessboard material groups: " << chessboardGroups.size() << '\n';
        }

        // Otherwise, standard single-material mesh
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

                if (idx.texcoord_index >= 0)
                {
                    vertices.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
                    vertices.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
                }
                else
                {
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                }

                indices.push_back(indexOffset++);
            }
        }

        if (!indices.empty())
        {
            GLuint vao = 0, vbo = 0, ebo = 0;
            glGenVertexArrays(1, &vao);
            vaoList.push_back(vao);
            glGenBuffers(1, &vbo);
            vboList.push_back(vbo);
            glGenBuffers(1, &ebo);
            eboList.push_back(ebo);

            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);

            MeshData mesh{};
            mesh.vao          = vao;
            mesh.indexCount   = static_cast<GLsizei>(indices.size());
            meshMap[meshName] = mesh;
        }

        std::cout << meshName << "\n";
    }

    std::cout << "Meshes: " << meshMap.size() << " loaded\n";
}

void RenderEngine::setViewMatrix(const glm::mat4& view)
{
    viewMatrix = view;
}

void RenderEngine::create3DObjects()
{
    gameObjects.clear();

    obj3D boardObj;
    boardObj.row   = 0;
    boardObj.col   = 0;
    boardObj.piece = nullptr;
    gameObjects.push_back(boardObj);

    for (int i = 0; i < 32; ++i)
    {
        obj3D obj;
        obj.row   = 0;
        obj.col   = 0;
        obj.piece = nullptr;
        gameObjects.push_back(obj);
    }

    std::cout << "Objects : " << gameObjects.size() << " created\n";
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

void RenderEngine::linkMeshesTo3DObjects()
{
    if (gameObjects.size() < 33)
    {
        std::cerr << "Error: not enough objects to assign meshes\n";
        return;
    }

    // Assign the board mesh to the first object (index 0)
    if (meshMap.find("Chessboard") != meshMap.end())
    {
        gameObjects[0].meshVAO    = meshMap["Chessboard"].vao;
        gameObjects[0].indexCount = meshMap["Chessboard"].indexCount;
    }
    else
    {
        std::cerr << "Mesh not found for: Chessboard\n";
    }

    // Assign piece meshes to the rest of the objects
    for (size_t i = 1; i < gameObjects.size(); ++i)
    {
        obj3D& obj = gameObjects[i];

        if (!obj.piece)
        {
            std::cerr << "Warning: obj3D at index " << i << " has no linked piece.\n";
            continue;
        }

        char        symbol = obj.piece->getType();
        std::string key;

        switch (symbol)
        {
        case 'P': key = "Pawn"; break;
        case 'R': key = "Rook"; break;
        case 'N': key = "Knight"; break;
        case 'B': key = "Bishop"; break;
        case 'Q': key = "Queen"; break;
        case 'K': key = "King"; break;
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

    std::cout << "Meshes : assigned .\n";
}

void RenderEngine::renderAll(Board& board)
{
    glUseProgram(shaderProgram);

    GLint lightDirLoc = glGetUniformLocation(shaderProgram, "lightDir");
    glUniform3f(lightDirLoc, 0.0f, -0.5f, 0.0f);

    glUniform3f(glGetUniformLocation(shaderProgram, "lightDir"), 0.0f, -0.5f, 0.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 1.0f, 0.7f, 0.5f);    

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

        // Render the board (index 0 in gameObjects) with multiple materials
        if (&obj == &gameObjects[0])
        {
            for (const auto& group : chessboardGroups)
            {
                glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(group.diffuseColor));
                
                glBindVertexArray(group.vao);
                glDrawElements(GL_TRIANGLES, group.indexCount, GL_UNSIGNED_INT, 0);
            }
            continue; // skip to next object
        }

        // Render chess pieces
        glm::vec3 color = obj.piece && !obj.piece->isWhite ? glm::vec3(0.2f) : glm::vec3(1.0f);

        GLint colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
        glUniform3fv(colorLoc, 1, glm::value_ptr(color));

        if (obj.indexCount == 0)
        {
            std::cerr << "Erreur : indexCount = 0\n";
            continue;
        }

        glBindVertexArray(obj.meshVAO);
        if (obj.piece->getType() == 'N' && obj.piece->isWhite)
        {
            float angle = glm::radians(180.0f);

            glm::mat4 model = glm::mat4(1.0f);

            glm::vec3 objectPosition = glm::vec3(5.0f, 0.0f, 0.0f);

            model = glm::translate(model, convertTo3D(obj.piece->row, obj.piece->col));

            model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        }
        glDrawElements(GL_TRIANGLES, obj.indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

TimeOfDay RenderEngine::getSceneTimeOfDay()
{
    std::time_t now       = std::time(nullptr);
    std::tm*    localTime = std::localtime(&now);

    int hour = localTime->tm_hour;

    std::vector<double> probabilities;

    if (hour >= 6 && hour < 18)
    {
        probabilities = {0.8, 0.1, 0.1}; // Day
    }
    else if ((hour >= 5 && hour < 6) || (hour >= 18 && hour < 20))
    {
        probabilities = {0.2, 0.2, 0.6}; // Twilight
    }
    else
    {
        probabilities = {0.1, 0.8, 0.1}; // Night
    }

    Multinomial timeOfDay(probabilities);
    int         result = timeOfDay.roll();

    switch (result)
    {
    case 0: return TimeOfDay::Day;
    case 1: return TimeOfDay::Night;
    case 2: return TimeOfDay::Twilight;
    default: return TimeOfDay::Day; // fallback
    }
}

void RenderEngine::setSceneLighting(TimeOfDay timeOfDay)
{
    glm::vec3 lightColor;

    switch (timeOfDay)
    {
        case TimeOfDay::Day:
            lightColor = glm::vec3(1.0f, 1.0f, 0.9f); // bright warm white
            break;
        case TimeOfDay::Night:
            lightColor = glm::vec3(0.1f, 0.1f, 0.3f); // dark blueish
            break;
        case TimeOfDay::Twilight:
            lightColor = glm::vec3(0.9f, 0.5f, 0.3f); // orange-pink glow
            break;
    }

    glUseProgram(shaderProgram);  // Ensure the shader program is active
    glUniform3fv(lightColorLocation, 1, glm::value_ptr(lightColor));
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
    float squareSize = 1.5f;

    float totalSize = squareSize * 8.0f;
    float xOffset   = totalSize / 2.0f - squareSize / 2.0f;
    float zOffset   = totalSize / 2.0f - squareSize / 2.0f;

    float x = (col * squareSize) - xOffset;
    float z = (row * squareSize) - zOffset;
    float y = -0.2f;

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
    shader.set_uniform_matrix_4fv("model", model);     
    shader.set_uniform_matrix_4fv("view", viewMatrix); 

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
        render3DObj(path, 0, i++, viewMatrix, projectionMatrix);
    }
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
        shaderProgram = 0;
    }

    std::cout << "Cleanup: All resources deleted.\n";
}

RenderEngine::~RenderEngine()
{
    cleanUp();
}
