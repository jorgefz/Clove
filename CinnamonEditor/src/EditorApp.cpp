
#include <Clove.h>
#include "imgui.h"

#include "Clove/Core/EntryPoint.h" // Include only once!!

#include "EditorLayer.h"

#include "Clove/Core/Core.h"

namespace Clove {

	class CinnamonEditor : public Clove::GameApp {
	public:
		CinnamonEditor()
			: GameApp("Cinnamon Editor")
		{
			GameApp::PushLayer(new EditorLayer());
		}

		~CinnamonEditor() {

		}
	};

	Clove::GameApp* Clove::CreateApp() {
		return new CinnamonEditor();
	}

}


