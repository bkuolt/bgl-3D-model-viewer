#version 330 core

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    bool isTextured;
    sampler2D texture;
};

uniform Light light;
uniform Material material;

in vec3 pixelNormal;
in vec2 pixelTexCoord;

out vec4 FragColor;

void main() {
    vec3 norm = normalize(pixelNormal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 lighting = ambient + diffuse;

    vec4 baseColor = material.isTextured ? texture(material.texture, pixelTexCoord) : vec4(1.0);
    FragColor = vec4(lighting, 1.0) * baseColor;
}
