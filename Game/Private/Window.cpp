// @third party code - BEGIN
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
// @third party code - END

#include "Window.h"

Window::Window(const ConstructorParam& constructorParam)
	: Window(constructorParam.title, constructorParam.x, constructorParam.y, constructorParam.w, constructorParam.h, constructorParam.flags)
{
}

Window::Window(const std::string& title, int32_t x, int32_t y, int32_t width, int32_t height, const EWindowFlags& flags)
{
	window_ = SDL_CreateWindow(title.c_str(), x, y, width, height, static_cast<uint32_t>(flags));
	CHECK((window_ != nullptr), "failed to create window...");
}

Window::~Window()
{
	if (window_)
	{
		SDL_DestroyWindow(window_);
		window_ = nullptr;
	}
}

void Window::SetPosition(const int32_t& x, const int32_t& y)
{
	SDL_SetWindowPosition(window_, x, y);
}

void Window::GetPosition(int32_t& outX, int32_t& outY)
{
	SDL_GetWindowPosition(window_, &outX, &outY);
}

void Window::SetSize(const int32_t& width, const int32_t& height)
{
	SDL_SetWindowSize(window_, width, height);
}

void Window::GetSize(int32_t& outWidth, int32_t& outHeight)
{
	SDL_GetWindowSize(window_, &outWidth, &outHeight);
}

void Window::SetTitle(const std::string& title)
{
	SDL_SetWindowTitle(window_, title.c_str());
}

std::string Window::GetTitle()
{
	return std::string(SDL_GetWindowTitle(window_));
}

void Window::Show()
{
	SDL_ShowWindow(window_);
}

void Window::Hide()
{
	SDL_HideWindow(window_);
}

float Window::GetAspectRatio()
{
	int32_t width, height;
	GetSize(width, height);

	return static_cast<float>(width) / static_cast<float>(height);
}

HWND Window::GetNativeHandle()
{
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);

	CHECK(SDL_GetWindowWMInfo(window_, &wmInfo) == SDL_TRUE, "failed to get native window info...");

	return wmInfo.info.win.window;
}
