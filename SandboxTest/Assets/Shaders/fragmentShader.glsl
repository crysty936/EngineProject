#version 330 core 

layout(location = 0) out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 ObjectColor;
uniform vec3 LightColor;
uniform vec3 LightPosition;

void main()
{
	vec3 norm= normalize(Normal);
	vec3 lightDir = normalize(LightPosition - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 Diffuse = diff*LightColor;

	float AmbientStrength= 0.1;
	vec3 Ambient= AmbientStrength * LightColor;
	vec3 Result=(Ambient + Diffuse) * ObjectColor;

	FragColor= vec4(Result, 1.0);
}