#pragma once

#include "Scene.h"

namespace TituEngine
{
	class SceneSerializer
	{
	public:
		static void SerializeScene(Scene* const scene, const std::string path);

		static bool DeserializeScene(Scene * const scene, const std::string path);
	};
}