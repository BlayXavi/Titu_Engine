#type vertex
//--------------------------------VERTEX---------------------
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in int a_EntityID;

layout(std140, binding = 0) uniform Camera
{
	vec3 u_CameraPosition;
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
	vec4 LightColor;
};

layout (location = 0) out vec2 v_TexCoord;
layout (location = 1) out vec3 v_Normal;
layout (location = 2) out vec3 v_VWPos;
layout (location = 3) out flat mat4 v_NormalMatrix;
layout (location = 7) out flat int v_EntityID; 

void main()
{
	v_EntityID = a_EntityID;
	v_TexCoord = a_TexCoord;

	v_NormalMatrix = transpose(inverse(u_ModelMatrix));
	v_Normal =  normalize(mat3(v_NormalMatrix) * a_Normal);

	vec4 vPos = u_ModelMatrix * vec4(a_Position, 1.0f);

	gl_Position = u_ModelViewProjectionMatrix * vPos;

	v_VWPos = vPos.xyz;
}

#type fragment
//--------------------------------FRAGMENT---------------------
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int colorId;

layout (location = 0) in vec2 v_TexCoord;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec3 v_VWPos;
layout (location = 3) in flat mat4 v_NormalMatrix;
layout (location = 7) in flat int v_EntityID;

layout (binding = 0) uniform sampler2D u_ColorTexture;
layout (binding = 1) uniform sampler2D u_SpecularTexture;
layout (binding = 2) uniform sampler2D u_NormalTexture;

layout(std140, binding = 0) uniform Camera
{
	vec3 u_CameraPosition;
	mat4 u_ModelViewProjectionMatrix;
};

layout(std140, binding = 2) uniform LightingData
{
	float AmbientLightIntensity;
	vec4 AmbientLightColor;
	vec3 LightPosition;
	vec4 LightColor;
};

void main()
{
	vec4 texColor = texture(u_ColorTexture, v_TexCoord);
	vec4 specularIntensity = texture(u_SpecularTexture, v_TexCoord);
	vec4 ambientColor = AmbientLightColor * AmbientLightIntensity;

	vec3 texNormal = texture(u_NormalTexture, v_TexCoord).xyz;
	texNormal *= 2.0f;
	texNormal -= 1.0f;

	//vec3 N = normalize(mat3(v_NormalMatrix) * normalize(texNormal));

	vec3 N = normalize(v_Normal);
	vec3 V = normalize(u_CameraPosition - v_VWPos);
	vec3 L = normalize(LightPosition - v_VWPos);
	vec3 R = normalize(reflect(-L, N));

	float NDotL = max(dot(N, L), 0.0f);
	vec4 diffColor = LightColor * NDotL;

	float VDotR = max(dot(V, R), 0.0f);
	float spec = pow(VDotR, 32);
	vec4 specColor = LightColor * (specularIntensity * spec);

	color =  vec4(texColor.xyz * vec3((specColor + diffColor + ambientColor)), texColor.a);
	colorId = 1;
}