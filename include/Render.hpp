#pragma once
#include <imgui.h>
#include <tiny_obj_loader.h>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "glad/glad.h"
#include "miniaudio.h"
namespace fs = std::filesystem;

glm::vec3 convertTo3D(int row, int col);
void render3DObj(std::string const &ObjectPath, int row, int col, GLuint shaderProgram);
void render3DPieces();

void renderSkybox();