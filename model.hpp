#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "shaderprogram.h"

class Model {
protected:
	float* vertices;
	unsigned int vertexCount;

	float* colors;
	float* normals;
	float* coords;
	GLuint texBase;
	GLuint texEmmisive;
	GLuint texNormal;
	GLuint texRoughness;


	Model( const char* filename, const char* base, const char* emmisive, const char* normal, const char* roughness );

	Model( float* vertices, unsigned int vertexCount, float* colors, float* normals )
		: vertices( vertices ), vertexCount( vertexCount ), colors( colors ), normals( normals ), coords( nullptr ) {

	}

	~Model() {
		delete[] this->vertices;
		delete[] this->colors;
		delete[] this->normals;
		delete[] this->coords;
		glDeleteTextures(1, &texBase);
		glDeleteTextures(1, &texEmmisive);
		glDeleteTextures(1, &texNormal);
		glDeleteTextures(1, &texRoughness);
	}

public:
	virtual void drawSolid( ShaderProgram* sp ) {
		GLuint spVertices = sp->a( "vertex" );
		GLuint spNormals = sp->a( "normal" );
		GLuint spColors = sp->a( "color" );
		GLuint spCoords = sp->a( "coord" );

		glEnableVertexAttribArray( spVertices );
		glEnableVertexAttribArray( spNormals );
		glEnableVertexAttribArray( spColors );
		glEnableVertexAttribArray( spCoords );


		glVertexAttribPointer( spVertices, 4, GL_FLOAT, false, 0, this->vertices );
		glVertexAttribPointer( spNormals, 4, GL_FLOAT, false, 0, this->normals );
		glVertexAttribPointer( spCoords, 2, GL_FLOAT, false, 0, this->coords );

		if ( !this->colors ) {
			glUniform1i(sp->u("texBase"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->texBase);
			glUniform1i(sp->u("texEmmisive"), 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, this->texEmmisive);
			glUniform1i(sp->u("texNormal"), 2);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, this->texNormal);
			glUniform1i(sp->u("texRoughness"), 3);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, this->texRoughness);
		} else {
			glVertexAttribPointer( spColors, 4, GL_FLOAT, false, 0, this->colors );
		}

		glDrawArrays( GL_TRIANGLES, 0, this->vertexCount );

		glDisableVertexAttribArray( spVertices );
		glDisableVertexAttribArray( spNormals );
		glDisableVertexAttribArray( spColors );
	}

	void drawWire( ShaderProgram* sp ) {
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		this->drawSolid( sp );
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
};
