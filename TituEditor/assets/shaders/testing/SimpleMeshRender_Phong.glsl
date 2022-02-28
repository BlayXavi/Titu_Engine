#type vertex
//--------------------------------VERTEX---------------------
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in int a_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ModelViewProjectionMatrix;
};

layout(std140, binding = 1) uniform ModelMatrix
{
	mat4 u_ModelMatrix;
};


layout(std140, binding = 2) uniform LightingData
{
	float AmbientLightIntensity;
	vec4 AmbientLightColor;
	vec3 LightPosition;
};

layout (location = 0) out vec2 v_TexCoord;
layout (location = 1) out vec3 v_Normal;
layout (location = 2) out vec3 v_LightDir;
layout (location = 3) out flat int v_EntityID;

void main()
{
	v_EntityID = a_EntityID;
	v_TexCoord = a_TexCoord;

	v_Normal =  normalize(mat3(transpose(inverse(u_ModelMatrix))) * a_Normal);

	vec4 vPos = u_ModelMatrix * vec4(a_Position, 1.0f);

	v_LightDir = normalize(LightPosition - vec3(vPos));

	gl_Position = u_ModelViewProjectionMatrix * vPos;
}

#type fragment
//--------------------------------FRAGMENT---------------------
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int colorId;

layout (location = 0) in vec2 v_TexCoord;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec3 v_LightDir;
layout (location = 3) in flat int v_EntityID;

layout (binding = 0) uniform sampler2D u_Texture;

layout(std140, binding = 2) uniform LightingData
{
	float AmbientLightIntensity;
	vec4 AmbientLightColor;
	vec3 LightPosition;
};

void main()
{
	float diff = max(dot(v_Normal, v_LightDir), 0.0);

	color = texture(u_Texture, v_TexCoord) * vec4(diff, diff, diff, 1.0);
	colorId = 1;
}