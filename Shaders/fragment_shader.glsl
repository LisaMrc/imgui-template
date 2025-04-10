#version 330 core

in vec3  FragNormal;
out vec4 FragColor;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 objectColor; // <- New uniform

void main()
{
    vec3  norm  = normalize(FragNormal);
    vec3  light = normalize(lightDir);
    float diff  = max(dot(norm, light), 0.0);

    vec3 baseColor = vec3(1.0, 1.0, 1.0);
    vec3 shaded = baseColor * lightColor * diff;
    vec3 shaded = objectColor * diff;

    FragColor = vec4(shaded, 1.0);
}
