#version 450

in vec4 pos;
in vec3 normal;
in vec2 tex;

out vec4 final_color;

uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;

uniform vec4 color = vec4(1,1,1,1);

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float specular_power;

uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;
uniform vec3 light_direction;

uniform vec3 camera_pos;

void main()
{
	vec3 N = normalize(normal);
	vec3 L = normalize(light_direction);
	
	vec3 tex_diffuse = texture(diffuse_texture, tex).rgb;
	vec3 tex_specular = texture(specular_texture, tex).rgb;
	
	float lambert_term = max(0, dot(N, -L));
	
	vec3 V = normalize(camera_pos - pos.xyz);
	vec3 R = reflect(L, N);
	
	float specular_term = pow(max(0, dot(R, V)), specular_power);
	
	vec3 ambient = Ia * Ka;
	vec3 diffuse = Id * Kd * tex_diffuse * lambert_term;
	vec3 specular = Is * Ks * tex_specular * specular_term;
	
	final_color = vec4(ambient + diffuse * specular, 1) * color; //*color;
}