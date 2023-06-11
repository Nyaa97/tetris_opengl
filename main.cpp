#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdlib.h>

#include "model.hpp"
#include "cube.hpp"
#include "engine.hpp"
#include "gameobject.hpp"
#include "scene.hpp"
#include "shaderprogram.h"

class Game: public Engine {
	Cube* cube;
	ShaderProgram* sp;
	Scene scene;

public:
	Game(): scene(
		glm::perspective( glm::radians( 90.0f ), 1.0f, 1.0f, 50.0f ),
		glm::lookAt(
			glm::vec3( 0.0f, 0.0f, -10.0f ),
			glm::vec3( 0.0f, 0.0f, 0.0f ),
			glm::vec3( 0.0f, 1.0f, 0.0f )
		)
	) {
		this->sp = new ShaderProgram( "v_constant.glsl", NULL, "f_constant.glsl" );

		this->cube = new Cube();
		this->scene.objects.push_back( ( new GameObject( cube ) )->translate( { -2, 2, 0 } ) );
		this->scene.objects.push_back( ( new GameObject( cube ) )->translate( { 2, 2, 0 } ) );

		this->scene.objects.push_back(
			( new GameObject( NULL, {
				//( new GameObject( cube ) )->translate( { -2, 0, 0 } ),
				( new GameObject( cube ) )->translate( { 0, 0, 0 } )->scale( { 3, 1, 1 } ),
				//( new GameObject( cube ) )->translate( { 2, 0, 0 } ),
				( new GameObject( cube ) )->translate( { -2, -2, 0 } ),
			} ) )->translate( { 0, -2, 0 } )
		);
	}

	~Game() {
		delete this->cube;

		for ( auto &obj : this->scene.objects ) {
			for ( auto &subobj : obj->subobjects ) {
				delete subobj;
			}
			delete obj;
		}

		delete this->sp;
	}

	void update( float elapsedTime ) {
		for ( auto &obj : this->scene.objects ) {
			obj->rotate( 50 * elapsedTime, { 1.0f, 0.0f, 0.0f } );
		}

		this->sp->use(); //Aktywacja programu cieniującego
		scene.draw( this->sp );
	}
};

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

int main() {
	GLFWwindow *window;
	glfwSetErrorCallback(error_callback);

	if(!glfwInit()) {
		exit(1);
	}

	window = glfwCreateWindow(1000, 1000, "Tetris", NULL, NULL);
	if (!window) {
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	glEnable( GL_DEPTH_TEST );

	Game game;
	game.run( window );

	glfwDestroyWindow( window );
	glfwTerminate();

	return 0;
}
