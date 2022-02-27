#type vertex
//--------------------------------VERTEX---------------------
#version 450 core
			
layout(location  = 0) in vec3 a_Position;
layout(location  = 3) in int a_EntityID;

layout (location = 0) out flat int v_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ModelViewProjectionMatrix;
} camera;

layout(std140, binding = 1) uniform ModelMatrix
{
	mat4 u_ModelMatrix;
} modelmatrix;

void main()
{
	gl_Position = camera.u_ModelViewProjectionMatrix * modelmatrix.u_ModelMatrix *  vec4(a_Position, 1.0);

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
	color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	colorId = v_EntityID;
}
