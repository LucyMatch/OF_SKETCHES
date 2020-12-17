#version 150

// ======================================
// Inputs
// ======================================

uniform sampler2DRect tex0; 	//displacement
uniform sampler2DRect tex1;		//background	

in vec2 texCoordVarying;	
out vec4 fragColor;

vec2 offset[4];

// ======================================
// Main
// ======================================
void main()
{

	vec2 st = texCoordVarying;
	// vec2 st = gl_TexCoord[0].st;

	offset[0] = vec2(-1.0, 0.0);
	offset[1] = vec2(1.0, 0.0);
	offset[2] = vec2(0.0, 1.0);
	offset[3] = vec2(0.0, -1.0);

	//  Grab the information arround the active pixel
	//
	//      [3]
	//
	//  [0]  st  [1]
	//
	//      [2]

	float offsetX = texture2DRect(tex0, st + offset[0]).r - texture2DRect(tex0, st + offset[1]).r;
	float offsetY = texture2DRect(tex0, st + offset[3]).r - texture2DRect(tex0, st + offset[2]).r;

	float shading = offsetX;

	vec4 pixel = texture2DRect(tex1, st + vec2(offsetX, offsetY));

	pixel.r += shading;
	pixel.g += shading;
	pixel.b += shading;

	fragColor = pixel;
}



