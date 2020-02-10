#include "glm.hpp"
#include "ext.hpp"
#include "../glcore/gl_core_4_5.h"
#include "glfw3.h"

#include "Mesh.h"

#include <fstream>
#include <sstream>

#define uint unsigned int
#define WIDTH 1080
#define HEIGHT 720
int main()
{
	float r = 0.25f																																																				;
	float g	= 0.25f																																																				;
	float b	= 0.25f																																																				;
	float a	= 1.0f																																																				;

	Mesh cube;

	bool rpositive = true;
	bool gpositive = true;
	bool bpositive = true;
	bool apositive = true;

	//Initialise everything
	if (glfwInit() == false)
		return -1																																																				;

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Computer Graphics", nullptr, nullptr)																																		;

	if (window == nullptr)
	{
		glfwTerminate()																																																			;
		return -2																																																				;
	}

	glfwMakeContextCurrent(window)																																																;

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window)																																																;
		glfwTerminate()																																																			;
		return -3																																																				;
	}

	//auto major = ogl_GetMajorVersion();
	//auto minor = ogl_GetMinorVersion();
	//printf("GL:");

	cube.initialiseCube();

	//Camera
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 50.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(-2, -2, 1), glm::vec3(0), glm::vec3(0, 1, 0));
	glm::mat4 model = glm::mat4(1.0f);
	
	uint vertex_shader_id = 0;
	uint fragment_shader_id = 0;
	uint shader_program_id = 0;

#pragma region Vertex Shader

	//Load shader from file into string
	std::string shader_data;
	std::ifstream in_file_stream("..\\Shaders\\simple_vertex.glsl", std::ifstream::in);
	
	//Load the source into the string for compilation
	std::stringstream string_stream;
	if (in_file_stream.is_open())
	{
		string_stream << in_file_stream.rdbuf();
		shader_data = string_stream.str();
		in_file_stream.close();
	}


	//Allocate space for shader program
	vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

	//Convert to raw char*
	const char* data = shader_data.c_str();

	//Send in the char* to shader location
	glShaderSource(vertex_shader_id, 1, (const GLchar**)&data, 0);

	//Build
	glCompileShader(vertex_shader_id);

	//Did it work
	//Check the shader compiled
	GLint success = GL_FALSE;
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
	
	if (success == GL_FALSE)
	{
		GLint log_length = 0;
		glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &log_length);
		char* log = new char[log_length];
		glGetShaderInfoLog(vertex_shader_id, log_length, 0, log);

		std::string error_message(log);
		error_message += "VERTEX_SHADER_FAILED_TO_COMPILE";
		printf(error_message.c_str());
		delete[] log;
	}

#pragma endregion

#pragma region Fragment Shader
	//Load shader from file into string
	std::ifstream in_file_stream_frag("..\\Shaders\\simple_frag.glsl", std::ifstream::in);
	
	//Load the source into the string for compilation
	std::stringstream frag_string_stream;
	if (in_file_stream_frag.is_open())
	{
		frag_string_stream << in_file_stream_frag.rdbuf();
		shader_data = frag_string_stream.str();
		in_file_stream_frag.close();
	}


	//Allocate space for shader program
	fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	//Convert to raw char*
	data = shader_data.c_str();

	//Send in the char* to shader location
	glShaderSource(fragment_shader_id, 1, (const GLchar**)&data, 0);

	//Build
	glCompileShader(fragment_shader_id);

	//Did it work
	//Check the shader compiled
	success = GL_FALSE;
	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLint log_length = 0;
		glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &log_length);
		char* log = new char[log_length];
		glGetShaderInfoLog(fragment_shader_id, log_length, 0, log);

		std::string error_message(log);
		error_message += "FRAGMENT_SHADER_FAILED_TO_COMPILE";
		printf(error_message.c_str());
		delete[] log;
	}
#pragma endregion

#pragma region Linking Shader
	//Link the two
	shader_program_id = glCreateProgram();

	glAttachShader(shader_program_id, vertex_shader_id);
	glAttachShader(shader_program_id, fragment_shader_id);

	glLinkProgram(shader_program_id);

	success = GL_FALSE;
	glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);

	if (!success)
	{
		GLint log_length = 0;
		glGetProgramiv(shader_program_id, GL_INFO_LOG_LENGTH, &log_length);
		char* log = new char[log_length];
		glGetProgramInfoLog(shader_program_id, log_length, 0, log);

		std::string error_message(log);
		error_message += "SHADER_FAILED_TO_COMPILE";
		printf(error_message.c_str());
		delete[] log;
	}

#pragma endregion

	glClearColor(0.211764706f, 0.223529412f, 0.243137255f, 1.0f); //r, g, b, a)																																						
	
	//glPolygonMode(GL_FRONT, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Game loop
	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)																																										;

		static int framecount = 0;
		framecount++;

		model = glm::rotate(model, 0.016f, glm::vec3(0.2f, 1.0f, 1.0f));

		glm::mat4 pv = projection * view;
		glm::vec4 color = glm::vec4(r, g, b, a);

		glUseProgram(shader_program_id);
		auto uniform_location = glGetUniformLocation(shader_program_id, "projection_view_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(pv));
		uniform_location = glGetUniformLocation(shader_program_id, "model_matrix");
		glUniformMatrix4fv(uniform_location, 1, false, glm::value_ptr(model));
		uniform_location = glGetUniformLocation(shader_program_id, "color");
		glUniform4fv(uniform_location, 1, glm::value_ptr(color));
		
		cube.draw();

		//GAME LOGIC, UPDATE AND RENDER
		if (rpositive)
			r += 0.009f;
		else
			r -= 0.009f;

		if(gpositive)
			g += 0.003f;
		else
			g -= 0.003f;

		if(bpositive)
			b += 0.005f;
		else
			b -= 0.005f;

		if (apositive)
			a += 0.007f;
		else
			a -= 0.007f;


		if (r >= 1)
			rpositive = false;
		else if (r <= 0)
			rpositive = true;

		if (g >= 1)
			gpositive = false;
		else if (g <= 0)
			gpositive = true;

		if (b >= 1)
			bpositive = false;
		else if (b <= 0)
			bpositive = true;

		if (a >= 1)
			apositive = false;
		else if (a <= 0)
			apositive = true;

		glfwSwapBuffers(window)																																																	;
		glfwPollEvents();

	}

	glfwDestroyWindow(window)																																																	;
	glfwTerminate()																																																				;
	return 0;
}