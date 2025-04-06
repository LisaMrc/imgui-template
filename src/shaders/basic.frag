#version 330 core

out vec4 FragColor;

uniform vec3 uColor; // Couleur passée depuis l'application

void main()
{
    FragColor = vec4(uColor, 1.0);
}
