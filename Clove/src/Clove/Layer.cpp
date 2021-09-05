
#include "Layer.h"

namespace Clove {

	Layer::Layer() {  }
	Layer::~Layer() {  }

	LayerStack::LayerStack() {
		m_layer_insert = m_layers.begin();
	}
	LayerStack::~LayerStack() {
		for (Layer* layer : m_layers) { delete layer; }
	}

	void LayerStack::PushLayer(Layer* layer) {
		m_layer_insert = m_layers.emplace(m_layer_insert, layer);
		// m_layer_insert++;
	}
	void LayerStack::PopLayer(Layer* layer) {
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);
		if (it != m_layers.end()) {
			m_layers.erase(it);
			m_layer_insert--;
		}
	}

	void LayerStack::PushOverlay(Layer* overlay) {
		m_layers.emplace_back(overlay);
	}
	void LayerStack::PopOverlay(Layer* overlay) {
		auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
		if (it != m_layers.end()) m_layers.erase(it);
	}

}