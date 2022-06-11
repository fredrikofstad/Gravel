#pragma once

#include "Engine/Core.h"
#include "Layer.h"

#include <vector>

namespace Gravel {

	class GRAVEL_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void AddLayer(Layer* layer);
		void AddOverlay(Layer* overlay);
		void RemoveLayer(Layer* layer);
		void RemoveOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }
	private:
		std::vector<Layer*> m_layers;
		unsigned int m_layerInsertIndex = 0;
	};

}