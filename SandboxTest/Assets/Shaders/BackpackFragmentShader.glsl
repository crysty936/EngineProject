#version 330 core 

layout(location = 0) out vec4 FragColor;

struct Material
{
	float Shininess;
	
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
};


uniform Material UMaterial; 

in vec3 INormal;
in vec3 IFragPos;
in vec2 ITexCoords;

void main()
{

	vec3 norm= normalize(INormal);
	vec3 ViewDirection = normalize(-IFragPos);

	vec3 Result=vec3(texture(UMaterial.texture_diffuse1, ITexCoords));

	FragColor= texture(UMaterial.texture_diffuse1, ITexCoords);
}

