#pragma once
#include "model.hpp"
namespace CubeInternal {
	unsigned int vertexCount=36;

	float vertices[]={
		1.0f,-1.0f,-1.0f,1.0f,
		-1.0f, 1.0f,-1.0f,1.0f,
		-1.0f,-1.0f,-1.0f,1.0f,

		1.0f,-1.0f,-1.0f,1.0f,
		1.0f, 1.0f,-1.0f,1.0f,
		-1.0f, 1.0f,-1.0f,1.0f,


		-1.0f,-1.0f, 1.0f,1.0f,
		1.0f, 1.0f, 1.0f,1.0f,
		1.0f,-1.0f, 1.0f,1.0f,

		-1.0f,-1.0f, 1.0f,1.0f,
		-1.0f, 1.0f, 1.0f,1.0f,
		1.0f, 1.0f, 1.0f,1.0f,

		1.0f,-1.0f, 1.0f,1.0f,
		1.0f, 1.0f,-1.0f,1.0f,
		1.0f,-1.0f,-1.0f,1.0f,

		1.0f,-1.0f, 1.0f,1.0f,
		1.0f, 1.0f, 1.0f,1.0f,
		1.0f, 1.0f,-1.0f,1.0f,

		-1.0f,-1.0f,-1.0f,1.0f,
		-1.0f, 1.0f, 1.0f,1.0f,
		-1.0f,-1.0f, 1.0f,1.0f,

		-1.0f,-1.0f,-1.0f,1.0f,
		-1.0f, 1.0f,-1.0f,1.0f,
		-1.0f, 1.0f, 1.0f,1.0f,

		-1.0f,-1.0f,-1.0f,1.0f,
		1.0f,-1.0f, 1.0f,1.0f,
		1.0f,-1.0f,-1.0f,1.0f,

		-1.0f,-1.0f,-1.0f,1.0f,
		-1.0f,-1.0f, 1.0f,1.0f,
		1.0f,-1.0f, 1.0f,1.0f,

		-1.0f, 1.0f, 1.0f,1.0f,
		1.0f, 1.0f,-1.0f,1.0f,
		1.0f, 1.0f, 1.0f,1.0f,

		-1.0f, 1.0f, 1.0f,1.0f,
		-1.0f, 1.0f,-1.0f,1.0f,
		1.0f, 1.0f,-1.0f,1.0f,

	};

	float colors[]={
		1.0f,0.0f,0.0f,1.0f,
		1.0f,0.0f,0.0f,1.0f,
		1.0f,0.0f,0.0f,1.0f,

		1.0f,0.0f,0.0f,1.0f,
		1.0f,0.0f,0.0f,1.0f,
		1.0f,0.0f,0.0f,1.0f,

		0.0f,1.0f,0.0f,1.0f,
		0.0f,1.0f,0.0f,1.0f,
		0.0f,1.0f,0.0f,1.0f,

		0.0f,1.0f,0.0f,1.0f,
		0.0f,1.0f,0.0f,1.0f,
		0.0f,1.0f,0.0f,1.0f,

		0.0f,0.0f,1.0f,1.0f,
		0.0f,0.0f,1.0f,1.0f,
		0.0f,0.0f,1.0f,1.0f,

		0.0f,0.0f,1.0f,1.0f,
		0.0f,0.0f,1.0f,1.0f,
		0.0f,0.0f,1.0f,1.0f,

		1.0f,1.0f,0.0f,1.0f,
		1.0f,1.0f,0.0f,1.0f,
		1.0f,1.0f,0.0f,1.0f,

		1.0f,1.0f,0.0f,1.0f,
		1.0f,1.0f,0.0f,1.0f,
		1.0f,1.0f,0.0f,1.0f,

		0.0f,1.0f,1.0f,1.0f,
		0.0f,1.0f,1.0f,1.0f,
		0.0f,1.0f,1.0f,1.0f,

		0.0f,1.0f,1.0f,1.0f,
		0.0f,1.0f,1.0f,1.0f,
		0.0f,1.0f,1.0f,1.0f,

		1.0f,1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,1.0f,

		1.0f,1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,1.0f,
	};

