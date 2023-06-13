#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

#include <stdlib.h>
#include <iostream>
#include <random>
#include <stack>
#include <vector>

#include "model.hpp"
#include "cube.hpp"
#include "cube2.hpp"
#include "engine.hpp"
#include "gameobject.hpp"
#include "scene.hpp"
#include "shaderprogram.h"

const float TIME_STEP = 5.0f;
const float TIME_ANIM_FALL = .25f;
const float FACTOR_SPEED_UP = 50;
bool speedUp = false;

struct Vec3Int {
	int x, y, z;
};

enum class Action {
	NONE,
	BLOCK_LEFT,
	BLOCK_RIGHT,
	BLOCK_FWD,
	BLOCK_BACK,
	CAMERA_LEFT,
	CAMERA_RIGHT,
	CAMERA_FWD,
	CAMERA_BACK,
	ROTATE_LEFT,
	ROTATE_RIGHT,
	ROTATE_FWD,
	ROTATE_BACK,
};

enum class Animation {
	NONE,
	BLOCK_LEFT,
	BLOCK_RIGHT,
	BLOCK_FWD,
	BLOCK_BACK,
	BLOCK_FALL
};

enum class CameraAngle {
	FRONT,
	LEFT,
	RIGHT,
	TOP
};

std::stack<Action> actions;

struct TetrisBlock {
	std::vector<Vec3Int> blocks;

	bool isAboveBoard() {
		for ( auto &b : blocks ) {
			if ( b.y >= 20 ) {
				return true;
			}
		}

		return false;
	}

	bool rotX( bool* board, int dir = -1 ) {
		int nx = 99, mx = 0;
		int ny = 99, my = 0;

		for ( auto &b : blocks ) {
			nx = std::min( nx, b.x );
			ny = std::min( ny, b.y );
			mx = std::max( mx, b.x );
			my = std::max( my, b.y );
		}

		mx -= nx;
		my -= ny;

		for ( auto &b : blocks ) {
			b.x -= nx;
			b.y -= ny;

			if ( dir == -1 ) {
				b.x = mx - b.x;
				std::swap( b.x, b.y );
			} else {
				std::swap( b.x, b.y );
				b.x = mx - b.x;
			}

			b.x += nx;
			b.y += ny;
		}

		for ( auto &b : blocks ) {
			if (
				b.x > 9
				|| b.x < 0
				|| b.y < 0
				|| ( b.y < 20 && board[b.x * 200 + b.y * 10 + b.z] )
			) {
				for ( auto &b : blocks ) {
					b.x -= nx;
					b.y -= ny;


					if ( dir == -1 ) {
						std::swap( b.x, b.y );
						b.x = mx - b.x;
					} else {
						b.x = mx - b.x;
						std::swap( b.x, b.y );
					}
					std::swap( b.x, b.y );

					b.x += nx;
					b.y += ny;
				}

				return false;
			}
		}

		return true;
	}

	bool rotZ( bool* board, int dir = -1 ) {
		int nz = 99, mz = 0;
		int ny = 99, my = 0;

		for ( auto &b : blocks ) {
			nz = std::min( nz, b.z );
			ny = std::min( ny, b.y );
			mz = std::max( mz, b.z );
			my = std::max( my, b.y );
		}

		mz -= nz;
		my -= ny;

		for ( auto &b : blocks ) {
			b.z -= nz;
			b.y -= ny;

			if ( dir == -1 ) {
				b.z = mz - b.z;
				std::swap( b.z, b.y );
			} else {
				std::swap( b.z, b.y );
				b.z = mz - b.z;
			}

			b.z += nz;
			b.y += ny;
		}

		for ( auto &b : blocks ) {
			if (
				b.z > 9
				|| b.z < 0
				|| b.y < 0
				|| ( b.y < 20 && board[b.x * 200 + b.y * 10 + b.z] )
			) {
				for ( auto &b : blocks ) {
					b.z -= nz;
					b.y -= ny;

					if ( dir == -1 ) {
						std::swap( b.z, b.y );
						b.z = mz - b.z;
					} else {
						b.z = mz - b.z;
						std::swap( b.z, b.y );
					}
					std::swap( b.z, b.y );

					b.z += nz;
					b.y += ny;
				}

				return false;
			}
		}

		return true;
	}

