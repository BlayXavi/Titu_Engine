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
};

layout (location = 0) out vec2 v_TexCoord;
layout (location = 1) out vec3 v_Normal;
layout (location = 2) out vec3 v_VPos;
layout (location = 3) out flat int v_EntityID;

void main()
{
	v_EntityID = a_EntityID;
	v_TexCoord = a_TexCoord;

	v_Normal =  normalize(mat3(transpose(inverse(u_ModelMatrix))) * a_Normal);

	vec4 vPos = u_ModelMatrix * vec4(a_Position, 1.0f);

	gl_Position = u_ModelViewProjectionMatrix * vPos;

	v_VPos = vPos.xyz;
}

#type fragment
//--------------------------------FRAGMENT---------------------
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int colorId;

layout (location = 0) in vec2 v_TexCoord;
layout (location = 1) in vec3 v_Normal;
layout (location = 2) in vec3 v_VPos;
layout (location = 3) in flat int v_EntityID;

layout (binding = 0) uniform sampler2D u_ColorTexture;
layout (binding = 1) uniform sampler2D u_SpecularTexture;

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
};

void main()
{

	vec3 normal = normalize(v_Normal);

	vec3 lightDir = normalize(LightPosition - vec3(v_VPos));

	vec3 viewDir = normalize(u_CameraPosition - vec3(v_VPos));
	vec3 reflDir = normalize(reflect(-lightDir, normal)); 

	float diff = max(dot(normal, lightDir), 0.0);
	vec4 diffuse = vec4(diff, diff, diff, 1.0f);

	float spec = pow(max(dot(viewDir, reflDir), 0.0), 32);
	vec4 specular = texture(u_SpecularTexture, v_TexCoord) * spec;

	vec4 light = diffuse + specular;// + (AmbientLightColor * AmbientLightIntensity);

	color = vec4(texture(u_ColorTexture, v_TexCoord).xyz * light.xyz, 1.0f);
	colorId = 1;
}