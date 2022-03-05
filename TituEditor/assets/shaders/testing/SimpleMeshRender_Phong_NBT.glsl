#type vertex
//--------------------------------VERTEX---------------------
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec2 a_TexCoord;

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
	float u_AmbientLightIntensity;
	vec4 u_AmbientLightColor;
	vec3 u_LightPosition;
	vec4 u_LightColor;
};

layout (location = 0) out vec2 v_TexCoord;
layout (location = 1) out vec3 v_TangentLightPos;
layout (location = 2) out vec3 v_TangentCameraPos;
layout (location = 3) out vec3 v_TangentFragPos;

void main()
{
	v_TexCoord = a_TexCoord;

	mat3 normalMatrix = transpose(inverse(mat3(u_ModelMatrix)));

	vec3 v_Normal =	normalize(normalMatrix * a_Normal);
	vec3 v_Tangent =	normalize(normalMatrix * a_Tangent);
	v_Tangent = normalize(v_Tangent - dot(v_Tangent, v_Normal) * v_Normal);
	vec3 v_Bitangent =	cross(v_Normal, v_Tangent);

	mat3 TBN = transpose(mat3(v_Tangent, v_Bitangent, v_Normal));

	v_TangentLightPos = TBN * u_LightPosition;
	v_TangentCameraPos = TBN * u_CameraPosition;
	v_TangentFragPos = TBN * vec3(u_ModelMatrix * vec4(a_Position, 1.0f));

	gl_Position = u_ModelViewProjectionMatrix * u_ModelMatrix * vec4(a_Position, 1.0f);
}

#type fragment
//--------------------------------FRAGMENT---------------------
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int colorId;

layout (location = 0) in vec2 v_TexCoord;
layout (location = 1) in vec3 v_TangentLightPos;
layout (location = 2) in vec3 v_TangentCameraPos;
layout (location = 3) in vec3 v_TangentFragPos;

layout (binding = 0) uniform sampler2D u_ColorTexture;
layout (binding = 1) uniform sampler2D u_SpecularTexture;
layout (binding = 2) uniform sampler2D u_NormalTexture;

layout(std140, binding = 2) uniform LightingData
{
	float u_AmbientLightIntensity;
	vec4 u_AmbientLightColor;
	vec3 u_LightPosition;
	vec4 u_LightColor;
};

void main()
{
	vec4 texColor = texture(u_ColorTexture, v_TexCoord);
	vec4 specularIntensity = texture(u_SpecularTexture, v_TexCoord);
	vec4 ambientColor = u_AmbientLightColor * u_AmbientLightIntensity;

	vec3 texNormal = texture(u_NormalTexture, v_TexCoord).xyz;
	texNormal = normalize(texNormal* 2.0f -1.0f);

	vec3 N = normalize(texNormal);
	vec3 V = normalize(v_TangentCameraPos - v_TangentFragPos);
	vec3 L = normalize(v_TangentLightPos - v_TangentFragPos);
	vec3 R = normalize(reflect(-L, N));

	float NDotL = max(dot(N, L), 0.0f);
	vec4 diffColor = u_LightColor * NDotL;

	float VDotR = max(dot(V, R), 0.0f);
	float spec = pow(VDotR, 32);
	vec4 specColor = u_LightColor * (specularIntensity * spec);

	color =  vec4(texColor.xyz * vec3((specColor + diffColor + ambientColor)), texColor.a);
	colorId = 1;
}