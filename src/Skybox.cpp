#include "../include/Skybox.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

GLfloat skyboxVertices[] = {
    // Positions des sommets du cube
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f
};

// Constructeur
Skybox::Skybox()
    : VAO(0), VBO(0), cubemapTexture(0) {}

// Destructeur
Skybox::~Skybox()
{
    glDeleteTextures(1, &cubemapTexture);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

// Initialisation
void Skybox::init()
{
    setupCube();
}

// Configuration des vertices du cube
void Skybox::setupCube()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Chargement de la texture cubemap
GLuint Skybox::loadCubemap(const std::vector<std::string>& faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    const int     width = 1, height = 1;
    unsigned char blueSky[3] = {135, 206, 235}; // RGB ciel bleu clair

    for (GLuint i = 0; i < 6; i++)
    {
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, blueSky
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

// Charger et assigner la texture de la skybox
void Skybox::loadTextures(const std::vector<std::string>& faces)
{
    cubemapTexture = loadCubemap(faces);
}

// Dessiner la skybox
void Skybox::draw(GLuint shaderProgram, const glm::mat4& view, const glm::mat4& projection)
{
    glUseProgram(shaderProgram);

    glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(view)); // Ne prend que la rotation de la caméra
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &viewNoTranslation[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

GLuint generateSolidColorCubemap() {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    const int size = 1;
    GLubyte solidColor[3] = { 135, 206, 235 }; // Bleu ciel clair (RGB)

    for (GLuint i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                     size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, solidColor);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

// void createSkyboxVAO(GLuint &vao, GLuint &vbo) {
//     GLfloat skyboxVertices[] = {
//         -1.0f,  1.0f, -1.0f,
//         -1.0f, -1.0f, -1.0f,
//          1.0f, -1.0f, -1.0f,
//          1.0f, -1.0f, -1.0f,
//          1.0f,  1.0f, -1.0f,
//         -1.0f,  1.0f, -1.0f,

//         -1.0f, -1.0f,  1.0f,
//         -1.0f, -1.0f, -1.0f,
//         -1.0f,  1.0f, -1.0f,
//         -1.0f,  1.0f, -1.0f,
//         -1.0f,  1.0f,  1.0f,
//         -1.0f, -1.0f,  1.0f,

//          1.0f, -1.0f, -1.0f,
//          1.0f, -1.0f,  1.0f,
//          1.0f,  1.0f,  1.0f,
//          1.0f,  1.0f,  1.0f,
//          1.0f,  1.0f, -1.0f,
//          1.0f, -1.0f, -1.0f,

//         -1.0f, -1.0f,  1.0f,
//         -1.0f,  1.0f,  1.0f,
//          1.0f,  1.0f,  1.0f,
//          1.0f,  1.0f,  1.0f,
//          1.0f, -1.0f,  1.0f,
//         -1.0f, -1.0f,  1.0f,

//         -1.0f,  1.0f, -1.0f,
//          1.0f,  1.0f, -1.0f,
//          1.0f,  1.0f,  1.0f,
//          1.0f,  1.0f,  1.0f,
//         -1.0f,  1.0f,  1.0f,
//         -1.0f,  1.0f, -1.0f,

//         -1.0f, -1.0f, -1.0f,
//         -1.0f, -1.0f,  1.0f,
//          1.0f, -1.0f, -1.0f,
//          1.0f, -1.0f, -1.0f,
//         -1.0f, -1.0f,  1.0f,
//          1.0f, -1.0f,  1.0f
//     };

//     glGenVertexArrays(1, &vao);
//     glGenBuffers(1, &vbo);

//     glBindVertexArray(vao);
//     glBindBuffer(GL_ARRAY_BUFFER, vbo);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//     glBindVertexArray(0);
// }

// void renderSkybox(GLuint vao, GLuint cubemapTexture, GLuint shaderID, const glm::mat4 &view, const glm::mat4 &projection) {
//     glDepthFunc(GL_LEQUAL); // Permet à la skybox de s’afficher correctement
//     glUseProgram(shaderID);

//     glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(view)); // Supprime la translation
//     glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, &viewNoTranslation[0][0]);
//     glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, &projection[0][0]);

//     glBindVertexArray(vao);
//     glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
//     glDrawArrays(GL_TRIANGLES, 0, 36);
//     glBindVertexArray(0);

//     glDepthFunc(GL_LESS); // Restaure le test de profondeur normal
// }
