
#version 420

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform mat4 DepthBiasMVP;

in vec4 vertex;
in vec4 normal;
in vec4 color;

out vec4 iColor;
out vec4 sCoord;


void main() {


	iColor = color;

    sCoord = DepthBiasMVP * vertex;

	gl_Position = P * V * M * vertex;
}
