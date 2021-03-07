#pragma once

#include "TituEngine/Events/Event.h"
#include "TituEngine/Core.h"

namespace TituEngine
{
	class TE_API Layer
	{
	public:
		Layer(const std::string& name = "LAYER");
		~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};
}
