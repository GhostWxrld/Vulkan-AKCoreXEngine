#version 450

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec4 baseColor;

layout(location = 0) out vec4 outColor;

// Simple noise for variation
float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    f = f * f * (3.0 - 2.0 * f);
    return mix(mix(hash(i), hash(i + vec2(1.0, 0.0)), f.x),
               mix(hash(i + vec2(0.0, 1.0)), hash(i + vec2(1.0, 1.0)), f.x), f.y);
}

void main() {
    float height = fragPos.y;
    float slope = 1.0 - fragNormal.y;
    
    // Water level - make sure this matches vertex shader
    float waterLevel = -2.0;
    
    // Smooth color transitions
    vec3 color;
    
    // Deep water
    if (height < waterLevel - 1.0) {
        color = vec3(0.0, 0.15, 0.35);
    }
    // Shallow water
    else if (height < waterLevel) {
        float blend = smoothstep(waterLevel - 1.0, waterLevel, height);
        color = mix(vec3(0.0, 0.15, 0.35), vec3(0.2, 0.4, 0.8), blend);
    }
    // Beach/sand (immediately above water)
    else if (height < waterLevel + 0.5) {
        color = vec3(0.76, 0.70, 0.50);
    }
    // Grass
    else if (height < 3.0) {
        float blend = smoothstep(waterLevel + 0.5, waterLevel + 1.5, height);
        vec3 sand = vec3(0.76, 0.70, 0.50);
        vec3 grass = vec3(0.3, 0.6, 0.2);
        color = mix(sand, grass, blend);
        
        // Add grass variation
        float grassNoise = noise(fragPos.xz * 2.0) * 0.2;
        color += vec3(grassNoise * 0.1, grassNoise * 0.15, grassNoise * 0.05);
    }
    // Forest (darker green)
    else if (height < 6.0) {
        color = vec3(0.2, 0.4, 0.1);
    }
    // Rocky mountains
    else if (height < 8.0) {
        float blend = smoothstep(6.0, 7.0, height);
        color = mix(vec3(0.5, 0.5, 0.5), vec3(0.6, 0.6, 0.6), blend);
    }
    // Snow caps
    else {
        color = vec3(0.9, 0.9, 1.0);
    }

    // Simple but effective lighting
    vec3 lightDir = normalize(vec3(0.8, 1.0, 0.6));
    float lambert = max(dot(fragNormal, lightDir), 0.0);
    
    // Ambient light with height-based variation
    float ambient = 0.4 + height * 0.05;
    
    // Water specular highlights
    if (height < waterLevel) {
        vec3 viewDir = normalize(-fragPos);
        vec3 reflectDir = reflect(-lightDir, fragNormal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
        color += spec * vec3(0.3, 0.4, 0.6);
    }
    
    vec3 finalColor = color * (ambient + lambert * 0.6);
    outColor = vec4(finalColor, 1.0);
}