#version 330 core 

layout(location = 0) out vec4 FragColor;

in vec3 v_Position;
			
uniform float time;

void main()
{
	vec3 col=0.5 + 0.5*cos(time+ v_Position.xyx +vec3(0,2,4));
	FragColor = vec4(col,1.0);
}