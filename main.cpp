#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>

int main() {
	GLFWwindow *window;
	if(!glfwInit()) {
		exit(1);
	}

	window = glfwCreateWindow(1000, 1000, "Tetris", NULL, NULL);

	while(!glfwWindowShouldClose(window)) {
		
	}
	
	return 0;
}
