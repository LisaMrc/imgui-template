#version 330 core
in vec3 TexCoords;
out vec4 FragColor;

void main() {
    // Dégradé basé sur la direction
    vec3 color = normalize(TexCoords) * 0.5 + 0.5;
    FragColor = vec4(color, 1.0);
}
