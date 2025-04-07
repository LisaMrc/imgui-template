// #include "../include/Skybox.hpp"
// // #include "shader_utils.hpp" // à adapter selon ton projet
// #include <glm/gtc/type_ptr.hpp>
// #include "../include/ShaderLoader.hpp"

// // shaderProgram = load_shader(
// //     "shaders/skybox_test.vert",
// //     "shaders/skybox_test.frag"
// // );

// float skyboxVertices[] = {
//     // positions du cube (36 sommets)
//     -1.0f,  1.0f, -1.0f, // ...
//     // complète avec les 36 sommets du cube standard
//     // chaque face 2 triangles, chaque triangle 3 sommets
//     // -> total = 36 sommets
// };

// Skybox::Skybox() {
//     initBuffers();
//     loadShaders();
// }

// Skybox::~Skybox() {
//     glDeleteVertexArrays(1, &VAO);
//     glDeleteBuffers(1, &VBO);
//     glDeleteProgram(shaderProgram);
// }

// void Skybox::initBuffers() {
//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);

//     glBindVertexArray(VAO);
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
// }

// // void Skybox::loadShaders() {
// //     shaderProgram = loadShader("shaders/skybox_test.vert", "shaders/skybox_test.frag");
// // }

// void Skybox::render(const glm::mat4& view, const glm::mat4& projection) {
//     glDepthFunc(GL_LEQUAL);
//     glUseProgram(shaderProgram);

//     glm::mat4 viewNoTranslate = glm::mat4(glm::mat3(view));
//     glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewNoTranslate));
//     glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

//     glBindVertexArray(VAO);
//     glDrawArrays(GL_TRIANGLES, 0, 36);
//     glDepthFunc(GL_LESS);
// }
