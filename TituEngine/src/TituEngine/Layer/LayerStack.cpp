#include "tepch.h"
#include "LayerStack.h"

TituEngine::LayerStack::LayerStack()
{
	m_LayerInsert = m_Layers.begin();
}

TituEngine::LayerStack::~LayerStack()
{
	for (size_t i = 0; i < m_Layers.size(); i++)
	{
		Layer* layer = m_Layers[i];
		delete layer;
		layer = NULL;
	}
}

void TituEngine::LayerStack::PushLayer(Layer* layer)
{
	m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
}

void TituEngine::LayerStack::PushOverlay(Layer* overlay)
{
	m_Layers.emplace_back(overlay);
}

void TituEngine::LayerStack::PopLayer(Layer* layer)
{
	std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (it != m_Layers.end())
	{
		m_Layers.erase(it);
		m_LayerInsert--;
	}
}

void TituEngine::LayerStack::PopOverlay(Layer* overlay)
{
	std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
	if (it != m_Layers.end())
		m_Layers.erase(it);
}
