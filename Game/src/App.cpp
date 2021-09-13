
#include "Clove.h"

class TestLayer : public Clove::Layer {
public:
	TestLayer() 
		: Layer()
	{

	}

	void OnUpdate() override {

	}

	void OnEvent(Clove::Event& e) override {
		if (e.GetEventType() == Clove::EventType::KeyPressed) {
			int key = dynamic_cast<Clove::KeyPressEvent&>(e).GetKeyCode();
			if (key > 0 && key < CHAR_MAX) {
				std::cout << (char)key;
			}
		}
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