	float normals[]={
		0.0f, 0.0f,-1.0f,0.0f,
		0.0f, 0.0f,-1.0f,0.0f,
		0.0f, 0.0f,-1.0f,0.0f,

		0.0f, 0.0f,-1.0f,0.0f,
		0.0f, 0.0f,-1.0f,0.0f,
		0.0f, 0.0f,-1.0f,0.0f,

		0.0f, 0.0f, 1.0f,0.0f,
		0.0f, 0.0f, 1.0f,0.0f,
		0.0f, 0.0f, 1.0f,0.0f,

		0.0f, 0.0f, 1.0f,0.0f,
		0.0f, 0.0f, 1.0f,0.0f,
		0.0f, 0.0f, 1.0f,0.0f,

		1.0f, 0.0f, 0.0f,0.0f,
		1.0f, 0.0f, 0.0f,0.0f,
		1.0f, 0.0f, 0.0f,0.0f,

		1.0f, 0.0f, 0.0f,0.0f,
		1.0f, 0.0f, 0.0f,0.0f,
		1.0f, 0.0f, 0.0f,0.0f,

		-1.0f, 0.0f, 0.0f,0.0f,
		-1.0f, 0.0f, 0.0f,0.0f,
		-1.0f, 0.0f, 0.0f,0.0f,

		-1.0f, 0.0f, 0.0f,0.0f,
		-1.0f, 0.0f, 0.0f,0.0f,
		-1.0f, 0.0f, 0.0f,0.0f,

		0.0f,-1.0f, 0.0f,0.0f,
		0.0f,-1.0f, 0.0f,0.0f,
		0.0f,-1.0f, 0.0f,0.0f,

		0.0f,-1.0f, 0.0f,0.0f,
		0.0f,-1.0f, 0.0f,0.0f,
		0.0f,-1.0f, 0.0f,0.0f,

		0.0f, 1.0f, 0.0f,0.0f,
		0.0f, 1.0f, 0.0f,0.0f,
		0.0f, 1.0f, 0.0f,0.0f,

		0.0f, 1.0f, 0.0f,0.0f,
		0.0f, 1.0f, 0.0f,0.0f,
		0.0f, 1.0f, 0.0f,0.0f,
	};

	float vertexNormals[]={
		1.0f,-1.0f,-1.0f,0.0f,
		-1.0f, 1.0f,-1.0f,0.0f,
		-1.0f,-1.0f,-1.0f,0.0f,

		1.0f,-1.0f,-1.0f,0.0f,
		1.0f, 1.0f,-1.0f,0.0f,
		-1.0f, 1.0f,-1.0f,0.0f,


		-1.0f,-1.0f, 1.0f,0.0f,
		1.0f, 1.0f, 1.0f,0.0f,
		1.0f,-1.0f, 1.0f,0.0f,

		-1.0f,-1.0f, 1.0f,0.0f,
		-1.0f, 1.0f, 1.0f,0.0f,
		1.0f, 1.0f, 1.0f,0.0f,

		1.0f,-1.0f, 1.0f,0.0f,
		1.0f, 1.0f,-1.0f,0.0f,
		1.0f,-1.0f,-1.0f,0.0f,

		1.0f,-1.0f, 1.0f,0.0f,
		1.0f, 1.0f, 1.0f,0.0f,
		1.0f, 1.0f,-1.0f,0.0f,

		-1.0f,-1.0f,-1.0f,0.0f,
		-1.0f, 1.0f, 1.0f,0.0f,
		-1.0f,-1.0f, 1.0f,0.0f,

		-1.0f,-1.0f,-1.0f,0.0f,
		-1.0f, 1.0f,-1.0f,0.0f,
		-1.0f, 1.0f, 1.0f,0.0f,

		-1.0f,-1.0f,-1.0f,0.0f,
		1.0f,-1.0f, 1.0f,0.0f,
		1.0f,-1.0f,-1.0f,0.0f,

		-1.0f,-1.0f,-1.0f,0.0f,
		-1.0f,-1.0f, 1.0f,0.0f,
		1.0f,-1.0f, 1.0f,0.0f,

		-1.0f, 1.0f, 1.0f,0.0f,
		1.0f, 1.0f,-1.0f,0.0f,
		1.0f, 1.0f, 1.0f,0.0f,

		-1.0f, 1.0f, 1.0f,0.0f,
		-1.0f, 1.0f,-1.0f,0.0f,
		1.0f, 1.0f,-1.0f,0.0f,
	};

	float texCoords[]={
		1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
		1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

		1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
		1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

		1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
		1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

		1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
		1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

		1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
		1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

		1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
		1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
	};
}

class Cube: public Model {
public:
	Cube( glm::vec3 color = { -1, -1, -1 } ): Model(
		CubeInternal::vertices,
		CubeInternal::vertexCount,
		CubeInternal::colors,
		CubeInternal::normals
	) {
		if ( color.r >= 0 ) {
			this->colors = new float[this->vertexCount * 4];

			for ( size_t i = 0; i < this->vertexCount; i++ ) {
				this->colors[4 * i + 0] = color.r;
				this->colors[4 * i + 1] = color.g;
				this->colors[4 * i + 2] = color.b;
				this->colors[4 * i + 3] = 1.0f;
			}
		}
	}
};

class CubeWire: public Model {
public:
	CubeWire(): Model(
		CubeInternal::vertices,
		CubeInternal::vertexCount,
		CubeInternal::colors,
		CubeInternal::normals
	) {

	}

	virtual void drawSolid( ShaderProgram* sp, bool smooth = false ) {
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		Model::drawSolid( sp, smooth );
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
};
