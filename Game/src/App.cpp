
#include <Clove.h>
#include "imgui.h"

#include "Clove/Core/EntryPoint.h" // Include only once!!

#include "Game2D.h"
#include "TestLayer.h"
#include "Pong.h"


#include "Clove/Core/Core.h"

class GameTest : public Clove::GameApp {
public:
	GameTest() {
		//GameApp::PushLayer( new TestLayer() );
		GameApp::PushLayer( new Game2D() );
		//GameApp::PushLayer( new Pong() );
	}

	~GameTest() {
		
	}
};

Clove::GameApp* Clove::CreateApp() {
	return new GameTest();
}


