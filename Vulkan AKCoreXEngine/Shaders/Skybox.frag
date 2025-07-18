#version 450

layout(location = 0) in vec3 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 3) uniform samplerCube skyboxSampler;

void main(){
	outColor = texture(skyboxSampler, normalize(fragTexCoord));

}