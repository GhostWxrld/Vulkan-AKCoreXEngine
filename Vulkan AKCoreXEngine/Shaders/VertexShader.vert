#version 450

layout (binding = 0) uniform UniformBufferObject{
	mat4 model;
	mat4 view;
	mat4 proj;
}ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord; 
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;

void main(){
	gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
	fragColor = inColor;
	fragTexCoord = inTexCoord;
	
	//Transform the normal vector to world space
	mat3 normalMatrix = mat3(transpose(inverse(ubo.model)));
	fragNormal = normalize(normalMatrix * inNormal);
}