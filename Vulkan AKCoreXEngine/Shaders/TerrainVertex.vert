#version 450

layout(push_constant) uniform TerrainPushConstant {
    vec3 pos;
    vec4 color;
    mat4 view;
    mat4 proj;
} tpc;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec3 fragPos;
layout(location = 2) out vec4 baseColor;

// Better Perlin Noise Implementation
vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec2 mod289(vec2 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec3 permute(vec3 x) { return mod289(((x*34.0)+1.0)*x); }

float snoise(vec2 v) {
    const vec4 C = vec4(0.211324865405187, 0.366025403784439, -0.577350269189626, 0.024390243902439);
    vec2 i  = floor(v + dot(v, C.yy));
    vec2 x0 = v - i + dot(i, C.xx);
    vec2 i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
    vec4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;
    i = mod289(i);
    vec3 p = permute(permute(i.y + vec3(0.0, i1.y, 1.0)) + i.x + vec3(0.0, i1.x, 1.0));
    vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
    m = m*m;
    m = m*m;
    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;
    m *= 1.79284291400159 - 0.85373472095314 * (a0*a0 + h*h);
    vec3 g;
    g.x = a0.x * x0.x + h.x * x0.y;
    g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    return 130.0 * dot(m, g);
}

float fbm(vec2 p) {
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 0.5; // Lower frequency for smoother terrain
    
    for (int i = 0; i < 8; i++) {
        value += amplitude * snoise(frequency * p);
        frequency *= 2.0;
        amplitude *= 0.5;
    }
    
    return value;
}

float ridgedMultifractal(vec2 p) {
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 0.5;
    
    for (int i = 0; i < 6; i++) {
        float n = abs(snoise(frequency * p)); // Ridged effect
        n = 1.0 - n; // Invert for ridges
        n *= n; // Sharpen ridges
        value += n * amplitude;
        frequency *= 2.0;
        amplitude *= 0.5;
    }
    
    return value;
}

void main() {
    vec3 worldPos = inPos + tpc.pos;
    
    // SMOOTHER TERRAIN GENERATION
    float baseTerrain = fbm(worldPos.xz * 0.01) * 8.0; // Smooth base
    float mountains = ridgedMultifractal(worldPos.xz * 0.005) * 4.0; // Gentle mountains
    float hills = fbm(worldPos.xz * 0.02) * 2.0; // Small hills
    
    // Combine terrain layers
    float finalHeight = baseTerrain + mountains + hills;
    
    // Ensure water areas are properly low
    float waterLevel = -2.0; // Set water level
    if (finalHeight < waterLevel) {
        finalHeight = waterLevel; // Flatten water areas
    }
    
    worldPos.y += finalHeight;
    fragPos = worldPos;
    
    // Improved normal calculation
    float eps = 2.0; // Larger epsilon for smoother normals
    float h  = fbm(worldPos.xz * 0.01) * 8.0 + ridgedMultifractal(worldPos.xz * 0.005) * 4.0;
    float hx = fbm((worldPos.xz + vec2(eps, 0.0)) * 0.01) * 8.0 + ridgedMultifractal((worldPos.xz + vec2(eps, 0.0)) * 0.005) * 4.0;
    float hz = fbm((worldPos.xz + vec2(0.0, eps)) * 0.01) * 8.0 + ridgedMultifractal((worldPos.xz + vec2(0.0, eps)) * 0.005) * 4.0;
    
    vec3 normal = normalize(vec3(h - hx, eps, h - hz));
    fragNormal = normal;
    
    baseColor = tpc.color;
    
    gl_Position = tpc.proj * tpc.view * vec4(worldPos, 1.0);
}