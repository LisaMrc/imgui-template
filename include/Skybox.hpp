#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>


class Skybox {
public:
    void init();
    // void draw(const glm::mat4& view, const glm::mat4& projection);
private:
    GLuint loadCubemapFake();
    GLuint textureID;
    GLuint VAO, VBO;
};
