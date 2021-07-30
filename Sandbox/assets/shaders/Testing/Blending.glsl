#type vertex
//--------------------------------VERTEX---------------------
#version 330 core
			
layout(location  = 0) in vec3 a_Position;
layout(location  = 1) in vec2 a_TextCoord;

out vec4 v_Color;
out vec2 v_TextCoord;

uniform vec4 u_Color;
uniform mat4 u_ModelViewProjectionMatrix;

void main()
{
	v_Color = u_Color;
	v_TextCoord = a_TextCoord * 10.0f;
	gl_Position = u_ModelViewProjectionMatrix * vec4(a_Position, 1.0);
}

#type fragment
//--------------------------------FRAGMENT---------------------
#version 330 core
			
out vec4 color;

in vec4 v_Color;
in vec2 v_TextCoord;

uniform sampler2D u_Texture;

void main()
{
	color = v_Color * texture(u_Texture, v_TextCoord);
}