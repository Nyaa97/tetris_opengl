#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "model.hpp"

class GameObject {
	Model* model;

public:
	glm::mat4 M;
	std::vector<GameObject*> subobjects;
	ShaderProgram* sp;

	GameObject( Model* model, std::vector<GameObject*> subobjects = {}, ShaderProgram* sp = nullptr )
		: model( model ), M( 1.0f ), subobjects( subobjects ), sp( sp ) {
	}

	GameObject* translate( glm::vec3 offset ) {
		this->M = glm::translate( this->M, offset );

		return this;
	}

	GameObject* rotate( float angle, glm::vec3 axis ) {
		this->M = glm::rotate( this->M, glm::radians( angle ), axis );

		return this;
	}

	GameObject* scale( float scale ) {
		return this->scale( glm::vec3( scale ) );
	}

	GameObject* scale( glm::vec3 scale ) {
		this->M = glm::scale( this->M, scale );

		return this;
	}

	GameObject* reset() {
		this->M = glm::mat4( 1.0f );

		return this;
	}

	void draw( ShaderProgram* spParent = nullptr, glm::mat4 parent = glm::mat4( 1.0f ) ) {
		glm::mat4 m = parent * this->M;
		ShaderProgram* sp = this->sp ?: spParent;

		if ( sp && this->model ) {
			sp->use();
			glUniformMatrix4fv( sp->u( "M" ), 1, false, glm::value_ptr( m ) );
			this->model->drawSolid( sp );
		}

		for ( auto &subobj : this->subobjects ) {
			subobj->draw( sp, m );
		}
	}

	void drawWire( ShaderProgram* spParent = nullptr, glm::mat4 parent = glm::mat4( 1.0f ) ) {
		glm::mat4 m = parent * this->M;
		ShaderProgram* sp = this->sp ?: spParent;

		if ( sp && this->model ) {
			sp->use();
			glUniformMatrix4fv( sp->u( "M" ), 1, false, glm::value_ptr( m ) );
			this->model->drawWire( sp );
		}

		for ( auto &subobj : this->subobjects ) {
			subobj->drawWire( sp, m );
		}
	}
};
