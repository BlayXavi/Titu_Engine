#type vertex
//--------------------------------VERTEX---------------------
#version 330 core
			
layout(location  = 0) in vec3 a_Position;

out vec3 v_Position;

uniform mat4 u_ModelViewProjectionMatrix;

void main()
{
	v_Position = a_Position;
	gl_Position = u_ModelViewProjectionMatrix * vec4(a_Position, 1.0);
}

#type fragment
//--------------------------------FRAGMENT---------------------
#version 330 core
			
out vec4 color;
in vec3 v_Position;

uniform vec4 u_Color;

void main()
{
	color = u_Color;
}