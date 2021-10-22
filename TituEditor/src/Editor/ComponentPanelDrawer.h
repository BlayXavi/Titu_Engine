#pragma once
#include "TituEngine/Scene/Components.h"

namespace TituEngine
{
	class ComponentPanelDrawer
	{
	public:
		static void DrawTransform(TransformComponent& transform );
		static void DrawCameraComponent(CameraComponent& transform);
	};
}