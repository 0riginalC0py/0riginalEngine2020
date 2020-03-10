#version 450

layout(location = 0) in vec4 local_position;
layout(location = 1) in vec4 vec_normal;
layout(location = 2) in vec2 tex_coord;

uniform mat4 projection_view_matrix;
uniform mat4 model_matrix;
uniform mat3 normal_matrix;

out vec4 pos;
out vec3 normal;
out vec2 tex;

void main()
{
	tex = tex_coord;
	pos = model_matrix * local_position;
	normal = normal_matrix * vec_normal.xyz;
	gl_Position = (projection_view_matrix * model_matrix) * local_position; //(projection_view_matrix * model_matrix) * vec4(local_position, 1);
}