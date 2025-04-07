#pragma once
#include <imgui.h>
#include <tiny_obj_loader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"
#include "ShaderLoader.hpp"


class RenderEngine {
public:
    void loadShader();
    glm::vec3 convertTo3D(int row, int col);
    void render3DPieces();
    void renderSkybox();
    void render3DObj(const std::string& ObjectPath, int row, int col);

private:
    glmax::Shader shader; // ✅ remplacer le GLuint shaderProgram
};
