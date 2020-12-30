#version 330 core 

layout(location = 0) out vec4 FragColor;

in vec3 v_Position;
//in vec2 v_TexCoord;
			
//uniform sampler2D v_Texture;

void main()
{
	//FragColor = texture(v_Texture,v_TexCoord);
	FragColor= vec4(1.0,1.0,1.0,1.0);

}