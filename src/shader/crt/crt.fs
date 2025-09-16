
#version 330 core

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform float time;       // Animation time
uniform float curvature;  // Screen curvature
uniform float scanline;   // Scanline intensity
uniform float pixelSize;  // Pixelation
uniform float glow;       // Glow intensity
uniform float vignette;   // Vignette effect

// Slot mask settings
uniform float maskWidth;      // Width of one RGB stripe
uniform float maskGap;        // Gap between stripes
uniform float maskIntensity;  // Strength of mask effect

void main()
{
    // --- Normalize coordinates to [-1,1] for curvature ---
    vec2 uv = fragTexCoord * 2.0 - 1.0;

    // --- Barrel distortion ---
    uv.x *= 1.0 + curvature * (uv.y * uv.y);
    uv.y *= 1.0 + curvature * (uv.x * uv.x);

    uv = uv * 0.5 + 0.5;

    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
        finalColor = vec4(0.0,0.0,0.0,1.0);
        return;
    }

    // --- Pixelation ---
    uv = floor(uv * pixelSize) / pixelSize;

    // --- Base texture ---
    vec4 texel = texture(texture0, uv) * fragColor * colDiffuse;

    // --- Horizontal scanlines ---
    float scan = sin(fragTexCoord.y * pixelSize * 1.5 + time * 15.0) * 0.5 + 0.5;
    texel.rgb *= mix(1.0, scan, scanline);

    // --- Slot mask pattern ---
    // Repeat pattern horizontally
    float mask = fract(uv.x / maskWidth);

    // Stagger every other column
    float stagger = step(0.5, fract(uv.x * pixelSize * 0.5)) * 0.5; 
    float yOffset = uv.y + stagger;

    // RGB subpixels with gaps
    float rMask = step(0.0, mask) * step(mask, 0.33 - maskGap);
    float gMask = step(0.33 + maskGap, mask) * step(mask, 0.66 - maskGap);
    float bMask = step(0.66 + maskGap, mask) * step(mask, 1.0);
    vec3 maskCol = vec3(rMask, gMask, bMask);

    // Blend with mask
    texel.rgb *= mix(vec3(1.0), maskCol, maskIntensity);

    // --- RGB separation ---
    float offset = 1.0 / pixelSize * 0.5;
    float r = texture(texture0, uv + vec2(offset,0.0)).r;
    float g = texel.g;
    float b = texture(texture0, uv - vec2(offset,0.0)).b;
    texel.rgb = vec3(r, g, b);

    // --- Vignette ---
    float dist = distance(fragTexCoord, vec2(0.5,0.5));
    texel.rgb *= 1.0 - dist * vignette;

    // --- Subtle glow ---
    vec4 glowColor = vec4(
        texture(texture0, uv + vec2(1.0/pixelSize,0.0)).rgb,
        1.0
    );
    texel.rgb += glowColor.rgb * glow;

    // --- Flicker / noise ---
    float flicker = 0.001 * sin(time*40.0 + uv.y*100.0);
    texel.rgb += flicker;

    finalColor = texel;
}


