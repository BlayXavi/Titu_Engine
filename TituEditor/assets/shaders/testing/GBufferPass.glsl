#type vertex
//--------------------------------VERTEX---------------------
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 3) in vec2 a_TexCoord;
layout(location = 4) in int a_EntityID;

layout(std140, binding = 0) uniform Camera
{
	vec3 u_CameraPosition;
	mat4 u_ModelViewProjectionMatrix;
};

layout(std140, binding = 1) uniform ModelMatrix
{
	mat4 u_ModelMatrix;
	int u_EntityID;
};

layout (location = 0) out vec2 v_TexCoord;
layout (location = 1) out vec3 v_VWPos;
layout (location = 2) out vec3 v_Normal;
layout (location = 3) out flat int v_EntityID;

void main()
{
	v_TexCoord = a_TexCoord;

	mat4 v_NormalMatrix = transpose(inverse(u_ModelMatrix));
	v_Normal = normalize(vec3(v_NormalMatrix * vec4(a_Normal, 1.0f)));

	vec4 vPos = u_ModelMatrix * vec4(a_Position, 1.0f);
	v_VWPos = vPos.xyz;

	gl_Position = u_ModelViewProjectionMatrix * vPos;
	v_EntityID = u_EntityID;
}

#type fragment
//--------------------------------FRAGMENT---------------------
#version 450 core

const int MAX_LIGHTS = 8;

layout(location = 0) out vec4 positionColor;
layout(location = 1) out vec4 normalsColor;
layout(location = 2) out vec4 colorSpecularColor;
layout(location = 3) out int entityIDColor;

layout (location = 0) in vec2 v_TexCoord;
layout (location = 1) in vec3 v_VWPos;
layout (location = 2) in vec3 v_Normal;
layout (location = 3) in flat int v_EntityID;

layout (binding = 0) uniform sampler2D u_ColorTexture;
layout (binding = 1) uniform sampler2D u_SpecularTexture;

void main()
{
	vec4 texColor = texture(u_ColorTexture, v_TexCoord);
	float spec = texture(u_SpecularTexture, v_TexCoord).r;

	positionColor =  vec4(v_VWPos, 1.0f);
	normalsColor =  vec4(normalize(v_Normal.xyz), 1.0f);
	colorSpecularColor =  vec4(texColor.xyz, 1.0f);
	entityIDColor = v_EntityID;
}