#version 330 core

layout (triangles) in;
layout (triangle_strip,max_vertices = 3) out;

out vec3 CurrentPosition;
out vec3 Normal;
out vec3 Color;
out vec2 TexCord;
out vec4 FragPositionLight;

in DATA
{
	vec3 CurrentPosition;
	vec3 Normal;
	vec3 Color;
	vec2 TexCord;
	vec4 FragPositionLight;
	mat4 CameraMatrix;
	float T;
} data_in[];


void main(){
	
	vec3 v0 = vec3(gl_in[0].gl_Position -  gl_in[1].gl_Position);
	vec3 v1 = vec3(gl_in[2].gl_Position -  gl_in[1].gl_Position);
	vec4 surfaceNormal = vec4(normalize(cross(v0,v1)),0.0f);

	gl_Position = data_in[0].CameraMatrix * (gl_in[0].gl_Position + surfaceNormal * data_in[0].T);
	CurrentPosition = data_in[0].CurrentPosition;
	Normal = data_in[0].Normal;
	Color = data_in[0].Color;
	TexCord = data_in[0].TexCord;
	EmitVertex();

	gl_Position = data_in[1].CameraMatrix * (gl_in[1].gl_Position + surfaceNormal * data_in[1].T);
	CurrentPosition = data_in[1].CurrentPosition;
	Normal = data_in[1].Normal;
	Color = data_in[1].Color;
	TexCord = data_in[1].TexCord;
	EmitVertex();

	
	gl_Position = data_in[2].CameraMatrix * (gl_in[2].gl_Position + surfaceNormal * data_in[2].T);
	CurrentPosition = data_in[2].CurrentPosition;
	Normal = data_in[2].Normal;
	Color = data_in[2].Color;
	TexCord = data_in[2].TexCord;
	EmitVertex();

	EndPrimitive();
}


