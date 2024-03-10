#version 460 core
out vec4 FragColor;

struct Material
{
    vec3 specular;
    sampler2D diffuse;
    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 specular;
    vec3 diffuse;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main()
{
    vec3 diffuseAmbient = vec3(texture(material.diffuse, TexCoords));
    // normal of current fragment in world space
    vec3 normal = normalize(Normal);

    // direction of fragment position to position of light in world space
    vec3 lightDir = normalize(light.position - FragPos);

    // unlit color
    vec3 ambient = diffuseAmbient * light.ambient;

    // diffuse factor (how much is fragment facing toward light?)
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = (diffuseAmbient * diff) * light.diffuse;

    // direction of fragment position towards viewer position in world space
    vec3 viewDir = normalize(viewPos - FragPos);

    // direction of fragment position to light reflected over fragment normal
    vec3 reflectDir = reflect(-lightDir, normal);

    // intensity of specular reflection (how small is angle between reflected vector and viewer?)
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (material.specular * spec) * light.specular;

    FragColor = vec4((diffuse + ambient + specular), 1.0);
}

