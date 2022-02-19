#pragma once
#include "Clove/Core/GameApp.h"

// The user should implement CreateApp from which to return a subclassed GameApp instance
extern Clove::GameApp* Clove::CreateApp();

int main(int argc, char** argv) {

	CLOVE_PROFILE_BEGIN_SESSION("Startup", "CloveProfileStartup.json");
	auto app = Clove::CreateApp();
	CLOVE_PROFILE_END_SESSION();

	CLOVE_PROFILE_BEGIN_SESSION("Runtime", "CloveProfileRuntime.json");
	app->Run();
	CLOVE_PROFILE_END_SESSION();

	CLOVE_PROFILE_BEGIN_SESSION("Shutdown", "CloveProfileShutdown.json");
	delete app;
	CLOVE_PROFILE_END_SESSION();
	
	return 0;
}