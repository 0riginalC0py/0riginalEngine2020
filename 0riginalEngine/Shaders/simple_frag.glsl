#version 450

uniform vec4 color = vec4(1,1,1,1);

in vec3 pos;

out vec4 final_color;

void main()
{
	final_color = vec4(pos * 0.5 + vec3(0.5), 1) * color;
}