#version 450

layout(location = 0) in vec3 local_position;

uniform mat4 projection_view_matrix;
uniform mat4 model_matrix;

out vec3 pos;

void main()
{
	pos = local_position;
	gl_Position = (projection_view_matrix * model_matrix) * vec4(local_position, 1);
}