#version 420

uniform sampler2D texBase;
uniform sampler2D texEmmisive;
uniform sampler2D texNormal;
uniform sampler2D texRoughness;
uniform sampler2D sMap1;
uniform sampler2D sMap2;

out vec4 pixelColor;

in vec4 l1;
in vec4 l2;
in vec4 l3;
in vec4 v;
in vec2 iTc;
in vec4 sCoord1;
in vec4 sCoord2;

void main() {
	vec4 kb = texture( texBase, iTc );
	vec4 ke = texture( texEmmisive, iTc );
	vec4 kn = texture( texNormal, iTc );
	vec4 kr = texture( texRoughness, iTc );

	vec4 ml1 = normalize( l1 );
	vec4 ml2 = normalize( l2 );
	//vec4 ml3 = normalize( l3 );
	vec4 mv = normalize( v );
	vec4 mn = normalize( vec4( kn.xyz * 2 - 1, 0 ) );

	float mr1 = pow( clamp( dot( reflect( -ml1, mn ), mv ), 0, 1 ), 150 );
	float mr2 = pow( clamp( dot( reflect( -ml2, mn ), mv ), 0, 1 ), 150 );
	/*float mr3 = pow( clamp( dot( reflect( -ml3, mn ), mv ), 0, 1 ), 500 );*/;

	// float l = /*dot( mn, ml3 ) +*/ dot( mn, ml2 ) + dot( mn, ml1 );

	//float nl = clamp( l, 0, 1 );
	float rv = clamp( mr1 + mr2, 0, 1 );

	float s = 1;
	float tmp = 1;
	if ( texture( sMap1, sCoord1.xy ).x < sCoord1.z - 0.002 ) {
		s -= 0.35;
		tmp -= 0.5;
	}

	if ( texture( sMap2, sCoord2.xy ).x < sCoord2.z - 0.002 ) {
		s -= 0.35;
		tmp -= 0.5;
	}

	rv *= tmp;

	pixelColor = vec4( kb.rgb * s, kb.a ) + vec4( kr.rgb * rv / 5, 0 ) + ke / 2;
}
