#pragma once

#include "glm.hpp"
#include "ext.hpp"
#include "../glcore/gl_core_4_5.h"
#include "glfw3.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <string>
struct MeshData
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

};

class OBJLoader
{
public:

	virtual void draw();
	MeshData loadOBJ(const char* path);
	std::vector<std::vector<const char*>> triangulate(std::vector<const char*> data);
	std::vector<const char*> splitString(const char* data, const char* character = " ", bool ignoreFirst = true);

protected:
	unsigned int vertex_count;
	unsigned int vao, vbo, ibo;
};

