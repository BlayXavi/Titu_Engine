#type vertex
//--------------------------------VERTEX---------------------
#version 450 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 4) in int a_EntityID;

layout (location = 0) out flat int v_EntityID;

layout(std140, binding = 0) uniform Camera
{
	vec3 u_CameraPosition;
	mat4 u_ModelViewProjectionMatrix;
};

layout(std140, binding = 1) uniform ModelMatrix
{
	mat4 u_ModelMatrix;
};


void main()
{
	v_EntityID = a_EntityID;

	gl_Position = u_ModelViewProjectionMatrix * u_ModelMatrix * vec4(a_Position, 1.0);
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