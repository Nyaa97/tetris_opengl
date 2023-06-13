#version 420

uniform sampler2D sMap1;
uniform sampler2D sMap2;

in vec4 iColor;
in vec4 sCoord1;
in vec4 sCoord2;

out vec4 pixelColor;

void main() {
	float visibility = 1;
	if ( texture( sMap1, sCoord1.xy ).x < sCoord1.z - 0.005 ) {
		visibility -= 0.35;
	}

	if ( texture( sMap2, sCoord2.xy ).x < sCoord2.z - 0.005 ) {
		visibility -= 0.35;
	}

	pixelColor = vec4( iColor.rgb * visibility, 1 );
}
