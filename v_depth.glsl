#version 330 core

// Input vertex data, different for all executions of this shader.
in vec4 vertex;

// Values that stay constant for the whole mesh.
uniform mat4 depthMVP;
uniform mat4 M;

void main(){
	gl_Position =  depthMVP * M * vertex;
}
