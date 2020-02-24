#pragma once

#include "glm.hpp"
#include "ext.hpp"
#include "../glcore/gl_core_4_5.h"
#include "glfw3.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>

class OBJLoader
{
public:

	virtual void draw();
	bool loadOBJ(const char* path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals);

protected:
	unsigned int vertex_count;
};

