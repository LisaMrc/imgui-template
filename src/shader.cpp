#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include "../include/ShaderLoader.hpp"

namespace glmax {

// -------- CONSTRUCTEUR / DESTRUCTEUR --------

Shader::~Shader()
{
    glDeleteProgram(m_programID);
}

// -------- MÉTHODES PUBLIQUES --------

void Shader::use() const
{
    glUseProgram(m_programID);
}

GLuint Shader::getID() const
{
    return m_programID;
}

void Shader::load_shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    GLuint vertexShader   = compile_shader(vertexPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = compile_shader(fragmentPath, GL_FRAGMENT_SHADER);

    m_programID = glCreateProgram();
    glAttachShader(m_programID, vertexShader);
    glAttachShader(m_programID, fragmentShader);
    glLinkProgram(m_programID);

    GLint success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(m_programID, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::set_uniform_matrix_4fv(const std::string& name, const glm::mat4& value)
{
    auto location = get_uniform_location(name);
    if (location)
    {
        glUniformMatrix4fv(*location, 1, GL_FALSE, glm::value_ptr(value));
    }
}

void Shader::set_uniform_3fv(const std::string& name, const glm::vec3& value)
{
    auto location = get_uniform_location(name);
    if (location)
    {
        glUniform3fv(*location, 1, glm::value_ptr(value));
    }
}

void Shader::set_uniform_1f(const std::string& name, float value)
{
    auto location = get_uniform_location(name);
    if (location)
    {
        glUniform1f(*location, value);
    }
}

void Shader::set_uniform_1i(const std::string& name, int value)
{
    auto location = get_uniform_location(name);
    if (location)
    {
        glUniform1i(*location, value);
    }
}

// -------- MÉTHODES PRIVÉES --------

GLuint Shader::compile_shader(const std::string& path, GLenum type)
{
    std::string shaderCode   = read_file(path);
    const char* shaderSource = shaderCode.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n"
                  << path << "\n"
                  << infoLog << std::endl;
    }

    return shader;
}

std::string Shader::read_file(const std::string& path)
{
    std::ifstream     file(path);
    std::stringstream buffer;

    if (!file.is_open())
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_FOUND\n"
                  << path << std::endl;
        return "";
    }

    buffer << file.rdbuf();
    return buffer.str();
}

std::optional<GLint> Shader::get_uniform_location(const std::string& name)
{
    if (m_uniform_cache.find(name) != m_uniform_cache.end())
    {
        return m_uniform_cache[name];
    }

    GLint location = glGetUniformLocation(m_programID, name.c_str());

    if (location == -1)
    {
        std::cerr << "WARNING::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
        return std::nullopt;
    }

    m_uniform_cache[name] = location;
    return location;
}

} // namespace glmax
