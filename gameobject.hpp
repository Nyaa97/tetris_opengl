#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "model.hpp"

class GameObject {
	Model* model;
	glm::mat4 M;

public:
	GameObject( Model* model ) : model( model ), M( 1.0f ) {
	}

	GameObject* translate( glm::vec3 offset ) {
		this->M = glm::translate( this->M, offset );

		return this;
	}

	GameObject* rotate( float angle, glm::vec3 axis ) {
		this->M = glm::rotate( this->M, glm::radians( angle ), axis );

		return this;
	}

	GameObject* scale( glm::vec3 scale ) {
		this->M = glm::scale( this->M, scale );

		return this;
	}

	GameObject* reset() {
		this->M = glm::mat4( 1.0f );

		return this;
	}

	void draw( ShaderProgram* sp ) {
		glUniformMatrix4fv( sp->u( "M" ), 1, false, glm::value_ptr( this->M ) );
		this->model->drawSolid( sp );
	}

	void drawWire( ShaderProgram* sp ) {
		glUniformMatrix4fv( sp->u( "M" ), 1, false, glm::value_ptr( this->M ) );
		this->model->drawWire( sp );
	}
};
