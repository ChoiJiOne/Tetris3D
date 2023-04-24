#pragma once

#include <string>

#include "Macro.h"

struct SDL_Window;


/**
 * @brief 윈도우 옵션 플래그입니다.
 * 
 * @note https://wiki.libsdl.org/SDL2/SDL_WindowFlags
 */
enum class EWindowFlags : uint32_t
{
	FULLSCREEN = 0x00000001,
	OPENGL = 0x00000002,
	SHOWN = 0x00000004,
	HIDDEN = 0x00000008,
	BORDERLESS = 0x00000010,
	RESIZABLE = 0x00000020,
	MINIMIZED = 0x00000040,
	MAXIMIZED = 0x00000080,
	MOUSE_GRABBED = 0x00000100,
	INPUT_FOCUS = 0x00000200,
	MOUSE_FOCUS = 0x00000400,
	FULLSCREEN_DESKTOP = (FULLSCREEN | 0x00001000),
	FOREIGN = 0x00000800,
	ALLOW_HIGHDPI = 0x00002000,
	MOUSE_CAPTURE = 0x00004000,
	KEYBOARD_GRABBED = 0x00100000,
	VULKAN = 0x10000000,
};


/**
 * @brief 윈도우 옵션 플래그의 OR(|) 연산을 수행합니다.
 *
 * @param lhs OR(|) 연산의 우측 피연산자입니다.
 * @param rhs OR(|) 연산의 좌측 피연산자입니다.
 *
 * @return OR(|) 연산을 수행한 결과를 반환합니다.
 */
inline EWindowFlags operator|(const EWindowFlags& lhs, const EWindowFlags& rhs)
{
	return static_cast<EWindowFlags>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
}


/**
 * @brief 윈도우 창을 생성하고 관리합니다.
 */
class Window
{
public:
	/**
	 * @brief 윈도우 창을 생성하고 관리하는 클래스 생성자 파라미터의 구조체입니다.
	 */
	struct ConstructorParam
	{
		const std::string& title; /** 윈도우 타이틀입니다. */
		int32_t x;                /** 윈도우의 X좌표입니다. */
		int32_t y;                /** 윈도우의 Y좌표입니다. */
		int32_t w;                /** 윈도우의 가로 크기입니다. */
		int32_t h;                /** 윈도우의 세로 크기입니다. */
		EWindowFlags flags;       /** 윈도우의 생성 옵션입니다. */
	};


	/**
	 * @brief 윈도우 창을 생성하고 관리하는 클래스의 생성자입니다.
	 *
	 * @param constructorParam 윈도우 클래스의 생성 파라미터입니다.
	 *
	 * @throws 윈도우 생성에 실패하면 C++ 표준 예외를 던집니다.
	 *
	 * @see https://wiki.libsdl.org/SDL_CreateWindow
	 */
	explicit Window(const ConstructorParam& constructorParam);


	/**
	 * @brief 윈도우 클래스의 생성자입니다.
	 *
	 * @param title 윈도우의 타이틀입니다.
	 * @param x 윈도우 왼쪽 상단의 X좌표입니다.
	 * @param y 윈도우 왼쪽 상단의 Y좌표입니다.
	 * @param width 윈도우의 가로 크기입니다.
	 * @param height 윈도우의 세로 크기입니다.
	 * @param flags 윈도우 옵션입니다.
	 *
	 * @throws 윈도우 생성에 실패하면 C++ 표준 예외를 던집니다.
	 *
	 * @see https://wiki.libsdl.org/SDL_CreateWindow
	 */
	explicit Window(const std::string& title, int32_t x, int32_t y, int32_t width, int32_t height, const EWindowFlags& flags);


	/**
	 * @brief 윈도우 클래스의 가상 소멸자입니다
	 */
	virtual ~Window();


