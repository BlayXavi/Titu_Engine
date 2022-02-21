#pragma once

#include "Camera.h"

namespace TituEngine
{
	class Renderer3D
	{
	public:
		static void Shutdown();
		static void Init();

		static void BeginScene(const Camera* cam, const glm::mat4& view_Projection_matrix);
		static void EndScene();
	};
}