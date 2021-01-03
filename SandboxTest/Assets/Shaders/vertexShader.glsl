#version 330 core 

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;
out vec3 Result;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform vec3 CameraPos;
uniform vec3 ObjectColor;

void main()
{
	Normal = mat3(transpose(inverse( model))) * aNormal;
	FragPos = vec3(model * vec4(aPosition, 1.0));

	float AmbientStrength= 0.1;
	vec3 Ambient= AmbientStrength * LightColor;

	vec3 norm= normalize(Normal);
	vec3 LightDir = normalize(LightPosition - FragPos);
	float diff = max(dot(norm, LightDir), 0.0);
	vec3 Diffuse = diff * LightColor;

	float SpecularStrength = 1.0;
	vec3 ViewDirection = normalize(CameraPos -FragPos);
	vec3 ReflectDir = reflect(-LightDir, norm);
	float SpecAmount = pow(max(dot(ViewDirection, ReflectDir), 0.0), 32);
	vec3 Specular = SpecularStrength * SpecAmount * LightColor;

	Result = (Ambient + Diffuse + Specular) * ObjectColor;

	gl_Position=projection * view * model * vec4(aPosition , 1.0);

}