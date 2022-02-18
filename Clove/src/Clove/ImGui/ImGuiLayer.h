#pragma once

#include "Clove/Core/Layer.h"

namespace Clove {

	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_block_events = block; }

	private:
		bool m_block_events = true;
		float m_time = 0.0f;
	};

}