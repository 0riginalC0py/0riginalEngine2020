#pragma once

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

#define uint unsigned int

class ShaderManager
{
public:
	uint ID;

	ShaderManager(const char* vertexShaderPath, const char* fragmentShaderPath);

	void useShader();

};

