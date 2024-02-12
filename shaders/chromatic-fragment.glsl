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

const float kR = 0.65;
const float kG = 0.67;
const float kB = 0.69;
const float FresnalPower = 5.0;

const float F = ((1.0-kG) * (1.0-kG)) / ((1.0+kG) * (1.0+kG)) ;
void main()
{
	vec3 I = normalize(CurrentPosition - CameraPosition);
	vec3 N = normalize(Normal);

	float ratio = F + (1.0 - F) * pow((1.0f - dot(-I,N)),FresnalPower);

	vec3 RefractR = refract(-I,-N,kR);
	vec3 RefractG = refract(-I,-N,kG);
	vec3 RefractB = refract(-I,-N,kB);

	vec3 Reflect = reflect(I,N);

	vec3 Refract;

	Refract.r = vec3(texture(skybox,RefractR)).r;
	Refract.g = vec3(texture(skybox,RefractG)).g;
	Refract.b = vec3(texture(skybox,RefractB)).b;
	
	vec3 RF = vec3(texture(skybox,Reflect));

	vec3 color = mix(Refract,RF,ratio);

	FragColor = vec4(color,1.0);
}