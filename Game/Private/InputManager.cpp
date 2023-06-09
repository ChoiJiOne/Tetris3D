// @third party code - BEGIN
#include <SDL2/SDL.h>
// @third party code - END

#include "InputManager.h"

void InputManager::Setup()
{
	if (bIsSetup_) return;

	prevKeyboardState_.resize(static_cast<size_t>(EVirtualKey::CODE_NUM_SCANCODES));
	currKeyboardState_.resize(static_cast<size_t>(EVirtualKey::CODE_NUM_SCANCODES));

	bIsSetup_ = true;
}

void InputManager::Cleanup()
{
	if (!bIsSetup_) return;

	prevKeyboardState_.clear();
	currKeyboardState_.clear();

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

	const uint8_t* keyboardState = SDL_GetKeyboardState(NULL);

	std::memcpy(
		reinterpret_cast<void*>(&prevKeyboardState_[0]),
		reinterpret_cast<const void*>(&currKeyboardState_[0]),
		static_cast<int32_t>(EVirtualKey::CODE_NUM_SCANCODES)
	);

	std::memcpy(
		reinterpret_cast<void*>(&currKeyboardState_[0]),
		reinterpret_cast<const void*>(&keyboardState[0]),
		static_cast<int32_t>(EVirtualKey::CODE_NUM_SCANCODES)
	);

	prevCursorPosition_ = currCursorPosition_;
	prevMouseState_ = currMouseState_;

	currMouseState_ = SDL_GetMouseState(&currCursorPosition_.x, &currCursorPosition_.y);
}

EPressState InputManager::GetKeyPressState(const EVirtualKey& keyCode) const
{
	EPressState pressState = EPressState::NONE;

	if (IsPressKey(prevKeyboardState_, keyCode))
	{
		if (IsPressKey(currKeyboardState_, keyCode))
		{
			pressState = EPressState::HELD;
		}
		else
		{
			pressState = EPressState::RELEASED;
		}
	}
	else
	{
		if (IsPressKey(currKeyboardState_, keyCode))
		{
			pressState = EPressState::PRESSED;
		}
		else
		{
			pressState = EPressState::NONE;
		}
	}

	return pressState;
}

EPressState InputManager::GetMousePressState(const EMouseButton& mouseButton) const
{
	EPressState pressState = EPressState::NONE;

	if (IsPressMouseButton(prevMouseState_, mouseButton))
	{
		if (IsPressMouseButton(currMouseState_, mouseButton))
		{
			pressState = EPressState::HELD;
		}
		else
		{
			pressState = EPressState::RELEASED;
		}
	}
	else
	{
		if (IsPressMouseButton(currMouseState_, mouseButton))
		{
			pressState = EPressState::PRESSED;
		}
		else
		{
			pressState = EPressState::NONE;
		}
	}

	return pressState;
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

bool InputManager::IsPressKey(const std::vector<uint8_t>& keyboardState, const EVirtualKey& keyCode) const
{
	return keyboardState[static_cast<int32_t>(keyCode)] == 0 ? false : true;
}

bool InputManager::IsPressMouseButton(uint32_t buttonState, const EMouseButton& mouseButton) const
{
	uint32_t mask = 0;

	switch (mouseButton)
	{
	case EMouseButton::LEFT:
		mask = SDL_BUTTON_LMASK;
		break;

	case EMouseButton::RIGHT:
		mask = SDL_BUTTON_RMASK;
		break;

	default:
		ENFORCE_THROW_EXCEPTION("undefined mouse button...");
	}

	return (buttonState & mask) == 0 ? false : true;
}
