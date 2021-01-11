#version 330 core 
layout(location = 0) out vec4 FragColor;

uniform vec3 ULightColor;


void main()
{
	FragColor= vec4(ULightColor, 1.0);
}