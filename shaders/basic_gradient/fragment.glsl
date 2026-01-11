#version 330 core
out vec4 FragColor;

in vec3 Normal;      // Model space
in vec3 FragPos;     // World space  
in vec3 ModelPos;    // Model space

uniform vec3 lightPos;
uniform mat4 view;
uniform mat4 model;  // IMPORTANT: Pass this from C++

void main()
{
    // Transform everything needed for lighting to view space
    vec3 FragPosView = vec3(view * vec4(FragPos, 1.0));
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vec3 NormalView = normalize(normalMatrix * normalize(Normal));
    
    // GRADIENT: Use model-space normal (rotates with object!)
    vec3 norm = normalize(Normal);
    
    // Vertical gradient in model space
    float gradient = (norm.y + 1.0) / 2.0;
    
    // Colors
    vec3 colorTop = vec3(1.0, 0.0, 0.0);      // Red
    vec3 colorMid = vec3(0.0, 1.0, 0.0);      // Green
    vec3 colorBottom = vec3(0.0, 0.0, 1.0);   // Blue
    
    vec3 objectColor;
    if (gradient > 0.66) {
        objectColor = mix(colorMid, colorTop, (gradient - 0.66) / 0.34);
    } else if (gradient > 0.33) {
        objectColor = mix(colorBottom, colorMid, (gradient - 0.33) / 0.34);
    } else {
        objectColor = colorBottom;
    }
    
    // Lighting
    float ambient = 0.2;
    vec3 lightDir = normalize(lightPos - FragPosView);
    float diffuse = max(dot(NormalView, lightDir), 0.0);
    
    vec3 result = objectColor; // * (ambient + diffuse);
    FragColor = vec4(result, 1.0);
}