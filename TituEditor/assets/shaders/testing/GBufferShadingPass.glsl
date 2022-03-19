#type vertex
//--------------------------------VERTEX---------------------
#version 450 core
layout(location = 0) in vec3 a_Position;
layout(location = 3) in vec2 a_TexCoord;

layout(location = 0) out vec2 v_TexCoord;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = vec4(a_Position, 1.0f);
}

#type fragment
//--------------------------------FRAGMENT---------------------
#version 450 core

const int MAX_LIGHTS = 8;

layout(location = 0) out vec4 color;

layout(binding = 0) uniform sampler2D uPositionTexture;
layout(binding = 1) uniform sampler2D uNormalTexture;
layout(binding = 2) uniform sampler2D uColorSpecularTexture;

layout(location = 0) in vec2 v_TexCoord;

layout(std140, binding = 0) uniform Camera
{
	vec3 u_CameraPosition;
	mat4 u_ModelViewProjectionMatrix;
};

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

vec4 CalculatePointLight(PointLight pl, vec3 P, vec3 N, float SpecI)
{
	vec3 V = normalize(u_CameraPosition - P);
	vec3 L = normalize(pl.Position.xyz - P);
	vec3 R = normalize(reflect(-L, N));

	float NDotL = max(dot(N, L), 0.0f);
	vec4 diffColor = pl.Color * NDotL;

	float VDotR = max(dot(V, R), 0.0f);
	float spec = pow(VDotR, 32);
	vec4 specColor = pl.Color * (SpecI * spec);

	return diffColor;
}

void main()
{
	vec3 Position = texture(uPositionTexture, v_TexCoord).xyz;
	vec3 Normal = normalize(texture(uNormalTexture, v_TexCoord).xyz);
	vec4 Color = texture(uColorSpecularTexture, v_TexCoord);
	float specI = Color.z;
	Color = vec4(Color.xyz, 1.0f);

	vec4 pointLightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	for(int i = 0; i < pointLightCount; i++)
	{
		PointLight pl = pointLights[i];
		pointLightColor += CalculatePointLight(pl, Position, Normal, 0.0f);
	}
	
	color = vec4(Color.xyz * (vec3(0.1f, 0.1f, 0.1f) + pointLightColor.xyz), 1.0f);
}
