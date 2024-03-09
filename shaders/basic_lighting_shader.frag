#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main()
{
    vec3 normal = normalize(Normal);

    vec3 lightDir = normalize(lightPos - FragPos);

    float ambientStrength = 0.1f;
    vec3 ambient = lightColor * ambientStrength;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lightColor * diff;

    float specularStrength = .5f;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = lightColor * spec * specularStrength;

    FragColor = vec4((diffuse + ambient + specular) * objectColor, 1.0);
}

