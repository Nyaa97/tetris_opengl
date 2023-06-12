#version 420

uniform sampler2D texBase;
uniform sampler2D texEmmisive;
uniform sampler2D texNormal;
uniform sampler2D texRoughness;

out vec4 pixelColor;

in vec4 l1;
in vec4 l2;
in vec4 l3;
in vec4 v;
in vec2 iTc;

void main() {
	vec4 kb = texture( texBase, iTc );
	vec4 ke = texture( texEmmisive, iTc );
	vec4 kn = texture( texNormal, iTc );
	vec4 kr = texture( texRoughness, iTc );

	vec4 ml1 = normalize( l1 );
	vec4 ml2 = normalize( l2 );
	vec4 ml3 = normalize( l3 );
	vec4 mv = normalize( v );
	vec4 mn = normalize( vec4( kn.xyz * 2 - 1, 0 ) );

	vec4 mr = reflect( -ml3, mn ) + reflect( -ml2, mn ) + reflect( -ml1, mn );

	float l = dot( mn, ml3 ) + dot( mn, ml2 ) + dot( mn, ml1 );

	float nl = clamp( l, 0, 1 );
	float rv = pow( clamp( dot( mr, mv ), 0, 1 ), 5000 );

	pixelColor = vec4( kb.rgb * nl, kb.a ) + vec4( kr.rgb * rv / 5 , 0 ) + ke;
}
