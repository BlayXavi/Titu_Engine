#pragma once
namespace TituEngine
{
	enum class TRANSFORM_MANIPULATION_OPERATION
	{
		TRANSLATE,
		ROTATE,
		SCALE,
		BOUNDS,
	};

	enum class COORDINATE_SYSTEM
	{
		WORLD = 0,
		LOCAL = 1
	};

}