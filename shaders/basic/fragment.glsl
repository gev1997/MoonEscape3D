#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
uniform vec3 lightPos;   // now expected in view space
uniform vec3 viewPos;    // camera position in view space, usually (0,0,0)
uniform vec3 objectColor;
uniform mat4 view;

void main()
{
    // Transform fragment position to view space
    vec3 FragPosView = vec3(view * vec4(FragPos, 1.0));

    // Transform normal to view space
    vec3 NormalView = normalize(mat3(transpose(inverse(view))) * Normal);

    // Ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * objectColor;

    // Diffuse
    vec3 lightDir = normalize(lightPos - FragPosView); // lightPos in view space
    float diff = max(dot(NormalView, lightDir), 0.0);
    vec3 diffuse = diff * objectColor;

    vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0);
}
