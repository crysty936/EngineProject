#version 330 core 

layout(location = 0) out vec4 FragColor;

struct Material
{
	float Shininess;
	
	sampler2D DiffuseMap;
	sampler2D SpecularMap;
	sampler2D EmissionMap;
};


struct DirLight
{
	vec3 Direction;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

uniform DirLight UDirectionalLight;

vec3 CalcDirLight(DirLight Light, vec3 Normal, vec3 ViewDir);

struct PointLight
{
	vec3 Position;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;


	float Constant;
	float Linear;
	float Quadratic;
};

vec3 CalcPointLight(PointLight Light, vec3 Normal, vec3 ViewDir);
#define NR_POINT_LIGHTS 4
uniform PointLight UPointLights[NR_POINT_LIGHTS];


struct SpotLight
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

vec3 CalcSpotLight(SpotLight Light, vec3 Normal, vec3 ViewDir);
uniform SpotLight USpotLight;

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

in vec3 INormal;
in vec3 IFragPos;
in vec2 ITexCoords;

uniform Material UMaterial;
uniform Light ULight;

void main()
{

	vec3 norm= normalize(INormal);
	vec3 ViewDirection = normalize(-IFragPos);

	vec3 Result= vec3(0);

	Result+=CalcDirLight(UDirectionalLight, norm, ViewDirection);

	for(int i=0; i< NR_POINT_LIGHTS; i++)
	{
		Result+=CalcPointLight(UPointLights[i],norm,ViewDirection);
	}

	Result+=CalcSpotLight(USpotLight, norm, ViewDirection);



	FragColor= vec4(Result, 1.0);
}



vec3 CalcDirLight(DirLight Light, vec3 Normal, vec3 ViewDir)
{
	vec3 lightDir = normalize(-Light.Direction);
	
	float diff = max(dot(Normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(ViewDir, reflectDir), 0.0), UMaterial.Shininess);

	vec3 ambient = Light.Ambient * vec3(texture(UMaterial.DiffuseMap, ITexCoords));
	vec3 diffuse = Light.Diffuse * diff * vec3(texture(UMaterial.DiffuseMap, ITexCoords));
	vec3 specular = Light.Specular * spec * vec3(texture(UMaterial.SpecularMap,ITexCoords));

	return (ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight Light, vec3 Normal, vec3 ViewDir)
{

	vec3 LightDir = normalize(Light.Position - IFragPos);
	float diff = max(dot(Normal, LightDir), 0.0);

	vec3 ReflectDir = reflect(-LightDir, Normal);
	float SpecAmount = pow(max(dot(ViewDir, ReflectDir), 0.0), UMaterial.Shininess);

	vec3 Ambient = Light.Ambient * vec3(texture(UMaterial.DiffuseMap, ITexCoords));
	vec3 Diffuse = Light.Diffuse * diff * vec3(texture(UMaterial.DiffuseMap, ITexCoords));
	vec3 Specular = Light.Specular *SpecAmount * vec3(texture(UMaterial.SpecularMap, ITexCoords));

	float Distance = length(Light.Position - IFragPos);
	float Attenuation = 1.0/(Light.Constant + Light.Linear * Distance + Light.Quadratic*(Distance*Distance));

	Ambient *= Attenuation;
	Diffuse *= Attenuation;
	Specular *= Attenuation;

	return (Ambient+ Diffuse + Specular);
}


vec3 CalcSpotLight(SpotLight Light, vec3 Normal, vec3 ViewDir)
{
	vec3 LightDir = normalize(Light.Position - IFragPos);
	float diff = max(dot(Normal, LightDir), 0.0);

	vec3 ReflectDir = reflect(-LightDir, Normal);
	float SpecAmount = pow(max(dot(ViewDir, ReflectDir), 0.0), UMaterial.Shininess);

	vec3 Ambient = Light.Ambient * vec3(texture(UMaterial.DiffuseMap, ITexCoords));
	vec3 Diffuse = Light.Diffuse * diff * vec3(texture(UMaterial.DiffuseMap, ITexCoords));
	vec3 Specular = Light.Specular *SpecAmount * vec3(texture(UMaterial.SpecularMap, ITexCoords));

	float Distance = length(Light.Position - IFragPos);
	float Attenuation = 1.0/(Light.Constant + Light.Linear * Distance + Light.Quadratic*(Distance*Distance));
	
	float Theta=dot(LightDir, normalize(-Light.Direction));
	float Epsilon = Light.InnerCutOff - Light.OuterCutOff;
	float Intensity = clamp((Theta-Light.OuterCutOff)/Epsilon, 0.0, 1.0);

	Diffuse*= Attenuation * Intensity;
	Specular*= Attenuation* Intensity;

	vec3 Result;
	if(Theta>Light.OuterCutOff)
	{
		Result=(Ambient + Diffuse + Specular) ;
	}
	else
	{
		Result= Light.Ambient * vec3(texture(UMaterial.DiffuseMap, ITexCoords));
	}

	return Result;

}