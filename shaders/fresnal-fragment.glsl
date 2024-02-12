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

const float k = 0.2;
const float FresnalPower = 5.0;

void main()
{
	vec3 I = normalize(CurrentPosition - CameraPosition);
	vec3 N = normalize(Normal);

    float F = ((1.0-k) * (1.0-k)) / ((1.0+k) * (1.0+k)) ;
	float ratio = F + (1.0 - F) * pow((1.0f - dot(-I,N)),FresnalPower);
	
	vec3 Refract = refract(-I,-N,k);
	vec3 Reflect = reflect(I,N);

	Reflect.z = Reflect.z * -1;
	vec3 RE = vec3(texture(skybox,Refract));
	vec3 RF = vec3(texture(skybox,Reflect));

	vec3 color = mix(RE,RF,ratio);

	FragColor = vec4(color,1.0);
}