	/**
	 * @brief 윈도우 클래스의 복사 생성자와 대입 연산자를 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Window);


	/**
	 * @brief 게임 윈도우 위치를 변경합니다.
	 * 
	 * @note 이때, 단위는 픽셀단위입니다.
	 *
	 * @param x 스크린 상의 게임 윈도우 왼쪽 상단의 X좌표입니다.
	 * @param y 스크린 상의 게임 윈도우 왼쪽 상단의 Y좌표입니다.
	 *
	 * @see https://wiki.libsdl.org/SDL_SetWindowPosition
	 */
	void SetPosition(const int32_t& x, const int32_t& y);


	/**
	 * @brief 게임 윈도우 위치를 얻습니다.
	 * 
	 * @note 이때, 단위는 픽셀단위입니다.
	 *
	 * @param outX[out] 스크린 상의 게임 윈도우 왼쪽 상단의 X좌표입니다.
	 * @param outY[out] 스크린 상의 게임 윈도우 왼쪽 상단의 Y좌표입니다.
	 *
	 * @see https://wiki.libsdl.org/SDL_GetWindowPosition
	 */
	void GetPosition(int32_t& outX, int32_t& outY);


	/**
	 * @brief 게임 윈도우의 크기를 변경합니다.
	 * 
	 * @note 이때, 단위는 픽셀단위입니다.
	 *
	 * @param width 스크린 상의 게임 윈도우의 가로 크기입니다.
	 * @param height 스크린 상의 게임 윈도우의 세로 크기입니다.
	 *
	 * @see https://wiki.libsdl.org/SDL_SetWindowSize
	 */
	void SetSize(const int32_t& width, const int32_t& height);


	/**
	 * @brief 게임 윈도우의 크기를 얻습니다.
	 * 
	 * @note 이때, 단위는 픽셀단위입니다.
	 *
	 * @param outWidth 스크린 상의 게임 윈도우의 가로 크기입니다.
	 * @param outHeight 스크린 상의 게임 윈도우의 세로 크기입니다.
	 *
	 * @see https://wiki.libsdl.org/SDL_GetWindowSize
	 */
	void GetSize(int32_t& outWidth, int32_t& outHeight);


	/**
	 * @brief 게임 윈도우의 타이틀을 설정합니다.
	 *
	 * @param title 설정할 게임 윈도우 타이틀입니다.
	 *
	 * @see https://wiki.libsdl.org/SDL_SetWindowTitle
	 */
	void SetTitle(const std::string& title);


	/**
	 * @brief 게임 윈도우의 타이틀을 얻습니다.
	 *
	 * @return 게임 윈도우 타이틀을 반환합니다.
	 *
	 * @see https://wiki.libsdl.org/SDL_GetWindowTitle
	 */
	std::string GetTitle();


	/**
	 * @brief 게임 윈도우를 화면에 표시합니다.
	 */
	void Show();


	/**
	 * @brief 게임 윈도우를 화면에서 숨깁니다.
	 */
	void Hide();


	/**
	 * @brief 윈도우의 크기 비율을 얻습니다.
	 * 
	 * @note 이때 비율 값은 가로 / 세로 입니다.
	 *
	 * @return 게임 윈도우의 크기 비율을 반환합니다.
	 */
	float GetAspectRatio();


	/**
	 * SDL윈도우의 포인터를 반환합니다.
	 *
	 * @return SDL 윈도우의 포인터를 반환합니다.
	 */
	SDL_Window* GetWindow() { return window_; }


	/**
	 * @brief SDL 윈도우가 관리하는 네이티브 윈도우 핸들을 얻습니다.
	 * 
	 * @return 네이티브 윈도우 핸들을 반환합니다.
	 * 
	 * @see
	 * https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types#:~:text=typedef%20HANDLE%20WINSTA%3B-,HWND,-A%20handle%20to
	 * https://stackoverflow.com/questions/24117983/get-window-handle-of-sdl-2-application
	 */
	HWND GetNativeHandle();


private:
	/**
	 * SDL 윈도우 포인터입니다.
	 */
	SDL_Window* window_ = nullptr;
};