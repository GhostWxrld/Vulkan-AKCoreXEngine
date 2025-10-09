#version 450

layout(location = 0) in vec3 fragPos;  // world-space position from vertex shader
layout(location = 0) out vec4 outColor;

layout(push_constant) uniform PushConstants {
    mat4 model;
    mat4 view;
    mat4 proj;

} pc;

void main() {
    outColor = vec4(0.0, 0.0, 0.0, 1.0); 
}



