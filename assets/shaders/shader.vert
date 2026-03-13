#version 450

layout(location = 0) in vec2 inPosition;   // Matches vertex attr 0
layout(location = 1) in vec3 inColor;      // Matches vertex attr 1

layout(location = 0) out vec3 fragColor;   // Output vec3 (3 components)

void main() {
    gl_Position = vec4(inPosition, 0.0, 1.0);
    fragColor = inColor;  // Pass your clean vertex colors
}

