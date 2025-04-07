#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

layout(location = 1) in vec3 aNormal;
out vec3 FragNormal;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    FragNormal = mat3(transpose(inverse(model))) * aNormal;
}