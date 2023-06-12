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

	Model( float* vertices, unsigned int vertexCount, float* colors, float* normals )
		: vertices( vertices ), vertexCount( vertexCount ), colors( colors ), normals( normals ) {

	}

	~Model() {
		delete[] this->vertices;
		delete[] this->colors;
		delete[] this->normals;
	}

public:
	virtual void drawSolid( ShaderProgram* sp, bool smooth = false ) {
		GLuint spVertices = sp->a( "vertex" );
		GLuint spNormals = sp->a( "normal" );
		GLuint spColors = sp->a( "color" );

		glEnableVertexAttribArray( spVertices );
        glEnableVertexAttribArray( spNormals );
        glEnableVertexAttribArray( spColors );


        glVertexAttribPointer( spVertices, 4, GL_FLOAT, false, 0, this->vertices );
        glVertexAttribPointer( spNormals, 4, GL_FLOAT, false, 0, this->normals );
        //else glVertexAttribPointer(1,4,GL_FLOAT,false,0,vertexNormals);
        // glVertexAttribPointer(2,4,GL_FLOAT,false,0,texCoords);
        glVertexAttribPointer( spColors, 4, GL_FLOAT, false, 0, this->colors );

        glDrawArrays( GL_TRIANGLES, 0, this->vertexCount );

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
