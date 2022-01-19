#type vertex
//--------------------------------VERTEX---------------------
#version 330 core
			
layout(location  = 0) in vec3 a_Position;
layout(location  = 1) in vec4 a_Color;
layout(location  = 2) in vec2 a_TexCoord;
layout(location  = 3) in int a_TexIndex;
layout(location  = 4) in vec2 a_Tiling;

out vec4 v_Color;
out vec2 v_TexCoord;
flat out int v_TexIndex;

uniform mat4 u_ModelViewProjectionMatrix;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord * a_Tiling;
	v_TexIndex = a_TexIndex;
	gl_Position = u_ModelViewProjectionMatrix * vec4(a_Position, 1.0);
}

#type fragment
//--------------------------------FRAGMENT---------------------
#version 330 core
			
layout(location = 0) out vec4 color;
layout(location = 1) out int colorId;

in vec4 v_Color;
in vec2 v_TexCoord;
flat in int v_TexIndex;

uniform sampler2D u_Textures[32];

void main()
{
	color = texture(u_Textures[v_TexIndex], v_TexCoord) * v_Color;
	colorId = 50;
}