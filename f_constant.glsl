#version 420

uniform sampler2DShadow sMap;

in vec4 iColor;
in vec4 sCoord;

out vec4 pixelColor;

void main() {
	float visibility = 1.0;
	if ( texture( sMap, vec3(sCoord.xy, (sCoord.z)/sCoord.w) ) < sCoord.z - 0.005 ) {
			visibility = 0.5;
			pixelColor = vec4( 1, 1, 0, 1 );
	}

	//visibility = texture( sMap, vec3(sCoord.xy, (sCoord.z)/sCoord.w) );

	else pixelColor = vec4( texture( sMap, vec3(sCoord.xy, (sCoord.z)/sCoord.w) ), visibility * iColor.gb, 1) ;//vec4( iColor.rgb * visibility, 1 );
}
