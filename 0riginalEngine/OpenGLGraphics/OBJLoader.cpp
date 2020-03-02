//Source: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/ for LoadOBJ code
#include "OBJLoader.h"
#include <iostream>
#include <string>

#define vector std::vector
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

static bool startsWith(const char* str, const char* pre) {
	return !strncmp(str, pre, strlen(pre));
}

static int Indexof(const char* string, const char* pattern) {
	int patternLength = strlen(pattern);
	int index = 0;

	int output = -1;
	do {

		if (!memcmp(string + index, pattern, sizeof(char) * patternLength))
			return index;

		++index;
	} while (string[index] != '\n' && string[index] != 0);
	return output;
}

std::string ReadText(const char* filepath) {
	std::ifstream file(filepath, std::ifstream::in);
	std::stringstream fileData;


	//Vertex Shader File Loading
	if (file.is_open() && file.good()) {
		fileData << file.rdbuf();

		file.close();

		return fileData.str();
	}
	else
		throw "Could not Load File";
};

#pragma warning(disable : 4996);

void OBJLoader::draw()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	if (ibo)
		glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, 3 * vertex_count);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

MeshData OBJLoader::loadOBJ(const char* path)
{
	vector<unsigned int> vertexIndices;
	vector<glm::vec3> temp_vertices;
	vector<glm::vec2> temp_uvs;
	vector<glm::vec3> temp_normals;

	int vertIndex;
	int uvIndex;
	int normIndex;

	vector<glm::vec3> vertex_buffer;
	vector<glm::vec2> uv_buffer;
	vector<glm::vec3> normal_buffer;

	//FILE* file = fopen(path, "r");

	//if (!file)
	//{
	//	printf("OBJ File failed to open!");
	//	return false;
	//}

	//while (file) {
	//	char lineHeader[20];
	//	int res = fscanf(file, "%s", lineHeader);

	//	if (res == EOF)break;
	//}

	std::string file = ReadText(path);
	const char* data = file.c_str();



	vector<const char*> lines = splitString(data, '\n', false);


	for (const char* line : lines) 
	{
		if (line[0] == '#' || line[0] == '\n')
			continue;

		if (startsWith(line, "vt"))
		{
			vector<const char*> args = splitString(line);
			temp_uvs.push_back(glm::vec2(std::stof(args[0]), std::stof(args[1])));
			continue;
		}
		else
		if (startsWith(line, "vn"))
		{
			vector<const char*> args = splitString(line);
			temp_normals.push_back(glm::vec3(std::stof(args[0]), std::stof(args[1]), std::stof(args[2])));
			continue;
		}
		else
		if (startsWith(line, "v")) 
		{
			vector<const char*> args = splitString(line);
			check(args.size() == 3, "Position has the Wrong amount of Verticies");
			temp_vertices.push_back(glm::vec3(std::stof(args[0]), std::stof(args[1]), std::stof(args[2])));
			continue;
		}

		

		if (startsWith(line, "f"))
		{
			vector<const char*> args = splitString(line);
			vector<vector<const char*>> tris = triangulate(args);
			for (vector<const char*>& tri : tris)
			{
				for (const char* point : tri)
				{
					bool isDoubleSlash = false;
					vector<const char*> indicies;
					if (Indexof(point, "//") == -1)
						indicies = splitString(point, '/', false);
					else 
					{
						indicies = splitString(point, '//', false);
						isDoubleSlash = true;
					}

					switch (indicies.size())
					{
					case 1:
						vertexIndices.push_back(std::stoi(indicies[0]));
						break;
					case 2:
						if (isDoubleSlash)
						{
							vertIndex = std::stoi(indicies[0]);
							normIndex = std::stoi(indicies[1]);

							vertexIndices.push_back(vertexIndices.size());
							vertex_buffer.push_back(temp_vertices[vertIndex - 1]);
							normal_buffer.push_back(temp_normals[normIndex - 1]);
						}
						else
						{
							vertIndex = std::stoi(indicies[0]);
							uvIndex = std::stoi(indicies[1]);

							vertexIndices.push_back(vertexIndices.size());
							vertex_buffer.push_back(temp_vertices[vertIndex - 1]);
							uv_buffer.push_back(temp_uvs[uvIndex - 1]);
						}
						break;
					case 3:

						vertIndex = std::stoi(indicies[0]);
						uvIndex = std::stoi(indicies[1]);
						normIndex = std::stoi(indicies[2]);

						vertexIndices.push_back(vertexIndices.size());
						vertex_buffer.push_back(temp_vertices[vertIndex - 1]);
						normal_buffer.push_back(temp_normals[normIndex - 1]);
						uv_buffer.push_back(temp_uvs[uvIndex - 1]);
						break;
					default:
						std::cout << "Something's Wrong" << std::endl;
						break;
					}
				}
			}
		}
	}
	if (vertex_buffer.size() == 0)
		vertex_buffer = temp_vertices;

	MeshData mesh;
	mesh.vertices = vertex_buffer;
	mesh.uvs = uv_buffer;
	mesh.normals = normal_buffer;
	mesh.indices = vertexIndices;

	vertex_count = mesh.vertices.size();

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(glm::vec3), mesh.vertices.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(int), mesh.indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	return mesh;

	//FILE* file = fopen(path, "r");
	//if (!file)
	//{
	//	printf("OBJ File failed to open!");
	//	return false;
	//}

	//while (file)
	//{


	//	char lineHeader[20];
	//	int res = fscanf(file, "%s", lineHeader);

	//	if (res == EOF)break;

	//	if (lineHeader[0] == '#') {
	//		//Read until Newline;
	//		char comment[500];
	//		fgets(comment, 500, file);
	//		fscanf(file, "\n");
	//		continue;
	//	}

	//	if (strcmp(lineHeader, "g") == 0) {
	//		char ObjectName[100];
	//		check(fscanf(file, "%s", ObjectName), "Couldnt Load Object Name");
	//	}else if (strcmp(lineHeader, "mtllib") == 0)
	//	{
	//		char LibaryName[100];
	//		check(fscanf(file, "%s.mtl", LibaryName), "Couldnt Load Material Libary");
	//	}else if (strcmp(lineHeader, "v") == 0)
	//	{
	//		glm::vec3 vertex;
	//		check(fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z), "Couldnt Load Vertex Data");
	//		temp_vertices.push_back(vertex);
	//	}
	//	else if (strcmp(lineHeader, "vt") == 0)
	//	{
	//		glm::vec2 uv;
	//		check(fscanf(file, "%f %f\n", &uv.x, &uv.y), "Couldnt Load UV Data");
	//		temp_uvs.push_back(uv);
	//	}
	//	else if (strcmp(lineHeader, "vn") == 0)
	//	{
	//		glm::vec3 normal;
	//		check(fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z), "Couldnt Load Normal Data");
	//		temp_normals.push_back(normal);
	//	}
	//	else if (strcmp(lineHeader, "f") == 0)
	//	{
	//		FILE* backup = new FILE(*file);

	//		const char* data = (const char*)backup->_Placeholder;
	//			
	//		auto triangles = triangulate(data);

	//		for (auto triangle : triangles) {
	//			for (FILE* vertex : triangle) {
	//				int x;
	//				int y;
	//				int z;
	//				//TODO String Tokenizer for F string of /
	//				auto test = splitString((const char*)vertex->_Placeholder, '/', false);

	//				x = std::stoi(test[0]);
	//				y = std::stoi(test[1]);
	//				z = std::stoi(test[2]);
	//					
	//				//fscanf(vertex, "%i/%i/%i", x, y, z);
	//				printf("Loaded Triangle with Indicies: (%i, %i, %i)", x, y, z);
	//			}
	//		}


	//		//std::string vertex1, vertex2, vertex3;

	//		//fscanf(, "%d/%d/%d", vertex1, vertex2, vertex3);

	//		/*unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
	//		int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
	//		if (matches != 9)
	//		{
	//			printf("Your file is too strong for my parser : ( Try exporting with other options )\n");
	//			return false;
	//		}
	//		vertexIndices.push_back(vertexIndex[0]);
	//		vertexIndices.push_back(vertexIndex[1]);
	//		vertexIndices.push_back(vertexIndex[2]);
	//		uvIndices.push_back(uvIndex[0]);
	//		uvIndices.push_back(uvIndex[1]);
	//		uvIndices.push_back(uvIndex[2]);
	//		normalIndices.push_back(normalIndex[0]);
	//		normalIndices.push_back(normalIndex[1]);
	//		normalIndices.push_back(normalIndex[2]);*/
	//	}

	//	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	//	{
	//		unsigned int vertexIndex = vertexIndices[i];
	//		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
	//		out_vertices.push_back(vertex);
	//	}
	//	for (unsigned int i = 0; i < uvIndices.size(); i++)
	//	{
	//		unsigned int uvIndex = uvIndices[i];
	//		glm::vec2 uv = temp_uvs[uvIndex - 1];
	//		out_uvs.push_back(uv);
	//	}
	//	for (unsigned int i = 0; i < normalIndices.size(); i++)
	//	{
	//		unsigned int normalIndex = normalIndices[i];
	//		glm::vec3 normal = temp_normals[normalIndex - 1];
	//		out_normals.push_back(normal);
	//	}
	//}
	//
	//return true;
}

vector<vector<const char*>> OBJLoader::triangulate(vector<const char*> data)
{
	vector<vector<const char*>> result;
	for (int i = 2; i < data.size(); ++i)
	{
		result.push_back(vector<const char*>({ data[0], data[i - 1], data[i] }));
	}
	return result;
}

vector<const char*> OBJLoader::splitString(const char* data, char character, bool ignoreFirst)
{
	int offset = 0;

	vector<const char*> segments;

	if (!ignoreFirst)
		segments.push_back(data);

	while (data[offset] != 0 && (character == '\n' ? true : data[offset] != '\n') && ((character == ' ' || character == '\n') ? true : (data[offset] != ' ')))
	{
		if (data[offset] == character)
			segments.push_back(&data[++offset]);
		else
			++offset;
	}
	return segments;
}

