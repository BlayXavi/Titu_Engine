#include "tepch.h"
#include "LayerStack.h"

TituEngine::LayerStack::LayerStack()
{
	m_LayerInsertIndex = 0;
}

TituEngine::LayerStack::~LayerStack()
{
	for (Layer* layer : m_Layers)
	{
		layer->OnDetach();
		delete layer;
		layer = NULL;
	}
}

void TituEngine::LayerStack::PushLayer(Layer* layer)
{
	m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
	m_LayerInsertIndex++;
	layer->OnAttach();
}

void TituEngine::LayerStack::PushOverlay(Layer* overlay)
{
	m_Layers.emplace_back(overlay);
	overlay->OnAttach();
}

void TituEngine::LayerStack::PopLayer(Layer* layer)
{
	std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (it != m_Layers.end())
	{
		layer->OnDetach();
		m_Layers.erase(it);
		m_LayerInsertIndex--;
	}
}

void TituEngine::LayerStack::PopOverlay(Layer* overlay)
{
	std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
	if (it != m_Layers.end())
	{
		overlay->OnDetach();
		m_Layers.erase(it);
	}
}
