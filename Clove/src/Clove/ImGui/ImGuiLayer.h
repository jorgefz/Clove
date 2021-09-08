#pragma once

#include "Clove/GameApp.h"
#include "Clove/Core.h"
#include "Clove/Layer.h"

namespace Clove {

	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& e) override;

	private:
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnKeyPressed(KeyPressEvent& e);
		bool OnKeyReleased(KeyReleaseEvent& e);
		bool OnKeyTyped(KeyTypeEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		float m_time = 0.0f;
	};

}