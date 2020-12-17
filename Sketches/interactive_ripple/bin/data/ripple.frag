#version 150

// ======================================
// Inputs
// ======================================

//should we bring in img/vid background as tex?
uniform sampler2DRect backbuffer; 	//prev buff
uniform sampler2DRect tex0;			//actual buff i.e. the blobs
uniform vec2 canvasDims;
uniform float damping;				//reduces ripple

in vec2 texCoordVarying;	
out vec4 fragColor;	

vec2 offset[4];

// ======================================
// Main
// ======================================
void main()
{

	// Find the normalized text coordinate
	//float maxDim = max(canvasDims.x, canvasDims.y);
	//vec2 normTexCoord = texCoordVarying / maxDim;
	//vec2 st = normTexCoord.xy;
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

	//colour goes in here
	vec3 sum = vec3(1.0, 0.0, 1.0);

	for ( int i = 0; i < 4; i++){
		//color offset at the blob
		sum += texture2DRect(tex0, st + offset[i]).rgb;
	}

	//make an average & subtract center value
	sum = (sum / 2.0) - texture2DRect(backbuffer, st).rgb;
	sum *= damping;

	//I dont think this is working!
	fragColor = vec4(1.0, 0.0, 1.0, 1.0);
	//fragColor = vec4(sum, 1.0);
}



