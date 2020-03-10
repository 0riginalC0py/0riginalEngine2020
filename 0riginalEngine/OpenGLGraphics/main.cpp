#include "glm.hpp"
#include "ext.hpp"
#include "../glcore/gl_core_4_5.h"
#include "glfw3.h"

#include "Mesh.h"
#include "Camera.h"
#include "OBJMesh.h"

#include <fstream>
#include <sstream>


#define uint unsigned int
#define WIDTH 1080
#define HEIGHT 720

enum class TARGET_MODEL{
	TEAPOT,
	BUNNY,
	CUBE
};

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;
Camera cam(glm::vec3(0, 0, 9));

int main()
{
	float r = 0.25f																																																				;
	float g	= 0.25f																																																				;
	float b	= 0.25f																																																				;
	float a	= 1.0f																																																				;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float keyTimer = 0.0f;

	TARGET_MODEL model = TARGET_MODEL::TEAPOT;
	Mesh cube;
	aie::OBJMesh bunny;
	aie::OBJMesh teapot;

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

	
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	cube.initialiseCube();
	glm::mat4 cube_model = glm::mat4(1.0f);

	bunny.load("../OBJs/Orb.obj");
	glm::mat4 bunny_model = glm::mat4(1.0f);

	teapot.load("../OBJs/teapot.obj");
	glm::mat4 teapot_model = glm::mat4(1.0f);

	//Camera
	glm::mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
	
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
	
	glClearColor(0, 0, 0, 1.0f);

	glEnable(GL_DEPTH_TEST);

	//glPolygonMode(GL_FRONT, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

#pragma region Game loop
	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		float currentFrame = glfwGetTime();
		glm::mat4 selectedModel;

		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)																																										;
		
		static int framecount = 0;
		framecount++;

		cam.processKeyboard(window, deltaTime);

		glm::mat4 pv = projection * cam.getViewMatrix();//view;
		glm::vec4 color = glm::vec4(1, 1, 1, 1);// r, g, b, a);
		glm::vec3 ka = glm::vec3(1);
		glm::vec3 kd = glm::vec3(1);
		glm::vec3 ks = glm::vec3(1);
		glm::vec3 ia = glm::vec3(0.5);
		glm::vec3 id = glm::vec3(1);
		glm::vec3 is = glm::vec3(1);
		glm::vec3 lightDirection = glm::normalize(glm::vec3(glm::cos(currentFrame * 2), glm::sin(currentFrame * 2), 0));
		float specPower = 10.0f;
		
#pragma region Model Selection
		if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS))
		{
			if (keyTimer <= 0)
			{
				switch (model)
				{
				case TARGET_MODEL::TEAPOT:
					model = TARGET_MODEL::CUBE;
					break;
				case TARGET_MODEL::BUNNY:
					model = TARGET_MODEL::TEAPOT;
					break;
				case TARGET_MODEL::CUBE:
					model = TARGET_MODEL::BUNNY;
					break;
				}
				printf("Current Model Selected: %i\n", model);
				keyTimer = 1.0f;
			}
		};

		if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS))
		{
			if (keyTimer <= 0)
			{
				switch (model)
				{
				case TARGET_MODEL::TEAPOT:
					model = TARGET_MODEL::BUNNY;
					break;
				case TARGET_MODEL::BUNNY:
					model = TARGET_MODEL::CUBE;
					break;
				case TARGET_MODEL::CUBE:
					model = TARGET_MODEL::TEAPOT;
					break;
				}
				printf("Current Model Selected: %i\n", model);
				keyTimer = 1.0f;
			}
		};
