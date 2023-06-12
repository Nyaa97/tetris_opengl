#version 420

uniform sampler2D sMap;

in vec4 iColor;
in vec4 sCoord;

out vec4 pixelColor;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(sMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main() {
	float visibility = ShadowCalculation( sCoord );//texture( sMap, vec3(sCoord.xy, (sCoord.z)/sCoord.w) );
	if ( visibility == 0 ) {//< sCoord.z ) {
	//	visibility = 0.5;
		//	pixelColor = vec4( 1, 1, 0, 1 );
	}

	//visibility = texture( sMap, vec3(sCoord.xy, (sCoord.z)/sCoord.w) );

	pixelColor = vec4( vec3( visibility ), 1 );
}