	bool checkX( bool* board, int dir = -1 ) {
		for ( auto &b : blocks ) {
			if (
				( b.x + dir ) > 9
				|| ( b.x + dir ) < 0
				|| ( b.y < 20 && board[( b.x + dir ) * 200 + b.y * 10 + b.z] )
			) {
				return false;
			}
		}

		for ( auto &b : blocks ) {
			b.x += dir;
		}

		return true;
	}

	bool checkZ( bool* board, int dir = -1 ) {
		for ( auto &b : blocks ) {
			if (
				( b.z + dir ) > 9
				|| ( b.z + dir ) < 0
				|| ( b.y < 20 && board[b.x * 200 + b.y * 10 + ( b.z + dir )] )
			) {
				return false;
			}
		}

		for ( auto &b : blocks ) {
			b.z += dir;
		}

		return true;
	}

	bool checkDown( bool* board ) {
		for ( auto &b : blocks ) {
			if ( b.y < 21 && ( b.y <= 0 || board[b.x * 200 + ( b.y - 1 ) * 10 + b.z] ) ) {
				return false;
			}
		}

		for ( auto &b : blocks ) {
			b.y -= 1;
		}

		return true;
	}

	void lock( bool* board ) {
		for ( auto &b : blocks ) {
			board[b.x * 200 + b.y * 10 + b.z] = true;
		}
	}
};

class Game: public Engine {
	Cube* cube, *cubeWhite;
	Cube2* cube2;
	CubeWire* cubewire;
	ShaderProgram* sp, *spCube, *spDepth;
	Scene scene, light1, light2;
	float time1, time2;
	bool* board;

	GameObject* blocks;
	GameObject* bg;

	Animation curA;
	TetrisBlock cur;
	GameObject* curm;

	CameraAngle camera;

	GLuint fbDepth1, texDepth1, fbDepth2, texDepth2;
public:
	Game(): scene(
		glm::perspective( glm::radians( 90.0f ), 1.0f, 1.0f, 50.0f ),
		glm::lookAt(
			glm::vec3( -1.0f, -1.0f, -15.0f ),
			glm::vec3( -1.0f, -1.0f, -5.0f ),
			glm::vec3( 0.0f, 1.0f, 0.0f )
		)
	), light1(
		glm::ortho<float>( -20, 20, -20, 20, 1, 50 ),
		glm::lookAt(
			glm::vec3( 5, 10, -15 ),
			glm::vec3( -1, 0, 10 ),
			glm::vec3( 0, 1, 0 )
		)
	), light2(
		glm::ortho<float>( -20, 20, -20, 20, 1, 50 ),
		glm::lookAt(
			glm::vec3( 3, 13, -15 ),
			glm::vec3( -1, 0, 10 ),
			glm::vec3( 0, 1, 0 )
		)
	) {
		srand( time( NULL ) );

		time1 = time2 = 0;
		board = new bool[20 * 10 * 10];
		for ( int i = 0; i < 20 * 10 * 10; ++i ) {
			board[i] = false;
		}

		this->camera = CameraAngle::FRONT;
	}

