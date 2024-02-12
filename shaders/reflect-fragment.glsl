#version 330 core

out vec4 FragColor;

in vec3 Color;
in vec2 TexCord;
in vec3 Normal;
in vec3 CurrentPosition;
in vec4 FragPositionLight;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normal0;
uniform sampler2D shadowMap;
uniform samplerCube skybox;

uniform vec4 lightColor;
uniform vec3 lightPosition;
uniform vec3 CameraPosition;

void main()
{	
	vec3 I = normalize(CameraPosition - CurrentPosition);
	vec3 N = normalize(Normal);
	vec3 R = reflect(-I,N);
	R.z = R.z*-1;

	FragColor = vec4(texture(skybox,R).rgb,1.0);
}