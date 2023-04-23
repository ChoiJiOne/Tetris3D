// @third party code - BEGIN
#include <SDL2/SDL.h>
// @third party code - END

#include "InputManager.h"

void InputManager::Setup()
{
	if (bIsSetup_) return;

	bIsSetup_ = true;
}

void InputManager::Cleanup()
{
	if (!bIsSetup_) return;

	bIsSetup_ = false;
}

void InputManager::Tick()
{
	SDL_Event eventState;

	while (SDL_PollEvent(&eventState))
	{
		EWindowEvent windowEvent = static_cast<EWindowEvent>(eventState.window.event);

		if (windowEventActions_.find(windowEvent) != windowEventActions_.end())
		{
			windowEventActions_[windowEvent]();
		}
	}
}

void InputManager::BindWindowEventAction(const EWindowEvent& windowEvent, const std::function<void()>& eventAction)
{
	CHECK((windowEventActions_.find(windowEvent) == windowEventActions_.end()), "collision window event key...");

	windowEventActions_.insert({ windowEvent, eventAction });
}

void InputManager::UnbindWindowEventAction(const EWindowEvent& windowEvent)
{
	if (windowEventActions_.find(windowEvent) != windowEventActions_.end())
	{
		windowEventActions_.erase(windowEvent);
	}
}

InputManager::~InputManager()
{
	Cleanup();
}
