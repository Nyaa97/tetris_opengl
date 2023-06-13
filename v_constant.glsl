
#version 420

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform mat4 LightPV1;
uniform mat4 LightPV2;

in vec4 vertex;
in vec4 normal;
in vec4 color;

out vec4 iColor;
out vec4 sCoord1;
out vec4 sCoord2;

void main() {
	iColor = color;

    sCoord1 = LightPV1 * M * vertex;
    sCoord2 = LightPV2 * M * vertex;

	gl_Position = P * V * M * vertex;
}
