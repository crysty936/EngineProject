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
 vec3 Direction;

 vec3 Ambient;
 vec3 Diffuse;
 vec3 Specular;

 float Constant;
 float Linear;
 float Quadratic;
 float InnerCutOff;
 float OuterCutOff;
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

	float Theta=dot(LightDir, normalize(-ULight.Direction));
	float Epsilon = ULight.InnerCutOff - ULight.OuterCutOff;
	float Intensity = clamp((Theta-ULight.OuterCutOff)/Epsilon, 0.0, 1.0);


	Diffuse*=Attenuation * Intensity;
	Specular*=Attenuation * Intensity;

	vec3 Result;
	if(Theta>ULight.OuterCutOff)
	{
		Result=(Ambient + Diffuse + Specular) * UObjectColor ;
	}
	else
	{
		Result=ULight.Ambient * vec3(texture(UMaterial.DiffuseMap, TexCoords));
	}




	FragColor= vec4(Result, 1.0);
}