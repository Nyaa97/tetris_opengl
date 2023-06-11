#pragma once
#include <GLFW/glfw3.h>

class Engine {
public:
	void run( GLFWwindow* window ) {
		glfwSetTime( 0 );
		while( !glfwWindowShouldClose( window ) ) {
			float elapsedTime = glfwGetTime();
			glfwSetTime( 0 );

			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			this->update( elapsedTime );

			glfwSwapBuffers( window );
			glfwPollEvents();
		}
	}

	virtual void update( float elapsedTime ) = 0;
};
