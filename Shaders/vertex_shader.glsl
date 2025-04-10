#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragNormal;

void main()
{
    // Transform vertex position
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // Transform the normal vector to world space (ignoring non-uniform scaling)
    FragNormal = mat3(transpose(inverse(model))) * aNormal;
}
