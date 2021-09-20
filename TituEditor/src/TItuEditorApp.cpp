#include <TituEngine.h>
#include <TituEngine/Core/EntryPoint.h>
#include "TituEditor.h"

namespace TituEngine
{
	class TituEditor : public Application
	{
	public:
		TituEditor()
		{
			PushLayer(new TituEditorLayer());
		}
	};

	Application* CreateApplication()
	{
		TE_PROFILE_PROFILE_FUNC();
		return new TituEditor();
	}
}
