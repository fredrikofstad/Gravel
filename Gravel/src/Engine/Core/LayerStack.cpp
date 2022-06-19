#include "grpch.h"
#include "LayerStack.h"

namespace Gravel {

	//layerinsert handled where layers get put in, needs to be before any overlay

	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_layers)
		{
			layer->OnDetach();
			delete layer;
		}		
	}

	void LayerStack::AddLayer(Layer* layer)
	{
		m_layers.emplace(m_layers.begin() + m_layerInsertIndex, layer);
		m_layerInsertIndex++;
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
			m_layerInsertIndex--;
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