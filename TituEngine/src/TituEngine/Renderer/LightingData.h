#ifndef LIGHT_DATA_H
#define LIGHT_DATA_H

#include "TituEngine/Core/Core.h"
#include "glm/glm.hpp"

namespace TituEngine
{

#define DIRECTIONAL_LIGHTS_SIZE 4
#define POINT_LIGHTS_SIZE 8

	enum class LightType
	{
		DIRECTIONAL_LIGHT = 0,
		POINT_LIGHT = 1
	};

	struct DirectionalLightData
	{
		glm::vec4 Color;
		glm::vec3 Direction;
		float Intensity;
	};

	struct PointLightData
	{
		glm::vec4 Color;
		glm::vec4 Position;
	};
}
#endif