	void init() {
		Engine::init( 1000, 1000, "Tetris" );
		glEnable( GL_DEPTH_TEST );
		glEnable( GL_TEXTURE_2D );

		this->sp = new ShaderProgram( "v_constant.glsl", NULL, "f_constant.glsl" );
		this->spCube = new ShaderProgram( "v_cube.glsl", NULL, "f_cube.glsl" );
		this->spDepth = new ShaderProgram( "v_depth.glsl", NULL, "f_depth.glsl" );

		this->cube = new Cube();
		this->cube2 = new Cube2();
		this->cubewire = new CubeWire();
		this->cubeWhite = new Cube( { 1.0f, 1.0f, 1.0f } );

		this->fbDepth1 = this->texDepth1 = 0;
		glGenFramebuffers(1, &this->fbDepth1);
		glBindFramebuffer(GL_FRAMEBUFFER, fbDepth1);
		glGenTextures(1, &this->texDepth1);
		glBindTexture(GL_TEXTURE_2D, texDepth1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1000, 1000, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindFramebuffer(GL_FRAMEBUFFER, fbDepth1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texDepth1, 0);
    	glBindFramebuffer(GL_FRAMEBUFFER, 0);

		this->fbDepth2 = this->texDepth2 = 0;
		glGenFramebuffers(1, &this->fbDepth2);
		glBindFramebuffer(GL_FRAMEBUFFER, fbDepth2);
		glGenTextures(1, &this->texDepth2);
		glBindTexture(GL_TEXTURE_2D, texDepth2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1000, 1000, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindFramebuffer(GL_FRAMEBUFFER, fbDepth2);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texDepth2, 0);
    	glBindFramebuffer(GL_FRAMEBUFFER, 0);

		this->blocks = ( new GameObject( NULL, {}, this->spCube ) )->scale( .6f );

		this->curm = ( new GameObject( NULL, {} ) )->scale( .6f );
		this->spawnBlock();

		this->bg = ( new GameObject( NULL, {
			/*( new GameObject( this->cubewire ) )
				->translate( { -1, -1, 9 } )
				->scale( { 10, 20, 10 } ),*/

			( new GameObject( this->cube ) )
				->translate( { -1, -21, 9 } )
				->scale( { 10.1f, .1f, 10.1f } )
				->rotate( 180, { 1.0f, 0, 0 } ),

			( new GameObject( this->cube ) )
				->translate( { -1, -1, 19 } )
				->scale( { 10.1f, 20.1f, .1f } )
				->rotate( 180, { 1.0f, 0, 0 } ),

			( new GameObject( this->cube ) )
				->translate( { -11, -1, -1 } )
				->scale( { .1f, 20, .1f } )
				->rotate( 180, { 1.0f, 0, 0 } ),

			( new GameObject( this->cube ) )
				->translate( { -11, 19, 9 } )
				->scale( { .1f, .1f, 10.1f } )
				->rotate( 180, { 1.0f, 0, 0 } ),

			( new GameObject( this->cube ) )
				->translate( { 9, -1, -1 } )
				->scale( { .1f, 20, .1f } )
				->rotate( 180, { 1.0f, 0, 0 } ),

			( new GameObject( this->cube ) )
				->translate( { 9, 19, 9 } )
				->scale( { .1f, .1f, 10.1f } )
				->rotate( 180, { 1.0f, 0, 0 } ),

			( new GameObject( this->cube ) )
				->translate( { -1, 19, -1 } )
				->scale( { 10.1f, .1f, .1f } )
				->rotate( 180, { 1.0f, 0, 0 } ),

			( new GameObject( this->cubeWhite ) )
				->translate( { -1, -20.9, 18.9 } )
				->scale( { 10.1f, .1f, .1f } ),

			( new GameObject( this->cubeWhite ) )
				->translate( { 8.9, -20.9, 9 } )
				->scale( { .1, .1, 10.1f } ),

			( new GameObject( this->cubeWhite ) )
				->translate( { -10.9, -20.9, 9 } )
				->scale( { .1, .1, 10.1f } ),

			( new GameObject( this->cubeWhite ) )
				->translate( { -1, -20.9, -0.9 } )
				->scale( { 10.1f, .1, .1 } )
		}, this->sp ) )->scale( .6f );

		for ( int i = 19; i > -20; i -= 2 ) {
			this->bg->subobjects.push_back(
				( new GameObject( this->cubeWhite ) )
					->translate( { -1, i, 18.9 } )
					->scale( { 10.1f, .1f, .1f } )
			);
		}

		for ( int i = 9; i >= -11; i -= 2 ) {
			this->bg->subobjects.push_back(
				( new GameObject( this->cubeWhite ) )
					->translate( { i, -1, 18.9 } )
					->scale( { .1f, 20, .1 } )
			);
		}

		for ( int i = 17; i > -1; i -= 2 ) {
			this->bg->subobjects.push_back(
				( new GameObject( this->cubeWhite ) )
					->translate( { -1, -20.9, i } )
					->scale( { 10.1f, .1f, .1f } )
			);
		}

		for ( int i = 7; i > -11; i -= 2 ) {
			this->bg->subobjects.push_back(
				( new GameObject( this->cubeWhite ) )
					->translate( { i, -20.9, 9 } )
					->scale( { .1f, .1f, 10.1f } )
			);
		}

		this->scene.objects.push_back( this->blocks );
		this->scene.objects.push_back( this->bg );
		this->scene.objects.push_back( this->curm );
		this->light1.objects = this->scene.objects;
		this->light2.objects = this->scene.objects;
	}

	~Game() {
		delete this->board;

		for ( auto &obj : this->scene.objects ) {
			for ( auto &subobj : obj->subobjects ) {
				delete subobj;
			}
			delete obj;
		}

		delete this->cube2;
		delete this->cube;
		delete this->sp;
	}

	void createBlock( std::vector<Vec3Int> subblocks ) {
		/*if ( this->curm ) {
			delete this->curm;
		}*/

		cur.blocks.clear();

		int mx = 0, my = 0, mz = 0;
		for ( auto &sb : subblocks ) {
			mx = std::max( mx, sb.x );
			my = std::max( my, sb.y );
			mz = std::max( mz, sb.z );
		}

		int r = rand() % 0b111111;
		for ( auto &sb : subblocks ) {
			if ( r & 0b000001 ) {
				sb.x = mx - sb.x;
			}

			if ( r & 0b000010 ) {
				sb.y = my - sb.y;
			}

			if ( r & 0b000100 ) {
				sb.z = mz - sb.z;
			}
		}

		if ( r & 0b001000 ) {
			for ( auto &sb : subblocks ) {
				std::swap( sb.x, sb.y );
			}

			std::swap( mx, my );
		}

		if ( r & 0b010000 ) {
			for ( auto &sb : subblocks ) {
				std::swap( sb.x, sb.z );
			}

			std::swap( mx, mz );
		}

		if ( r & 0b100000 ) {
			for ( auto &sb : subblocks ) {
				std::swap( sb.y, sb.z );
			}

			std::swap( my, mz );
		}

		Vec3Int offset = { rand() % ( 10 - mx ), 20, rand() % ( 10 - mz ) };

		for ( auto &sb : subblocks ) {
			this->cur.blocks.push_back( { sb.x + offset.x, sb.y + offset.y, sb.z + offset.z } );
		}

		this->recreateBlockModel();
	}

	void recreateAllBlocks() {
		for ( auto &obj : this->blocks->subobjects ) {
			delete obj;
		}

		this->blocks->subobjects.clear();
		TetrisBlock bkp;

		std::swap( bkp, this->cur );
		std::swap( this->curm, this->blocks );
		for ( int y = 0; y < 20; y++ ) {
			for ( int x = 0; x < 10; x++ ) {
				for ( int z = 0; z < 10; z++ ) {
					if ( this->board[x * 200 + y * 10 + z] ) {
						cur.blocks.push_back( { x, y, z } );
					}
				}
			}
		}

		this->recreateBlockModel();
		std::swap( this->curm, this->blocks );
		std::swap( bkp, this->cur );
	}

	void recreateBlockModel() {
		for ( auto &obj : this->curm->subobjects ) {
			delete obj;
		}

		this->curm->subobjects.clear();

		for ( auto &sb : cur.blocks ) {
			this->curm->subobjects.push_back(
				( new GameObject( cube2, {}, this->spCube ) )
					->translate( { ( sb.x - 5 ) * 2, ( sb.y * 2 ) - 20, sb.z * 2 } )
			);
		}
	}

	void spawnBlock() {
		int x = rand() % 7;

		switch ( rand() % 5 ) {
		case 0:
			// x x x
			//   x
			this->createBlock( {
				{ 0, 1, 0 }, { 1, 1, 0 }, { 2, 1, 0 },
				             { 1, 0, 0 }
			} );
			break;

		case 1:
			// x x
			// x x
			this->createBlock( {
				{ 0, 4, 0 }, { 1, 4, 0 }, { 2, 4, 0 }, { 3, 4, 0 }, { 4, 4, 0 },
				{ 0, 3, 0 }, { 1, 3, 0 }, { 2, 3, 0 }, { 3, 3, 0 }, { 4, 3, 0 },
				{ 0, 2, 0 }, { 1, 2, 0 }, { 2, 2, 0 }, { 3, 2, 0 }, { 4, 2, 0 },
				{ 0, 1, 0 }, { 1, 1, 0 }, { 2, 1, 0 }, { 3, 1, 0 }, { 4, 1, 0 },
				{ 0, 0, 0 }, { 1, 0, 0 }, { 2, 0, 0 }, { 3, 0, 0 }, { 4, 0, 0 },
			} );
			break;

		case 2:
			// x x x
			//     x
			this->createBlock( {
				{ 0, 1, 0 }, { 1, 1, 0 }, { 2, 1, 0 },
				                          { 2, 0, 0 },
			} );
			break;

		case 3:
			// x x x x
			this->createBlock( {
				{ 0, 0, 0 }, { 1, 0, 0 }, { 2, 0, 0 }, { 3, 0, 0 },
			} );
			break;

		case 4:
			// x x
			//   x x
			this->createBlock( {
				{ 0, 1, 0 }, { 1, 1, 0 },
				             { 1, 0, 0 }, { 2, 0, 0 }
			} );
			break;
		}
	}

	void update( float elapsedTime ) {
		time1 += speedUp ? FACTOR_SPEED_UP * elapsedTime : elapsedTime;
		while ( time2 <= 0 && time1 < TIME_STEP && !actions.empty() ) {
			this->recreateBlockModel();
			Action action = actions.top();
			actions.pop();

			if (
				action == Action::CAMERA_LEFT
				|| action == Action::CAMERA_FWD
				|| action == Action::CAMERA_RIGHT
				|| action == Action::CAMERA_BACK
			) {
				if ( camera == CameraAngle::LEFT ) {
					this->scene.V = glm::translate(
						glm::rotate(
							glm::translate( this->scene.V, { 0, 0, 5 } ),
							glm::radians( -90.0f ),
							glm::vec3( .0f, 1.0f, .0f )
						),
						{ 0, 0, -5 }
					);
				} else if ( camera == CameraAngle::RIGHT ) {
					this->scene.V = glm::translate(
						glm::rotate(
							glm::translate( this->scene.V, { 0, 0, 5 } ),
							glm::radians( 90.0f ),
							glm::vec3( .0f, 1.0f, .0f )
						),
						{ 0, 0, -5 }
					);
				} else if ( camera == CameraAngle::TOP ) {
					this->scene.V = glm::translate(
					glm::rotate(
						glm::translate( this->scene.V, { 0, 0, 5 } ),
						glm::radians( 90.0f ),
						glm::vec3( 1.0f, .0f, .0f )
					),
						{ 0, 0, -5 }
					);
				}

				if (
					action == Action::CAMERA_BACK
					|| ( camera == CameraAngle::LEFT && action == Action::CAMERA_RIGHT )
					|| ( camera == CameraAngle::RIGHT && action == Action::CAMERA_LEFT )
				) {
					camera = CameraAngle::FRONT;
				} else if ( action == Action::CAMERA_LEFT ) {
					printf( "Rotate Left\n" );
					this->scene.V = glm::translate(
						glm::rotate(
							glm::translate( this->scene.V, { 0, 0, 5 } ),
							glm::radians( 90.0f ),
							glm::vec3( .0f, 1.0f, .0f )
						),
						{ 0, 0, -5 }
					);

					camera = CameraAngle::LEFT;
				} else if ( action == Action::CAMERA_RIGHT ) {
					printf( "Rotate Right\n" );
					this->scene.V = glm::translate(
						glm::rotate(
							glm::translate( this->scene.V, { 0, 0, 5 } ),
							glm::radians( -90.0f ),
							glm::vec3( .0f, 1.0f, .0f )
						),
						{ 0, 0, -5 }
					);

					camera = CameraAngle::RIGHT;
				} else if ( action == Action::CAMERA_FWD ) {
					printf( "Rotate Forward\n" );
					this->scene.V = glm::translate(
						glm::rotate(
							glm::translate( this->scene.V, { 0, 0, 5 } ),
							glm::radians( -90.0f ),
							glm::vec3( 1.0f, .0f, .0f )
						),
						{ 0, 0, -5 }
					);

					camera = CameraAngle::TOP;
				}
			} else if (
				( action == Action::BLOCK_LEFT && ( camera == CameraAngle::FRONT || camera == CameraAngle::TOP ) )
				|| ( action == Action::BLOCK_BACK && camera == CameraAngle::LEFT )
				|| ( action == Action::BLOCK_FWD && camera == CameraAngle::RIGHT )
			) {
				printf( "Left\n" );
				if ( cur.checkX( this->board, 1 ) ) {
					curA = Animation::BLOCK_LEFT;
					time2 += TIME_ANIM_FALL;
				}
			} else if (
				( action == Action::BLOCK_RIGHT && ( camera == CameraAngle::FRONT || camera == CameraAngle::TOP ) )
				|| ( action == Action::BLOCK_BACK && camera == CameraAngle::RIGHT )
				|| ( action == Action::BLOCK_FWD && camera == CameraAngle::LEFT )
			) {
				if ( cur.checkX( this->board, -1 ) ) {
					curA = Animation::BLOCK_RIGHT;
					time2 += TIME_ANIM_FALL;
				}
			} else if (
				( action == Action::BLOCK_FWD && ( camera == CameraAngle::FRONT || camera == CameraAngle::TOP ) )
				|| ( action == Action::BLOCK_LEFT && ( camera == CameraAngle::LEFT ) )
				|| ( action == Action::BLOCK_RIGHT && ( camera == CameraAngle::RIGHT ) )
			) {
				printf( "Forward\n" );
				if ( cur.checkZ( this->board, 1 ) ) {
					curA = Animation::BLOCK_FWD;
					time2 += TIME_ANIM_FALL;
				}
			} else if (
				( action == Action::BLOCK_BACK && ( camera == CameraAngle::FRONT || camera == CameraAngle::TOP ) )
				|| ( action == Action::BLOCK_RIGHT && ( camera == CameraAngle::LEFT ) )
				|| ( action == Action::BLOCK_LEFT && ( camera == CameraAngle::RIGHT ) )
			) {
				printf( "Back\n" );
				if ( cur.checkZ( this->board, -1 ) ) {
					curA = Animation::BLOCK_BACK;
					time2 += TIME_ANIM_FALL;
				}
			} else if (
				(
					( action == Action::ROTATE_LEFT || action == Action::ROTATE_RIGHT )
					&& ( camera == CameraAngle::FRONT || camera == CameraAngle::TOP )
				)
				|| (
					( action == Action::ROTATE_FWD || action == Action::ROTATE_BACK )
					&& ( camera == CameraAngle::LEFT || camera == CameraAngle::RIGHT )
				)
			) {
				int dir = camera == CameraAngle::LEFT
					? action == Action::ROTATE_FWD ? 1 : -1
					: camera == CameraAngle::RIGHT
						? action == Action::ROTATE_FWD ? -1 : 1
						: action == Action::ROTATE_LEFT ? -1 : 1;

				if ( cur.rotX( this->board, dir ) ) {
					this->recreateBlockModel();
				}
			} else if (
				(
					( action == Action::ROTATE_FWD || action == Action::ROTATE_BACK )
					&& ( camera == CameraAngle::FRONT || camera == CameraAngle::TOP )
				)
				|| (
					( action == Action::ROTATE_LEFT || action == Action::ROTATE_RIGHT )
					&& ( camera == CameraAngle::LEFT || camera == CameraAngle::RIGHT )
				)
			) {
				int dir = camera == CameraAngle::LEFT
					? action == Action::ROTATE_LEFT ? 1 : -1
					: camera == CameraAngle::RIGHT
						? action == Action::ROTATE_LEFT ? -1 : 1
						: action == Action::ROTATE_FWD ? -1 : 1;

				if ( cur.rotZ( this->board, dir ) ) {
					this->recreateBlockModel();
				}
			}

			action = Action::NONE;
		}

		this->recreateAllBlocks();

		if ( time2 <= 0 && time1 >= TIME_STEP ) {
			this->recreateBlockModel();
			time1 = 0;

			if ( cur.checkDown( this->board ) ) {
				curA = Animation::BLOCK_FALL;
				time2 += TIME_ANIM_FALL;
			} else if ( !cur.isAboveBoard() ) {
				for ( auto &block : this->curm->subobjects ) {
					this->blocks->subobjects.push_back( block );
				}

				this->curm->subobjects.clear();
				cur.lock( this->board );

				for ( int y = 0; y < 20; y++ ) {
					bool res = true;
					for ( int x = 0; res && x < 10; x++ ) {
						for ( int z = 0; res && z < 10; z++ ) {
							res = this->board[x * 200 + y * 10 + z];
						}
					}

					if ( res ) {
						printf( "PLANE CLEAR! (y = %i)\n", y );

						for ( int y2 = y; y2 < 19; y2++ ) {
							for ( int x = 0; x < 10; x++ ) {
								for ( int z = 0; z < 10; z++ ) {
									this->board[x * 200 + y2 * 10 + z] = this->board[x * 200 + ( y2 + 1 ) * 10 + z];
								}
							}
						}

						this->recreateAllBlocks();
						--y;
					}
				}

				this->spawnBlock();
			} else {
				printf( "OVER!\n" );

				for ( int i = 0; i < 20 * 10 * 10; ++i ) {
					board[i] = false;
				}

				this->recreateAllBlocks();
			}
		} else if ( time2 > 0 ) {
			if ( curA == Animation::BLOCK_FALL ) {
				float offset = ( std::min( elapsedTime, time2 ) / TIME_ANIM_FALL ) * -2;
				for ( auto &block : curm->subobjects ) {
					block->translate( { 0, offset, 0 } );
				}
			} else if ( curA == Animation::BLOCK_LEFT || curA == Animation::BLOCK_RIGHT ) {
				float offset = ( std::min( elapsedTime, time2 ) / TIME_ANIM_FALL )
					* ( curA == Animation::BLOCK_RIGHT ? -2 : 2 );
				for ( auto &block : curm->subobjects ) {
					block->translate( { offset, 0, 0 } );
				}
			} else if ( curA == Animation::BLOCK_FWD || curA == Animation::BLOCK_BACK ) {
				float offset = ( std::min( elapsedTime, time2 ) / TIME_ANIM_FALL )
					* ( curA == Animation::BLOCK_BACK ? -2 : 2 );
				for ( auto &block : curm->subobjects ) {
					block->translate( { 0, 0, offset } );
				}
			}

			time2 -= elapsedTime;
		}

		glm::mat4 biasMatrix(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
		);

		glm::mat4 lightPV1 = biasMatrix * this->light1.P * this->light1.V;
		glm::mat4 lightPV2 = biasMatrix * this->light2.P * this->light2.V;

		glBindFramebuffer(GL_FRAMEBUFFER, this->fbDepth1);
		glViewport(0, 0, 1000, 1000);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->spDepth->use();
		this->light1.draw(this->spDepth);

		glViewport(0, 0, 1000, 1000);
		glBindFramebuffer(GL_FRAMEBUFFER, this->fbDepth2);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->spDepth->use();
		this->light2.draw(this->spDepth);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 1000, 1000);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->sp->use();
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, this->texDepth1);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, this->texDepth2);
		glUniformMatrix4fv(this->sp->u("LightPV1"), 1, GL_FALSE, &lightPV1[0][0]);
		glUniform1i(sp->u("sMap1"), 4);
		glUniformMatrix4fv(this->sp->u("LightPV2"), 1, GL_FALSE, &lightPV2[0][0]);
		glUniform1i(sp->u("sMap2"), 5);

		this->spCube->use();
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, this->texDepth1);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, this->texDepth2);
		glUniformMatrix4fv(this->spCube->u("LightPV1"), 1, GL_FALSE, &lightPV1[0][0]);
		glUniform1i(spCube->u("sMap1"), 4);
		glUniformMatrix4fv(this->spCube->u("LightPV2"), 1, GL_FALSE, &lightPV2[0][0]);
		glUniform1i(spCube->u("sMap2"), 5);

		this->draw();
	}

	void draw( ShaderProgram* sp = nullptr ) {
		if ( !sp ) {
			this->spCube->use();
			this->scene.setPV( this->spCube );
			this->sp->use();
			this->scene.setPV( this->sp );
		} else {
			sp->use();
		}

		scene.draw( sp );
	}
};

