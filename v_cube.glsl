#version 420

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform mat4 LightPV1;
uniform mat4 LightPV2;

in vec4 vertex;
in vec4 normal;
in vec2 coord;

out vec4 l1;
out vec4 l2;
//out vec4 l3;
out vec4 v;
out vec2 iTc;
out vec4 sCoord1;
out vec4 sCoord2;

void main() {
	/*float w = 20;//4.5;
	vec4 lp1 = vec4( -w, 0, 2, 1 );
	vec4 lp2 = vec4( w, 0, 2, 1 );
	vec4 lp3 = vec4( 0, 0, 6 - w, 1 );*/

	vec4 lp1 = V * vec4( 5, 10, -15, 1 );
	vec4 lp2 = V * vec4( 3, 13, -15, 1 );

	vec4 vp = V * M * vertex;

	l1 = normalize( lp1 - vp );
	l2 = normalize( lp2 - vp );
	//l3 = normalize( lp3 - vp );
	v = normalize( vec4( 0, 0, 0, 1 ) - vp );

	iTc = coord;

	gl_Position = P * vp;

	sCoord1 = LightPV1 * M * vertex;
	sCoord2 = LightPV2 * M * vertex;
}
