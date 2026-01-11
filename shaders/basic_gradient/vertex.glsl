#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 Normal;     // Will be in model space
out vec3 FragPos;    // In world space
out vec3 ModelPos;   // Add this for position-based gradients

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // World position (for lighting)
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Model-space normal (for gradient that rotates with object)
    Normal = aNormal;
    
    // Model-space position (optional, for other gradient types)
    ModelPos = aPos;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}