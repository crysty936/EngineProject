#version 330 core 

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec3 v_Position;
out vec2 v_TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	v_Position=a_Position;
	v_TexCoord=a_TexCoord;

	gl_Position= projection * view * model * vec4(a_Position , 1.0);
	
}