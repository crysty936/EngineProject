#version 330 core 

layout(location = 0) out vec4 FragColor;
			
in vec3 v_Position;
in vec3 v_Color;
in vec2 v_TexCoord;

uniform sampler2D v_Texture1;
uniform sampler2D v_Texture2;

void main()
{
	vec2 newTextCoord=vec2(-v_TexCoord.x,v_TexCoord.y);

	FragColor = mix(texture(v_Texture1,v_TexCoord), texture(v_Texture2, newTextCoord), 0.2);
}