#version 330

// Input varying from the vertex shader
in vec2 fragTexCoord; // Texture coordinates
in vec4 fragColor;    // Vertex color (not used here)

// Output color
out vec4 finalColor;

// Uniforms for color correction mode and intensity
uniform sampler2D texture0; // Default texture sampler
uniform int mode;           // 0 - None, 1 - Protanopia, 2 - Deuteranopia, 3 - Tritanopia
uniform float intensity;    // Correction intensity (0.0 to 1.0)

void main()
{
    if (mode == 0) // No correction
    {
        finalColor = texture(texture0, fragTexCoord);
        return;
    }
    // Get the color from the texture
    vec4 tex = texture(texture0, fragTexCoord);

    // Convert to LMS color space
    float L = (17.8824 * tex.r) + (43.5161 * tex.g) + (4.11935 * tex.b);
    float M = (3.45565 * tex.r) + (27.1554 * tex.g) + (3.86714 * tex.b);
    float S = (0.0299566 * tex.r) + (0.184309 * tex.g) + (1.46709 * tex.b);

    float l, m, s;
    if (mode == 1) // Protanopia
    {
        l = 0.0 * L + 2.02344 * M + -2.52581 * S;
        m = 0.0 * L + 1.0 * M + 0.0 * S;
        s = 0.0 * L + 0.0 * M + 1.0 * S;
    }
    else if (mode == 2) // Deuteranopia
    {
        l = 1.0 * L + 0.0 * M + 0.0 * S;
        m = 0.494207 * L + 0.0 * M + 1.24827 * S;
        s = 0.0 * L + 0.0 * M + 1.0 * S;
    }
    else if (mode == 3) // Tritanopia
    {
        l = 1.0 * L + 0.0 * M + 0.0 * S;
        m = 0.0 * L + 1.0 * M + 0.0 * S;
        s = -0.395913 * L + 0.801109 * M + 0.0 * S;
    }

    // Convert back to RGB with error correction
    vec4 error;
    error.r = (0.0809444479 * l) + (-0.130504409 * m) + (0.116721066 * s);
    error.g = (-0.0102485335 * l) + (0.0540193266 * m) + (-0.113614708 * s);
    error.b = (-0.000365296938 * l) + (-0.00412161469 * m) + (0.693511405 * s);
    error.a = 1.0;

    vec4 diff = tex - error;
    vec4 correction;
    correction.r = 0.0;
    correction.g = (diff.r * 0.7) + (diff.g * 1.0);
    correction.b = (diff.r * 0.7) + (diff.b * 1.0);
    correction = tex + correction;
    correction.a = tex.a * intensity;

    // Output the final corrected color
    finalColor = correction;
}
