#pragma once
#include <imgui.h>
#include <tiny_obj_loader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"

class RenderEngine {
public:
    GLuint    loadShader(const char* vertexPath, const char* fragmentPath);
    glm::vec3 convertTo3D(int row, int col);
    void      render3DObj(std::string const& ObjectPath, int row, int col, GLuint shaderProgram);
    void      render3DPieces();
    void      renderSkybox();
    GLuint    shaderProgram{};
};