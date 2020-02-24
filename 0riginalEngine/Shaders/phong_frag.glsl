#version 410

in vec4 vPosition;
in vec3 vNormal;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float specularPower;

uniform vec3 Ia;

uniform vec3 Id;
uniform vec3 Is;
uniform vec3 LightDirection;

out vec4 FragColour;

void main()
{
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);
	
	float lambertTerm = max(0, min(1, dot(N, -L)));
	
	vec3 V = normalize(cameraPos - vPosition.xyz);
	vec3 R = reflect(L, N);
	
	float specularTerm = pow(max(0, dot(R, V)), specularPower);
	
	vec3 ambient = Ia * Ka;
	vec3 diffuse = Id * Kd * lambertTerm;
	vec3 specular = Is * Ks * specularTerm;
	
	FragColour = vec4(ambient + diffuse + specular, 1);
}