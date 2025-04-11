#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDirNorm = normalize(-lightDir);

    // Simulate light bouncing with stronger ambient
    float ambientStrength = 0.5; // Increased
    vec3 ambient = ambientStrength * lightColor;

    // Soft diffuse (can be scaled down if ambient is strong)
    float diff = max(dot(norm, lightDirNorm), 0.0);
    vec3 diffuse = 0.5 * diff * lightColor; // 0.5 to soften harshness

    // Optional fill light from opposite direction (like a bounce light)
    vec3 fillDir = normalize(vec3(0.3, -1.0, -0.5)); // comes from the opposite
    float fillDiff = max(dot(norm, fillDir), 0.0);
    vec3 fillLight = 0.2 * fillDiff * lightColor; // subtle secondary light

    vec3 result = (ambient + diffuse + fillLight) * objectColor;
    FragColor = vec4(result, 1.0);
}
