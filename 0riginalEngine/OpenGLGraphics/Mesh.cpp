#include "Mesh.h"

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Mesh::initialiseQuad()
{
	assert(vao == 0);

	glm::vec3 verticies[] =
	{
		glm::vec3(-1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 0.0f)
	};
	
	int index_buffer[]{0,2,1,1,2,3};
	indexCount = 6;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, indexCount * sizeof(glm::vec3), &verticies[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(int), &index_buffer[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Mesh::initialiseCube()
{
	assert(vao == 0);

	glm::vec3 verticies[] =
	{
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f,-1.0f, 1.0f),
			glm::vec3(1.0f, 1.0f,-1.0f), 
			glm::vec3(1.0f,-1.0f,-1.0f),
			glm::vec3(-1.0f,-1.0f,-1.0f),
			glm::vec3(-1.0f,-1.0f, 1.0f),
			glm::vec3(-1.0f, 1.0f,-1.0f),
			glm::vec3(-1.0f, 1.0f, 1.0f)
	};

	int index_buffer[]
	{	0,2,1,1,2,3,
		0,1,5,0,5,7,
		5,4,6,6,7,5,
		3,6,4,3,2,6,
		6,2,0,0,7,6,
		1,4,5,4,1,3 
	};
	indexCount = 36;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, indexCount * sizeof(glm::vec3), &verticies[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(int), &index_buffer[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Mesh::draw()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	if (ibo)
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, 3 * indexCount);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
