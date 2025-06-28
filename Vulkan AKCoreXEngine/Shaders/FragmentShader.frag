#version 450


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;

layout(binding = 1) uniform sampler2D texSampler;

//Light UBO
layout (binding = 2) uniform Light{
	vec4 lightDir;								
	vec4 lightColor;
	float lightIntensity;
	float ambient;
}light;

layout(location = 0) out vec4 outColor;

void main(){

	vec3 normal = normalize(fragNormal);
	vec3 lightDirNormal = normalize(light.lightDir.xyz);

	float diff = max(dot(normal, lightDirNormal), 0.0);

	float ambient = light.ambient;
	vec3 baseColor = fragColor * texture(texSampler, fragTexCoord).rgb;
	vec3 lighting = light.lightColor.rgb * (ambient + diff * light.lightIntensity);
	vec4 finalColor = vec4(baseColor * lighting, 1.0);

	outColor = finalColor;
}