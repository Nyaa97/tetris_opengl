#include "model.hpp"
#include <fstream>
#include <vector>
#include "lodepng.h"

std::vector<std::string> split( std::string s ) {
	char buff[20];
	std::vector<std::string> out;
	int j = 0;
	for ( auto& c : s ) {
		if ( c == ' ' ) {
			if ( buff[0] != 0 ) {
				buff[j++] = 0;
				out.push_back( std::string( buff ) );
				j = 0;
				buff[0] = 0;
			}
			continue;
		}
		buff[j++] = c;
	}
	buff[j++] = 0;
	out.push_back( std::string( buff ) );
	return out;
}

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filename);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
	GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

Model::Model( const char* filename, const char* base, const char* emmisive, const char* normal, const char* roughness ) {
	this->colors = nullptr;
	this->vertices = nullptr;
	this->normals = nullptr;
	this->coords = nullptr;
	this->texBase = readTexture(base);
	this->texEmmisive = readTexture(emmisive);
	this->texNormal = readTexture(normal);
	this->texRoughness = readTexture(roughness);

	std::ifstream file( filename );
	std::string line;
	std::vector<float> v, vn, vt;

	int lenV = 0;
	int lenN = 0;
	int lenC = 0;

	while ( std::getline( file, line ) ) {
		if ( line[0] == 'v' ) {
			if ( line[1] == ' ' ) {
				bool skip = true;
				auto parts = split(line);
				for ( auto& x : parts ) {
					if ( skip ) {
						skip = false;
						continue;
					}
					v.push_back( ( float ) atof( x.c_str() ) );
				}
			}
			else if ( line[1] == 'n' ) {
				auto parts = split(line);
				bool skip = true;
				for ( auto& x : parts ) {
					if ( skip ) {
						skip = false;
						continue;
					}
					vn.push_back( ( float ) atof( x.c_str() ) );
				}
			}
			else if ( line[1] == 't' ) {
				auto parts = split(line);
				bool skip = true;
				for ( auto& x : parts ) {
					if ( skip ) {
						skip = false;
						continue;
					}
					vt.push_back( ( float ) atof( x.c_str() ) );
				}
			}
		}
		else if ( line[0] == 'f' ) {
			auto parts = split(line);
			int g = 1;
			while ( g < parts.size() ) {
				char buff[3][10];
				int j = 0;
				int k = 0;
				for ( auto& c : parts[g] ) {
					if ( c == '/' ) {
						if ( j == 0 ) {
							buff[j][k++] = 0;
							int num = atoi( buff[j] ) - 1;
							if ( g > 3 ) {
								this->vertices = ( float* ) realloc( this->vertices, ( lenV + 12 ) * sizeof ( float ) );
								int z = 0;
								for ( int i = 0; i < 8; ++i ) {
									this->vertices[lenV] = this->vertices[lenV - 12 + z];
									++lenV;
									if ( i == 3 ) z = 4;
								}
							}
							else {
								this->vertices = ( float* ) realloc( this->vertices, ( lenV + 4 ) * sizeof ( float ) );
							}
							for ( int i = 0; i < 3; ++i ) {
								this->vertices[lenV++] = v[num * 3 + i];
							}
							this->vertices[lenV++] = 1.0f;
						}
						else if ( j == 1 ) {
							buff[j][k++] = 0;
							int num = atoi( buff[j] ) - 1;
							if ( g > 3 ) {
								this->coords = ( float* ) realloc( this->coords, ( lenC + 6 ) * sizeof ( float ) );
								int z = 0;
								for ( int i = 0; i < 4; ++i ) {
									this->coords[lenC] = this->coords[lenC - 6 + z];
									++lenC;
									if ( i == 1 ) z = 2;
								}
							}
							else {
								this->coords = ( float* ) realloc( this->coords, ( lenC + 2 ) * sizeof ( float ) );
							}
							this->coords[lenC++] = vt[num * 2];
							this->coords[lenC++] = vt[num * 2 + 1];
						}
						++j;
						k = 0;
						continue;
					}
					buff[j][k++] = c;
				}
				if ( j == 2 ) {
					buff[j][k++] = 0;
					int num = atoi( buff[j] ) - 1;
					if ( g > 3 ) {
						this->normals = ( float* ) realloc( this->normals, ( lenN + 12 ) * sizeof ( float ) );
						int z = 0;
						for ( int i = 0; i < 8; ++i ) {
							this->normals[lenN] = this->normals[lenN - 12 + z];
							++lenN;
							if ( i == 3) z = 4;
						}
					}
					else {
						this->normals = ( float* ) realloc( this->normals, ( lenN + 4 ) * sizeof ( float ) );
					}
					for ( int i = 0; i < 3; ++i ) {
						this->normals[lenN++] = vn[num * 3 + i];
					}
					this->normals[lenN++] = 0.0f;
				}
				++g;
			}
		}
	}
	this->vertexCount = lenN / 4;
}
