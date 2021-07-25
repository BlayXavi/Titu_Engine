#pragma once

#include "TituEngine/Events/Event.h"
#include "TituEngine/Core/Core.h"
#include "TituEngine/Core/Timestep.h"

namespace TituEngine
{
	class TE_API Layer
	{
	public:
		Layer(const std::string& name = "LAYER");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}

		inline const std::string& GetName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};
}

