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
 * @brief 윈도우 옵션 플래그의 or 연산을 수행합니다.
 *
 * @param lhs or 연산의 우측 피연산자입니다.
 * @param rhs or 연산의 좌측 피연산자입니다.
 *
 * @return or 연산을 수행한 결과를 반환합니다.
 */
inline EWindowFlags operator|(const EWindowFlags& lhs, const EWindowFlags& rhs)
{
	return static_cast<EWindowFlags>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
}