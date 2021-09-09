#pragma once
#include "Clove/Core.h"

namespace Clove {

	class Input {
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePos();
		static float GetMouseX();
		static float GetMouseY();
	private:
		static Input* m_instance;
	};

}