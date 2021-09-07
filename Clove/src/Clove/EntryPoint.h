#pragma once
#include "clovepch.h"

extern Clove::GameApp* Clove::CreateApp();

int main(int argc, char** argv) {
	auto app = Clove::CreateApp();
	app->Run();
	delete app;
	return 0;
}