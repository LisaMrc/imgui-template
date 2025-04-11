#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "ShaderLoader.hpp"

class Skybox {
public:
    void init(const std::vector<std::string>& faces);
    void draw(const glm::mat4& view, const glm::mat4& projection);
private:
    GLuint        loadCubemap(const std::vector<std::string>& faces);
    GLuint        textureID;
    GLuint        VAO, VBO;
    glmax::Shader skyboxShader;
};
