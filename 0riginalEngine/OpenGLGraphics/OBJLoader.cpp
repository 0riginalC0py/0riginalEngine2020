//Source: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/ for LoadOBJ code
#include "OBJLoader.h"

#ifdef _DEBUG
#define check(value, message) \
	if(!value)throw message
#else
#define check(value, message) \
	if(!value)continue
#endif //_DEBUG

FILE* GetFile(FILE * base, const char* data) {
	FILE* f = new FILE(*base);
	f->_Placeholder = (void*)data;
	return f;
}

#pragma warning(disable : 4996)

void OBJLoader::draw()
{
	glDrawArrays(GL_TRIANGLES, 0, 3 * vertex_count);
}

bool OBJLoader::loadOBJ(const char* path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE* file = fopen(path, "r");
	if (!file)
	{
		printf("OBJ File failed to open!");
		return false;
	}

	while (file)
	{
		char lineHeader[3];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;
		else
		{
			if (strcmp(lineHeader, "v") == 0)
			{
				glm::vec3 vertex;
				check(fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z), "Couldnt Load Vertex Data");
				temp_vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0)
			{
				glm::vec2 uv;
				check(fscanf(file, "%f %f\n", &uv.x, &uv.y), "Couldnt Load UV Data");
				temp_uvs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0)
			{
				glm::vec3 normal;
				check(fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z), "Couldnt Load Normal Data");
				temp_normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0)
			{
				FILE* backup = new FILE(*file);

				const char* data = (const char*)backup->_Placeholder;
				
				auto triangles = triangulate(data);

				for (auto triangle : triangles) {
					for (FILE* vertex : triangle) {
						int* x = new int;
						int* y = new int;
						int* z = new int;
						//TODO String Tokenizer for F string of /
						fscanf(vertex, "%i/%i/%i", x, y, z);
						printf("Loaded Triangle with Indicies: (%i, %i, %i)", *x, *y, *z);
						delete x;
						delete y;
						delete z;
					}
				}


				//std::string vertex1, vertex2, vertex3;

				//fscanf(, "%d/%d/%d", vertex1, vertex2, vertex3);

				
				/*
					Try to Split into Triangles.

					for (int i = 2; i < vertCount; i++)
					{
						DoTriangle(verts[0], verts[i], verts[i-1]);
					}

					Start
					does triangle have 3 arguments?
					yes?
						add all 3 into the list
					no?
					does it have 2 arguments?
					yes?
						does it have 2 slashes?
						yes?
							add Vertex and Normal into the list
						no?
							add Vertex and UV into the List
					no?
						push index into array
					End.

				*/

				/*unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9)
				{
					printf("Your file is too strong for my parser : ( Try exporting with other options )\n");
					return false;
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);*/
			}

			for (unsigned int i = 0; i < vertexIndices.size(); i++)
			{
				unsigned int vertexIndex = vertexIndices[i];
				glm::vec3 vertex = temp_vertices[vertexIndex - 1];
				out_vertices.push_back(vertex);
			}
			for (unsigned int i = 0; i < uvIndices.size(); i++)
			{
				unsigned int uvIndex = uvIndices[i];
				glm::vec2 uv = temp_uvs[uvIndex - 1];
				out_uvs.push_back(uv);
			}
			for (unsigned int i = 0; i < normalIndices.size(); i++)
			{
				unsigned int normalIndex = normalIndices[i];
				glm::vec3 normal = temp_normals[normalIndex - 1];
				out_normals.push_back(normal);
			}
		}
	}
	
	return true;
}

std::vector<std::vector<std::FILE*>> OBJLoader::triangulate(const char* data)
{
	FILE* f = new FILE();
	std::vector<std::vector<std::FILE*>> verticies;

	std::vector<const char*> parts;

	int index = 0;
	while (data[index] != '\n' && data[index] != 0) {

		if (data[index] == ' ') {
			parts.push_back(data + index + 1);
		}

		index++;
	}

	for (int i = 2; i < parts.size(); i++)
	{
		std::vector<FILE*> triangle;
		triangle.push_back(GetFile(f, parts[0]));
		triangle.push_back(GetFile(f, parts[i]));
		triangle.push_back(GetFile(f, parts[i - 1]));
		verticies.push_back(triangle);
	}

	return verticies;
}