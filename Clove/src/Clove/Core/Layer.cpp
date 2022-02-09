#include "clovepch.h"
#include "Layer.h"

namespace Clove {

	Layer::Layer(std::string name) : m_name(name) {  }
	Layer::~Layer() {  }

	LayerStack::LayerStack() {

	}
	LayerStack::~LayerStack() {
		for (Layer* layer : m_layers) { delete layer; }
	}

	void LayerStack::PushLayer(Layer* layer) {
		m_layers.emplace(m_layers.begin() + m_insert_index, layer);
		m_insert_index++;
		layer->OnAttach();
	}
	void LayerStack::PopLayer(Layer* layer) {
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);
		if (it == m_layers.end()) return;
		layer->OnDetach();
		m_layers.erase(it);
		m_insert_index--;
	
	}

	void LayerStack::PushOverlay(Layer* overlay) {
		m_layers.emplace_back(overlay);
		overlay->OnAttach();
	}
	void LayerStack::PopOverlay(Layer* overlay) {
		auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
		if (it == m_layers.end()) return;
		overlay->OnDetach();
		m_layers.erase(it);
	}

}