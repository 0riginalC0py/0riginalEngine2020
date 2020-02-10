#pragma once
#include "glm.hpp"
#include "../glcore/gl_core_4_5.h"
class Mesh
{
public:

	Mesh() : indexCount(0), vao(0), vbo(0), ibo(0) {}
	virtual ~Mesh();

	struct Vertex { 
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
	};
	
	void initialiseQuad();
	void initialiseCube();
	virtual void draw();

protected:

	unsigned int indexCount;
	unsigned int vao, vbo, ibo;
};

