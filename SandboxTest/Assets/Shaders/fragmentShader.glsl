#version 330 core 

layout(location = 0) out vec4 FragColor;

struct Material
{
	float Shininess;
	
	sampler2D DiffuseMap;
	sampler2D SpecularMap;
	sampler2D EmissionMap;
};

struct Light
{
 vec3 Position;

 vec3 Ambient;
 vec3 Diffuse;
 vec3 Specular;

 float Constant;
 float Linear;
 float Quadratic;

};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 UObjectColor;
uniform Material UMaterial;
uniform Light ULight;

void main()
{

	float Distance = length(ULight.Position - FragPos);
	float Attenuation = 1.0/(ULight.Constant + ULight.Linear * Distance + ULight.Quadratic*(Distance*Distance));

	vec3 Ambient = ULight.Ambient * vec3(texture(UMaterial.DiffuseMap, TexCoords)) ;

	vec3 norm= normalize(Normal);
	vec3 LightDir = normalize(ULight.Position - FragPos);
	float diff = max(dot(norm, LightDir), 0.0);
	vec3 Diffuse = ULight.Diffuse * diff * vec3(texture(UMaterial.DiffuseMap, TexCoords));

	vec3 ViewDirection = normalize(-FragPos);
	vec3 ReflectDir = reflect(-LightDir, norm);
	float SpecAmount = pow(max(dot(ViewDirection, ReflectDir), 0.0), UMaterial.Shininess);
	vec3 Specular = ULight.Specular *SpecAmount * vec3(texture(UMaterial.SpecularMap, TexCoords));

	//vec3 Emission = vec3(texture(UMaterial.EmissionMap, TexCoords));

	Ambient*=Attenuation;
	Diffuse*=Attenuation;
	Specular*=Attenuation;

	vec3 Result=(Ambient + Diffuse + Specular) * UObjectColor ;

	FragColor= vec4(Result, 1.0);
}