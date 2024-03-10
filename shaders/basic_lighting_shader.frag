#version 460 core
out vec4 FragColor;

struct Material
{
    vec3 ambient;
    vec3 specular;
    vec3 diffuse;
    float shininess;
};

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform Material material;

void main()
{
    // normal of current fragment in world space
    vec3 normal = normalize(Normal);

    // direction of fragment position to position of light in world space
    vec3 lightDir = normalize(lightPos - FragPos);

    // unlit color
    vec3 ambient = material.ambient * lightColor;

    // diffuse factor (how much is fragment facing toward light?)
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = material.diffuse * diff;

    // direction of fragment position towards viewer position in world space
    vec3 viewDir = normalize(viewPos - FragPos);

    // direction of fragment position to light reflected over fragment normal
    vec3 reflectDir = reflect(-lightDir, normal);

    // intensity of specular reflection (how small is angle between reflected vector and viewer?)
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (material.specular * spec) * lightColor;

    FragColor = vec4((diffuse + ambient + specular), 1.0);
}

