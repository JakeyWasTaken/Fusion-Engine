#version 330 core
struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
};

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPosition;
in mat3 TBN;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform vec3 lightColor;
uniform Material material;

void main()
{
    float ambientStrength = 0.25;
    vec3 ambient = ambientStrength * lightColor;

    vec3 Normal = texture(material.texture_normal1, TexCoords).rgb;
    Normal = Normal * 2.0 - 1.0;
    Normal = normalize(TBN * Normal);

    vec3 lightDirection = normalize(FragPosition - lightPosition);
    float diff = max(dot(Normal, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = vec3(texture(material.texture_diffuse1, TexCoords)) * (ambient + diffuse);
    FragColor = vec4(result, 1.0);
}
