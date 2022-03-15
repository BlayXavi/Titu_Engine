#type vertex
//-------------------------------------------------VERTEX-------------------------------------------------
#version 450 core

const int MAX_LIGHTS = 8;

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
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

layout (location = 0) out vec2 v_TexCoord;
layout (location = 1) out flat int v_PointLightCount;
layout (location = 2) out vec3 v_CameraPos;
layout (location = 3) out vec3 v_FragPos;
layout (location = 4) out PointLight v_PointLightsTangent[MAX_LIGHTS];

void main()
{
	v_TexCoord = a_TexCoord;

	mat4 v_NormalMatrix = transpose(inverse(u_ModelMatrix));
	vec3 v_Normal =	vec3(normalize(mat3(v_NormalMatrix) * a_Normal));
	vec3 v_Tangent = vec3(normalize(mat3(v_NormalMatrix) * a_Tangent));
	v_Tangent = normalize(v_Tangent - dot(v_Tangent, v_Normal) * v_Normal);
	vec3 v_Bitangent = cross(v_Normal, v_Tangent);

	mat3 TBN = transpose(mat3(v_Tangent, v_Bitangent, v_Normal));
	
	vec4 vertexWorldPos = u_ModelMatrix * vec4(a_Position, 1.0f);

	gl_Position = u_ModelViewProjectionMatrix * vertexWorldPos;

	v_PointLightCount = pointLightCount;
	v_CameraPos = TBN * u_CameraPosition;
	v_FragPos = TBN * vertexWorldPos.xyz;
	for(int i = 0; i < pointLightCount; i++)
	{
		v_PointLightsTangent[i].Position = vec4(TBN * pointLights[i].Position.xyz, 1.0f);
		v_PointLightsTangent[i].Color = pointLights[i].Color;
	}

}

#type fragment
//-------------------------------------------------FRAGMENT-------------------------------------------------
#version 450 core

const int MAX_LIGHTS = 8;

layout(location = 0) out vec4 color;
layout(location = 1) out int colorId;

struct PointLight
{
	vec4 Position;
	vec4 Color;
};
					  
layout (location = 0) in vec2 v_TexCoord;
layout (location = 1) in flat int v_PointLightCount;
layout (location = 2) in vec3 v_CameraPos;
layout (location = 3) in vec3 v_FragPos;
layout (location = 4) in PointLight v_PointLightsTangent[MAX_LIGHTS];

layout (binding = 0) uniform sampler2D u_ColorTexture;
layout (binding = 1) uniform sampler2D u_SpecularTexture;
layout (binding = 2) uniform sampler2D u_NormalTexture;


vec4 CalculatePointLight(PointLight pl, vec4 specI)
{
	vec4 texNormal = texture(u_NormalTexture, v_TexCoord);
	texNormal = texNormal * 2.0f - 1.0f;

	vec3 N = normalize(texNormal.xyz);
	vec3 V = normalize(v_CameraPos - v_FragPos);
	vec3 L = normalize(pl.Position.xyz - v_FragPos);
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
	vec4 texSpec = texture(u_SpecularTexture, v_TexCoord);

	vec4 pointLightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	for(int i = 0; i < v_PointLightCount; i++)
	{
		PointLight pl = v_PointLightsTangent[i];
		pointLightColor += CalculatePointLight(pl, texSpec);
	}

	color =  vec4(texColor.xyz * vec3(pointLightColor), 1.0f);
	colorId = v_PointLightCount;
}