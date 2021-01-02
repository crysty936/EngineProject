#version 330 core 

layout(location = 0) out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 ObjectColor;
uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform vec3 CameraPosition;

void main()
{
	vec3 norm= normalize(Normal);
	vec3 LightDir = normalize(LightPosition - FragPos);
	float diff = max(dot(norm, LightDir), 0.0);
	vec3 Diffuse = diff * LightColor;

	float AmbientStrength= 0.1;
	vec3 Ambient= AmbientStrength * LightColor;

	float SpecularStrength = 0.5;
	vec3 ViewDirection = normalize(CameraPosition - FragPos);
	vec3 ReflectDir = (-LightDir, norm);
	float SpecAmount = pow(max(dot(ViewDirection, ReflectDir), 0.0), 32);
	vec3 Specular = SpecularStrength * SpecAmount * LightColor;


	vec3 Result=(Ambient + Diffuse + Specular) * ObjectColor;

	FragColor= vec4(Result, 1.0);
}