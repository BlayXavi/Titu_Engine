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
		LOCAL = 0,
		WORLD = 1
	};

	struct EditorSnapValues
	{
		float TranslationSnap = 1.0f;
		float RotationSnap = 5.0f;
		float ScaleSnap = 1.0f;
	};
}
