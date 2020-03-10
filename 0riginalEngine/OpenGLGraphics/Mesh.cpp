#include "Mesh.h"

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Mesh::initialiseTriangle(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3)
{
	assert(vao == 0);

	glm::vec3 verticies[] =
	{
		point1,
		point2,
		point3
	};

	int index_buffer[]{ 0,2,1 };
	indexCount = 3;

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

void Mesh::initialisePlane(unsigned int size)
{
	assert(vao == 0);

	int vertCount = size * size;
	int half = size / 2;
	glm::vec3 temp_vec;
	
	std::vector<glm::vec3> vertices;

	for (int width = 0; width < size; width++)
	{
		for (int height = 0; height < size; height++)
		{
			temp_vec.x = height - half;
			temp_vec.z = width - half;
			temp_vec.y = 0;
			vertices.push_back(temp_vec);
		}
	}

	indexCount = (size - 1) * (size - 1) * 2 * 3;

	std::vector<int> indicies;
	int runner = 0;
	for (int row = 0; row < size - 1; row++)
	{
		for (int col = 0; col < size - 1; col++)
		{
			indicies.push_back(size * row + col);
			indicies.push_back(size * row + col + size);
			indicies.push_back(size * row + col + size + 1);

			indicies.push_back(size * row + col);
			indicies.push_back(size * row + col + size + 1);
			indicies.push_back(size * row + col + 1);

		}
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, indexCount * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(int), &indicies[0], GL_STATIC_DRAW);

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