void error_callback( int error, const char* description ) {
	fputs( description, stderr );
}

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
	if ( action == GLFW_PRESS ) {
		switch ( key ) {
		case GLFW_KEY_LEFT:
			actions.push( mods & GLFW_MOD_SHIFT ? Action::ROTATE_LEFT : Action::BLOCK_LEFT );
			break;
		case GLFW_KEY_RIGHT:
			actions.push( mods & GLFW_MOD_SHIFT ? Action::ROTATE_RIGHT : Action::BLOCK_RIGHT );
			break;
		case GLFW_KEY_UP:
			actions.push( mods & GLFW_MOD_SHIFT ? Action::ROTATE_FWD : Action::BLOCK_FWD );
			break;
		case GLFW_KEY_DOWN:
			actions.push( mods & GLFW_MOD_SHIFT ? Action::ROTATE_BACK : Action::BLOCK_BACK );
			break;
		case GLFW_KEY_A:
			actions.push( Action::CAMERA_LEFT );
			break;
		case GLFW_KEY_D:
			actions.push( Action::CAMERA_RIGHT );
			break;
		case GLFW_KEY_W:
			actions.push( Action::CAMERA_FWD );
			break;
		case GLFW_KEY_S:
			actions.push( Action::CAMERA_BACK );
			break;
		case GLFW_KEY_SPACE:
			speedUp = true;
			break;
		}
	} else if ( action == GLFW_RELEASE ) {
		switch ( key ) {
		case GLFW_KEY_SPACE:
			speedUp = false;
			break;
		}
	}
}

int main() {
	glfwSetErrorCallback( error_callback );

	Game game;
	game.init();

	glfwSetKeyCallback( game.window, key_callback );

	game.run();
	game.destroy();

	return 0;
}
