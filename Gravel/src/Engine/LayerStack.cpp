#include "grpch.h"
#include "LayerStack.h"

namespace Gravel {

	//layerinsert handled where layers get put in, needs to be before any overlay

	LayerStack::LayerStack()
	{
		m_layerInsert = m_layers.begin();
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_layers)
			delete layer;
	}

	void LayerStack::AddLayer(Layer* layer)
	{
		m_layerInsert = m_layers.emplace(m_layerInsert, layer);
	}

	void LayerStack::AddOverlay(Layer* overlay)
	{
		m_layers.emplace_back(overlay);
	}

	void LayerStack::RemoveLayer(Layer* layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
			m_layerInsert--;
		}
	}

	void LayerStack::RemoveOverlay(Layer* overlay)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
		}
	}


}