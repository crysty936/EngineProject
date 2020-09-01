#version 330 core 

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec2 a_TexCoord;

out vec3 v_Position;
out vec3 v_Color;
out vec2 v_TexCoord;

void main()
{
	v_Position=a_Position;
	v_Color=a_Color;
	v_TexCoord=a_TexCoord;
	vec3 newPosition=vec3(a_Position.x,a_Position.y,a_Position.z);

	gl_Position= vec4(newPosition , 1.0);
	
}