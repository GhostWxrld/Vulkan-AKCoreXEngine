#version 450

layout(location = 0) in vec3 inPos;
layout(location = 0) out vec3 fragTexCoord;

// Main UBO (binding 0)
layout(binding = 0) uniform UBO {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

void main(){
	mat4 viewNoTranslation = mat4(mat3(ubo.view));

	vec4 pos = ubo.proj * viewNoTranslation * vec4(inPos, 1.0);

	gl_Position = pos.xyww;
	fragTexCoord = inPos;
}