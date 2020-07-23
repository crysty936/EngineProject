#pragma once

#include "Engine/EventsManager/Events/EventBase.h"
#include "Engine/EventsManager/Events/ApplicationEvents.h"
#include "Engine/EventsManager/EventDispatcher.h"
#include "Engine/EventsManager/Events/EventManager.h"


#ifdef PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
	Engine::Application* app = Engine::CreateApplication();
	app->Run();
}




#endif // PLATFORM_WINDOWS




