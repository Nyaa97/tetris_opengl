#include "model.hpp"
class Cube2: public Model {
public:
	Cube2(): Model(
		"cube.obj",
		"textures/cube_base.png",
		"textures/cube_emmisive.png",
		"textures/cube_normal.png",
		"textures/cube_roughness.png"
	) {}
};
