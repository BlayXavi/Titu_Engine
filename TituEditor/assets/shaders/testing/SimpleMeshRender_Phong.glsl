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
layout (location = 1) out vec3 v_Normal;
layout (location = 2) out vec3 v_VWPos;
layout (location = 3) out flat vec3 v_CameraPosition;

void main()
{
	v_TexCoord = a_TexCoord;

	mat4 v_NormalMatrix = transpose(inverse(u_ModelMatrix));
	v_Normal =  normalize(mat3(v_NormalMatrix) * a_Normal);

	vec4 vPos = u_ModelMatrix * vec4(a_Position, 1.0f);
	v_VWPos = vPos.xyz;
	v_CameraPosition = u_CameraPosition;

	gl_Position = u_ModelViewProjectionMatrix * vPos;

}

#type fragment
//--------------------------------FRAGMENT---------------------
#version 450 core

const int MAX_LIGHTS = 8;

layout(location = 0) out vec4 color;
layout(location = 1) out int colorId;

layout (location = 0) in vec2 v_TexCoord;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec3 v_VWPos;
layout (location = 3) in flat vec3 v_CameraPosition;

layout (binding = 0) uniform sampler2D u_ColorTexture;
layout (binding = 1) uniform sampler2D u_SpecularTexture;

struct PointLight
{
	vec4 Position;
	vec4 Color;
};

layout(std140, binding = 2) uniform PointLightsArray
{
	PointLight pointLights[MAX_LIGHTS];
	int pointLightCount;
};

vec4 CalculatePointLight(PointLight pl, vec3 v_VWPos, vec3 v_CameraPosition)
{
	vec3 N = normalize(v_Normal);
	vec3 V = normalize(v_CameraPosition - v_VWPos);
	vec3 L = normalize(pl.Position.xyz - v_VWPos);
	vec3 R = normalize(reflect(-L, N));

	float NDotL = max(dot(N, L), 0.0f);
	vec4 diffColor = pl.Color * NDotL;

	vec4 specularIntensity = texture(u_SpecularTexture, v_TexCoord);
	float VDotR = max(dot(V, R), 0.0f);
	float spec = pow(VDotR, 32);
	vec4 specColor = pl.Color * (specularIntensity * spec);

	return diffColor + specColor;
}

void main()
{
	vec4 texColor = texture(u_ColorTexture, v_TexCoord);

	vec4 pointLightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	for(int i = 0; i < pointLightCount; i++)
	{
		PointLight pl = pointLights[i];
		pointLightColor += CalculatePointLight(pl, v_VWPos, v_CameraPosition);
	}

	color =  vec4(texColor.xyz * vec3(pointLightColor), texColor.a);
	colorId = pointLightCount;
}