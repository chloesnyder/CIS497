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


// http://paulbourke.net/texture_colour/colourspace/ Colour Ramping for Data Visualisation
vec4 getColor(float v,float vmin, float vmax)
{
    vec4 c = vec4(1.0,1.0,1.0,1.0); // white
    float dv;

    if (v < vmin)
	v = vmin;
    if (v > vmax)
	v = vmax;
    dv = vmax - vmin;

    if (v < (vmin + 0.25 * dv)) {
	c.r = 0;
	c.g = 0;
	//c.g = 4 * (v - vmin) / dv;
    } else if (v < (vmin + 0.5 * dv)) {
	c.r = 0;
	c.b = 0;
	//c.b = 1 + 4 * (vmin + 0.25 * dv - v) / dv;
    } else if (v < (vmin + 0.75 * dv)) {
	c.r = 4 * (v - vmin - 0.5 * dv) / dv;
	c.b = 0;
    } else {
	//c.g = 1 + 4 * (vmin + 0.75 * dv - v) / dv;
	c.g = 0;
	c.b = 0;
    }

    return(c);
}

void main()
{

    vec4 diffuseColor = fs_Col;
    float diffuseTerm = dot(normalize(fs_Nor), normalize(fs_LightVec));

    diffuseTerm = clamp(diffuseTerm, 0, 1);

    float ambientTerm = 0.2;
    float lightIntensity = diffuseTerm + ambientTerm;

    // map the diffuse color (gray) to heat map color based on the density of the pixel
    // density is the alpha value, where a = r + g + b / 3

    vec4 heatColor = getColor(diffuseColor.a, 0, 1);
    if (heatColor.r > 0)
    {

	out_Col = vec4(heatColor.rgb * lightIntensity, 1);
    } else {
	out_Col = vec4(0,0,0,0);
    }


}
