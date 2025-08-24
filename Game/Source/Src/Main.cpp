#include <memory>

#include <raylib.h>

#include "GameApp.h"

int main(int argc, char* argv[])
{
	std::unique_ptr<GameApp> gameApp = std::make_unique<GameApp>();
	gameApp->Startup();
	gameApp->Run();
	gameApp->Shutdown();
	return 0;
}