#version 330 core 

layout(location = 0) out vec4 FragColor;

in vec3 v_Position;
in vec2 v_TexCoord;
			
uniform float time;
uniform sampler2D v_Texture;

void main()
{
	vec3 col=0.5 + 0.5*cos(time+ v_Position.xyx +vec3(0,2,4));
	FragColor =  mix(texture(v_Texture,v_TexCoord),vec4(col,1.0),0.5);
}