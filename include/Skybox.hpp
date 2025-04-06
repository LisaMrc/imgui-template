#include <glad/glad.h>
#include "glm/fwd.hpp"
#include <string>
#include <vector>

class Skybox {
public:
    Skybox();
    ~Skybox();
    void init();
    void loadTextures(const std::vector<std::string>& faces);
    void draw(GLuint shaderProgram, const glm::mat4& view, const glm::mat4& projection);

private:
    GLuint VAO, VBO, cubemapTexture;
    GLuint loadCubemap(const std::vector<std::string>& faces);
    void   setupCube();
};


// GLuint generateSolidColorCubemap();
// void createSkyboxVAO(GLuint &vao, GLuint &vbo);
// void renderSkybox(GLuint vao, GLuint cubemapTexture, GLuint shaderID, const glm::mat4 &view, const glm::mat4 &projection);



