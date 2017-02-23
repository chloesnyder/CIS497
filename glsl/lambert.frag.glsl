#version 150
// ^ Change this to version 130 if you have compatibility issues

// This is a fragment shader. If you've opened this file first, please open and read lambert.vert.glsl before reading on.
// Unlike the vertex shader, the fragment shader actually does compute the shading of geometry.
// For every pixel in your program's output screen, the fragment shader is run for every bit of geometry that particular pixel overlaps.
// By implicitly interpolating the position data passed into the fragment shader by the vertex shader, the fragment shader
// can compute what color to apply to its pixel based on things like vertex position, light position, and vertex color.


// These are the interpolated values out of the rasterizer, so you can't know
// their specific values without knowing the vertices that contributed to them
in vec4 fs_Nor;
in vec4 fs_LightVec;
in vec4 fs_Col;

uniform vec4 u_Color;

out vec4 out_Col;  // This is the final output color that you will see on your screen for the pixel that is currently being processed.

void main()
{

    vec4 diffuseColor = fs_Col;
    float diffuseTerm = dot(normalize(fs_Nor), normalize(fs_LightVec));

    diffuseTerm = clamp(diffuseTerm, 0, 1);

    float ambientTerm = 0.2;
    float lightIntensity = diffuseTerm + ambientTerm;
    out_Col = vec4(diffuseColor.rgb * lightIntensity, diffuseColor.a);
}
