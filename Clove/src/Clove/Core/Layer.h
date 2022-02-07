#pragma once
#include "clovepch.h"
#include "Clove/Events/Event.h"

namespace Clove {

	/*
	* Intended to be subclassed by user.
	*/
	class Layer {
	public:
		Layer();
		virtual ~Layer();

		virtual void OnAttach() {  } // when layer is pushed to layer stack
		virtual void OnDetach() {  } // when layer is popped from layer stack
		virtual void OnUpdate(float dt) {  }
		virtual void OnEvent(Event& e) {  }

		virtual void OnImGuiRender() {  }
	};

	/*
	* Layers: rendered first, e.g. game data
	* Overlays: rendered last, e.g. GUI
	* 
	* Vector stores both. Layers first, overlays last
	* m_layer_insert is iterator (pointer) that points to divide between layer and overlay space.
	*/
	class LayerStack {
	private:
		std::vector<Layer*> m_layers;
		unsigned int m_insert_index = 0;
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }

	};
}