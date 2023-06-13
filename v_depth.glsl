#version 420

in vec4 vertex;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main(){
	gl_Position = P * V * M * vertex;
}
