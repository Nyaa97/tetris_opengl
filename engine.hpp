#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Engine {
public:
	GLFWwindow* window;

	void init( int w, int h, const char* title ) {
		if( !glfwInit() ) {
			exit( 1 );
		}

		this->window = glfwCreateWindow( w, h, title, NULL, NULL );
		if ( !this->window ) {
			fprintf( stderr, "Nie można utworzyć okna.\n" );
			glfwTerminate();
			exit( 1 );
		}

		glfwMakeContextCurrent( this->window );
		glfwSwapInterval( 1 );

		if ( glewInit() != GLEW_OK ) {
			fprintf( stderr, "Nie można zainicjować GLEW.\n" );
			this->destroy();
			exit( 1 );
		}
	}

	void run() {
		glfwSetTime( 0 );
		while( !glfwWindowShouldClose( this->window ) ) {
			float elapsedTime = glfwGetTime();
			glfwSetTime( 0 );

			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			this->update( elapsedTime );

			glfwSwapBuffers( this->window );
			glfwPollEvents();
		}
	}

	void destroy() {
		glfwDestroyWindow( window );
		glfwTerminate();
	}

	virtual void update( float elapsedTime ) = 0;
};
