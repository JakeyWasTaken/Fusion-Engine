#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPosition;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform sampler2D texture_diffuse1;

void main()
{
    float ambientStrength = 0.25;
    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDirection = normalize(FragPosition - lightPosition);
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = vec3(texture(texture_diffuse1, TexCoords)) * (ambient + diffuse);
    FragColor = vec4(result, 1.0);
}
