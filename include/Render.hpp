#pragma once
#include <imgui.h>
#include <tiny_obj_loader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderLoader.hpp"
#include "glad/glad.h"

class RenderEngine {
public:
    void      loadShader();
    glm::vec3 convertTo3D(int row, int col);
    void      render3DPieces();
    void      renderSkybox();
    void      render3DObj(const std::string& ObjectPath, int row, int col);
    void      setViewMatrix(const glm::mat4& view);

private:
    glmax::Shader shader;
    glm::mat4     viewMatrix = glm::mat4(1.0f);
};
