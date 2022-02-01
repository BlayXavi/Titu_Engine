#type vertex
//--------------------------------VERTEX---------------------
#version 450 core
			
layout(location  = 0) in vec3 a_Position;
layout(location  = 1) in vec4 a_Color;
layout(location  = 2) in vec2 a_TexCoord;
layout(location  = 3) in int a_TexIndex;
layout(location  = 4) in vec2 a_Tiling;
layout(location  = 5) in int a_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ModelViewProjectionMatrix;
};

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	int TexIndex;
	vec2 TilingFactor;
};

layout (location = 0) out VertexOutput v_Output;
layout (location = 4) out flat int v_EntityID;

void main()
{
	v_Output.Color = a_Color;
	v_Output.TexCoord = a_TexCoord * a_Tiling;
	v_Output.TexIndex = a_TexIndex;
	v_Output.TilingFactor = a_Tiling;

	v_EntityID = a_EntityID;

	gl_Position = u_ModelViewProjectionMatrix * vec4(a_Position, 1.0);
}

#type fragment
//--------------------------------FRAGMENT---------------------
#version 450 core
			
layout(location = 0) out vec4 color;
layout(location = 1) out int colorId;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	int TexIndex;
	vec2 TilingFactor;
};

layout (location = 0) in flat VertexOutput Input;
layout (location = 4) in flat int v_EntityID;


layout (binding = 0) uniform sampler2D u_Textures[32];

void main()
{
	color = texture(u_Textures[Input.TexIndex], Input.TexCoord * Input.TilingFactor) * Input.Color;
	colorId = v_EntityID;
}