#pragma endregion
#pragma region Model Movement

		switch (model)
		{
			case TARGET_MODEL::TEAPOT:
				selectedModel = teapot_model;
				break;
			case TARGET_MODEL::BUNNY:
				selectedModel = bunny_model;
				break;
			case TARGET_MODEL::CUBE:
				selectedModel = cube_model;
				break;
		}


		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				selectedModel = glm::translate(selectedModel, glm::vec3(-1.0f, 0.0f, 0.0f) * deltaTime);
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				selectedModel = glm::translate(selectedModel, glm::vec3(1.0f, 0.0f, 0.0f) * deltaTime);
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				selectedModel = glm::translate(selectedModel, glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime);
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				selectedModel = glm::translate(selectedModel, glm::vec3(0.0f, -1.0f, 0.0f) * deltaTime);
			if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
				selectedModel = glm::rotate(selectedModel, 0.64f * deltaTime, glm::vec3(0.2f, 1.0f, 1.0f));
		}

		switch (model)
		{
		case TARGET_MODEL::TEAPOT:
			teapot_model = selectedModel;
			break;
		case TARGET_MODEL::BUNNY:
			bunny_model = selectedModel;
			break;
		case TARGET_MODEL::CUBE:
			cube_model = selectedModel;
			break;
		}
