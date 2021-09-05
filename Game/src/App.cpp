
#include "Clove.h"

class TestLayer : public Clove::Layer {
public:
	TestLayer() 
		: Layer()
	{

	}

	void OnUpdate() {

	}

	void OnEvent(Clove::Event& e) override {
		std::cout << "TestLayer event -> " << e.GetDebugName() << std::endl;
	}
};

class GameTest : public Clove::GameApp {
public:
	GameTest() {
		PushLayer( new TestLayer() ); // GameApp method

	}

	~GameTest() {

	}
};

Clove::GameApp* Clove::CreateApp() {
	return new GameTest();
}