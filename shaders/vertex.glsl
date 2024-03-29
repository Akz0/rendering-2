#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexture;

out vec3 CurrentPosition;
out vec3 Normal;
out vec3 Color;
out vec2 TexCord;
out vec4 FragPositionLight;

uniform mat4 CameraMatrix;
uniform mat4 model;
uniform mat4 lightProjection;
uniform float T;


out DATA
{
	vec3 CurrentPosition;
	vec3 Normal;
	vec3 Color;
	vec2 TexCord;
	vec4 FragPositionLight;
	mat4 CameraMatrix;
	float T;
} data_out;


void main()
{
   CurrentPosition = vec3(model * vec4(aPos.x,aPos.y,aPos.z,1.0f));
   gl_Position = model * vec4(aPos,1.0f);
   
   FragPositionLight = lightProjection * vec4(CurrentPosition,1.0f);
   data_out.FragPositionLight = lightProjection * vec4(CurrentPosition,1.0f);
   data_out.CameraMatrix = CameraMatrix;
   data_out.CurrentPosition = CurrentPosition;
   data_out.Normal = mat3(transpose(inverse(model))) * aNormal;
   data_out.Color = aColor; 
   data_out.TexCord = aTexture;
   data_out.T = T;
}