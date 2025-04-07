#version 330 core
out vec4 FragColor;

in vec3 FragNormal;
uniform vec3 lightDir; // e.g., vec3(0, 1, 1)

void main()
{
    float diff = max(dot(normalize(FragNormal), normalize(lightDir)), 0.0);
    vec3 color = vec3(1.0) * diff; // basic diffuse shading
    FragColor = vec4(color, 1.0);
}