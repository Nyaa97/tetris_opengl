#version 420

uniform sampler2D sMap;

in vec4 iColor;
in vec4 sCoord;

out vec4 pixelColor;

void main() {
	float visibility = 1;//texture( sMap, vec3(sCoord.xy, (sCoord.z)/sCoord.w) );
	if ( texture( sMap,sCoord.xy ).z < sCoord.z ) {
		visibility = 0.5;
		//	pixelColor = vec4( 1, 1, 0, 1 );
	}

	//visibility = texture( sMap, vec3(sCoord.xy, (sCoord.z)/sCoord.w) );

	pixelColor = vec4( texture( sMap,sCoord.xy ).rgb, 1 );
}
