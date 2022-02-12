#type vertex
//--------------------------------VERTEX---------------------
#version 450 core
			
layout(location  = 0) in vec3 a_Position;
layout(location  = 5) in int a_EntityID;

layout (location = 0) out flat int v_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ModelViewProjectionMatrix;
};

void main()
{
	gl_Position = u_ModelViewProjectionMatrix * vec4(a_Position, 1.0);

	v_EntityID = a_EntityID;
}

#type fragment
//--------------------------------FRAGMENT---------------------
#version 450 core
			
layout(location = 0) out vec4 color;
layout(location = 1) out int colorId;

layout (location = 0) in flat int v_EntityID;

void main()
{
	color = vec4(255.0f, 0.0f, 255.0f, 1.0f);
	colorId = v_EntityID;
}