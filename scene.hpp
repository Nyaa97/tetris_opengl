#pragma once
#include <vector>
#include "gameobject.hpp"

class Scene {
public:
	glm::mat4 P, V;
	std::vector<GameObject*> objects;

	Scene( glm::mat4 P, glm::mat4 V, std::vector<GameObject*> objects = {} )
		: P( P ), V( V ), objects( objects ) {
	}

	void draw( ShaderProgram* sp ) {
		if ( sp ) {
			this->setPV( sp );
		}

		for ( auto &obj : this->objects ) {
			obj->draw( sp );
		}
	}

	void setPV( ShaderProgram* sp ) {
		glUniformMatrix4fv( sp->u( "P" ), 1, false, glm::value_ptr( P ) );
		glUniformMatrix4fv( sp->u( "V" ), 1, false, glm::value_ptr( V ) );
	}
};
