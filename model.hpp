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
	virtual void drawSolid( ShaderProgram* sp, bool smooth = false ) {
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
		//else glVertexAttribPointer(1,4,GL_FLOAT,false,0,vertexNormals);
		// glVertexAttribPointer(2,4,GL_FLOAT,false,0,texCoords);
		glVertexAttribPointer( spColors, 4, GL_FLOAT, false, 0, this->colors );
		glVertexAttribPointer( spCoords, 2, GL_FLOAT, false, 0, this->coords );

		if ( !this->colors ) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->texBase);
			glUniform1i(sp->u("texBase"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, this->texEmmisive);
			glUniform1i(sp->u("texEmmisive"), 1);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, this->texNormal);
			glUniform1i(sp->u("texNormal"), 2);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, this->texRoughness);
			glUniform1i(sp->u("texRoughness"), 3);
		}

		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glDrawArrays( GL_TRIANGLES, 0, this->vertexCount );
		//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

		glDisableVertexAttribArray( spVertices );
		glDisableVertexAttribArray( spNormals );
		glDisableVertexAttribArray( spColors );
	}

	void drawWire( ShaderProgram* sp, bool smooth = false ) {
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

		this->drawSolid( sp, smooth );

		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
};