#pragma endregion

		glUseProgram(shader_program_id);
		auto bunny_uniform_location = glGetUniformLocation(shader_program_id, "projection_view_matrix");
		glUniformMatrix4fv(bunny_uniform_location, 1, false, glm::value_ptr(pv));
		bunny_uniform_location = glGetUniformLocation(shader_program_id, "model_matrix");
		glUniformMatrix4fv(bunny_uniform_location, 1, false, glm::value_ptr(bunny_model));
		bunny_uniform_location = glGetUniformLocation(shader_program_id, "normal_matrix");
		glUniformMatrix3fv(bunny_uniform_location, 1, false, glm::value_ptr(glm::inverse(bunny_model)));
		bunny_uniform_location = glGetUniformLocation(shader_program_id, "color");
		glUniform4fv(bunny_uniform_location, 1, glm::value_ptr(color));
		bunny_uniform_location = glGetUniformLocation(shader_program_id, "Ka");
		glUniform3fv(bunny_uniform_location, 1, glm::value_ptr(ka));		
		bunny_uniform_location = glGetUniformLocation(shader_program_id, "Kd");
		glUniform3fv(bunny_uniform_location, 1, glm::value_ptr(kd));		
		bunny_uniform_location = glGetUniformLocation(shader_program_id, "Ks");
		glUniform3fv(bunny_uniform_location, 1, glm::value_ptr(ks));		
		bunny_uniform_location = glGetUniformLocation(shader_program_id, "Ia");
		glUniform3fv(bunny_uniform_location, 1, glm::value_ptr(ia));		
		bunny_uniform_location = glGetUniformLocation(shader_program_id, "Id");
		glUniform3fv(bunny_uniform_location, 1, glm::value_ptr(id));		
		bunny_uniform_location = glGetUniformLocation(shader_program_id, "Is");
		glUniform3fv(bunny_uniform_location, 1, glm::value_ptr(is));
		bunny_uniform_location = glGetUniformLocation(shader_program_id, "light_direction");
		glUniform3fv(bunny_uniform_location, 1, glm::value_ptr(lightDirection));
		bunny_uniform_location = glGetUniformLocation(shader_program_id, "specular_power");
		glUniform1f(bunny_uniform_location, specPower);

		
		bunny.draw();

		auto cube_uniform_location = glGetUniformLocation(shader_program_id, "projection_view_matrix");
		glUniformMatrix4fv(cube_uniform_location, 1, false, glm::value_ptr(pv));
		cube_uniform_location = glGetUniformLocation(shader_program_id, "model_matrix");
		glUniformMatrix4fv(cube_uniform_location, 1, false, glm::value_ptr(cube_model));
		bunny_uniform_location = glGetUniformLocation(shader_program_id, "normal_matrix");
		glUniformMatrix3fv(bunny_uniform_location, 1, false, glm::value_ptr(glm::inverse(cube_model)));
		cube_uniform_location = glGetUniformLocation(shader_program_id, "color");
		glUniform4fv(cube_uniform_location, 1, glm::value_ptr(color));
		cube_uniform_location = glGetUniformLocation(shader_program_id, "Ka");
		glUniform3fv(cube_uniform_location, 1, glm::value_ptr(ka));
		cube_uniform_location = glGetUniformLocation(shader_program_id, "Kd");
		glUniform3fv(cube_uniform_location, 1, glm::value_ptr(kd));
		cube_uniform_location = glGetUniformLocation(shader_program_id, "Ks");
		glUniform3fv(cube_uniform_location, 1, glm::value_ptr(ks));
		cube_uniform_location = glGetUniformLocation(shader_program_id, "Ia");
		glUniform3fv(cube_uniform_location, 1, glm::value_ptr(ia));
		cube_uniform_location = glGetUniformLocation(shader_program_id, "Id");
		glUniform3fv(cube_uniform_location, 1, glm::value_ptr(id));
		cube_uniform_location = glGetUniformLocation(shader_program_id, "Is");
		glUniform3fv(cube_uniform_location, 1, glm::value_ptr(is));
		cube_uniform_location = glGetUniformLocation(shader_program_id, "light_direction");
		glUniform3fv(cube_uniform_location, 1, glm::value_ptr(lightDirection));
		cube_uniform_location = glGetUniformLocation(shader_program_id, "specular_power");
		glUniform1f(cube_uniform_location, specPower);

		cube.draw();

		auto teapot_uniform_location = glGetUniformLocation(shader_program_id, "projection_view_matrix");
		glUniformMatrix4fv(teapot_uniform_location, 1, false, glm::value_ptr(pv));
		teapot_uniform_location = glGetUniformLocation(shader_program_id, "model_matrix");
		glUniformMatrix4fv(teapot_uniform_location, 1, false, glm::value_ptr(teapot_model));
		bunny_uniform_location = glGetUniformLocation(shader_program_id, "normal_matrix");
		glUniformMatrix3fv(bunny_uniform_location, 1, false, glm::value_ptr(glm::inverse(teapot_model)));
		teapot_uniform_location = glGetUniformLocation(shader_program_id, "color");
		glUniform4fv(teapot_uniform_location, 1, glm::value_ptr(color));
		teapot_uniform_location = glGetUniformLocation(shader_program_id, "Ka");
		glUniform3fv(teapot_uniform_location, 1, glm::value_ptr(ka));
		teapot_uniform_location = glGetUniformLocation(shader_program_id, "Kd");
		glUniform3fv(teapot_uniform_location, 1, glm::value_ptr(kd));
		teapot_uniform_location = glGetUniformLocation(shader_program_id, "Ks");
		glUniform3fv(teapot_uniform_location, 1, glm::value_ptr(ks));
		teapot_uniform_location = glGetUniformLocation(shader_program_id, "Ia");
		glUniform3fv(teapot_uniform_location, 1, glm::value_ptr(ia));
		teapot_uniform_location = glGetUniformLocation(shader_program_id, "Id");
		glUniform3fv(teapot_uniform_location, 1, glm::value_ptr(id));
		teapot_uniform_location = glGetUniformLocation(shader_program_id, "Is");
		glUniform3fv(teapot_uniform_location, 1, glm::value_ptr(is));
		teapot_uniform_location = glGetUniformLocation(shader_program_id, "light_direction");
		glUniform3fv(teapot_uniform_location, 1, glm::value_ptr(lightDirection));
		teapot_uniform_location = glGetUniformLocation(shader_program_id, "specular_power");
		glUniform1f(teapot_uniform_location, specPower);

		teapot.draw();

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

		if (keyTimer > 0.0f)
			keyTimer -= deltaTime;
	}
#pragma endregion
	glfwDestroyWindow(window)																																																	;
	glfwTerminate()																																																				;
	return 0;
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	cam.processMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	cam.processMouseScroll(yOffset);
}