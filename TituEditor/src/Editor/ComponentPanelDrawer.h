#pragma once
#include "TituEngine/Scene/Components.h"

namespace TituEngine
{
	class ComponentPanelDrawer
	{
	public:
		static void DrawTransformComponent(TransformComponent& transform);
		static void DrawCameraComponent(CameraComponent& transform);

		static void DrawCamera(Camera& cam);
		static void DrawTransform(glm::mat4& transform);
		static bool DrawVec3(std::string label, glm::vec3& values);

	};
}