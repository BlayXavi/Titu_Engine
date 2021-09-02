#pragma once

#include "TituEngine/Layer/Layer.h"

namespace TituEngine
{
	class TE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		virtual void BeginRender();
		virtual void EndRender();
		virtual void OnImGuiRender();
		
	private:
		float m_Time = 0.0f;
	};
}