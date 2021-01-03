#version 330 core 

layout(location = 0) out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 Result;


void main()
{

	FragColor= vec4(Result, 1.0);
}