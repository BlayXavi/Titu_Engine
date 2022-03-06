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
};

layout (location = 0) out vec2 v_TexCoord;
layout (location = 1) out vec3 v_VWPos;
layout (location = 2) out flat vec3 v_CameraPosition;
layout (location = 3) out flat mat4 v_NormalMatrix;

void main()
{
	v_TexCoord = a_TexCoord;

	v_NormalMatrix = transpose(inverse(u_ModelMatrix));

	vec4 vPos = u_ModelMatrix * vec4(a_Position, 1.0f);
	v_VWPos = vPos.xyz;
	v_CameraPosition = u_CameraPosition;

	gl_Position = u_ModelViewProjectionMatrix * vPos;

}

#type fragment
//--------------------------------FRAGMENT---------------------
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int colorId;

layout (location = 0) in vec2 v_TexCoord;
layout (location = 1) in vec3 v_VWPos;
layout (location = 2) in flat vec3 v_CameraPosition;
layout (location = 3) in flat mat4 v_NormalMatrix;

layout (binding = 0) uniform sampler2D u_ColorTexture;
layout (binding = 1) uniform sampler2D u_NormalTexture;

struct PointLight
{
	vec4 Position;
	vec4 Color;
};

layout(std140, binding = 2) uniform PointLightsArray
{
	PointLight pointLights[8];
	int pointLightCount;
};

vec4 CalculatePointLight(PointLight pl, float specI)
{
	vec4 texNormal = texture(u_NormalTexture, v_TexCoord);
	texNormal = texNormal * 2.0f - 1.0f;

	vec3 N = normalize(mat3(v_NormalMatrix) * texNormal.xyz);
	vec3 V = normalize(v_CameraPosition - v_VWPos);
	vec3 L = normalize(pl.Position.xyz - v_VWPos);
	vec3 R = normalize(reflect(-L, N));

	float NDotL = max(dot(N, L), 0.0f);
	vec4 diffColor = pl.Color * NDotL;

	float VDotR = max(dot(V, R), 0.0f);
	float spec = pow(VDotR, 32);
	vec4 specColor = pl.Color * (specI * spec);

	return diffColor + specColor;
}

void main()
{
	vec4 texColor = texture(u_ColorTexture, v_TexCoord);

	vec4 pointLightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	for(int i = 0; i < pointLightCount; i++)
	{
		PointLight pl = pointLights[i];
		pointLightColor += CalculatePointLight(pl, texColor.a);
	}

	color =  vec4(texColor.xyz * vec3(pointLightColor), 1.0f);
	colorId = pointLightCount;
}