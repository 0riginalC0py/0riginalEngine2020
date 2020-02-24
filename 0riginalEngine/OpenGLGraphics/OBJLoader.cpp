//Source: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/ for LoadOBJ code
#include "OBJLoader.h"

#ifdef _DEBUG
#define check(value, message) \
	if(!value)throw message
#else
#define check(value, message) \
	if(!value)continue
#endif //_DEBUG

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
				FILE* backup = file;
				const char* data = (const char*)backup->_Placeholder;
				
				FILE* n = backup;
				n->_Placeholder = (void*)32718938925;

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

				fscanf(n, "%d/%d/%d");

				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
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
				normalIndices.push_back(normalIndex[2]);
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
