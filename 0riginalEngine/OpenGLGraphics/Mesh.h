#pragma once
#include "glm.hpp"
#include "../glcore/gl_core_4_5.h"
#include <vector>

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
	
	void initialiseTriangle(glm::vec3 point1 = glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3 point2 = glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3 point3 = glm::vec3(-1.0f, -1.0f, 0.0f));
	void initialiseQuad();
	void initialiseCube();
	void initialisePlane(unsigned int size = 10);
	virtual void draw();

protected:

	unsigned int indexCount;
	unsigned int vao, vbo, ibo;
};

