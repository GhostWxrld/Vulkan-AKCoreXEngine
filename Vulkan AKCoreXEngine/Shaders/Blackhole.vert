#version 450
layout(push_constant) uniform PushConstants {
    mat4 proj;
    mat4 view;
	mat4 model;
	vec3 cameraPos;

} pc;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 fragPos;

void main() {
		fragPos = vec3(pc.model * vec4(inPosition, 1.0));
		gl_Position = pc.proj * pc.view * vec4(fragPos, 1.0);
}