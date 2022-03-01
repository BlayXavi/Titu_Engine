#type vertex
//--------------------------------VERTEX---------------------
#version 450 core
			
layout(location  = 0) in vec3 a_Position;
layout(location  = 1) in vec3 a_Normal;
layout(location  = 3) in int a_EntityID;

layout (location = 0) out vec3 v_Normal;
layout (location = 1) out flat int v_EntityID;

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
	gl_Position = u_ModelViewProjectionMatrix * u_ModelMatrix *  vec4(a_Position, 1.0);
	v_Normal = normalize(mat3(transpose(inverse(u_ModelMatrix))) * a_Normal);
	v_EntityID = a_EntityID;
}

#type fragment
//--------------------------------FRAGMENT---------------------
#version 450 core
			
layout(location = 0) out vec4 color;
layout(location = 1) out int colorId;

layout (location = 0) in vec3 v_Normal;
layout (location = 1) in flat int v_EntityID;

void main()
{
	color = vec4(v_Normal.xyz, 1.0f);
	colorId = 1;
}
