#pragma once

#include <directxmath.h>
#include <unordered_map>
#include <functional>
#include <vector>

#include "Macro.h"


/**
 * @brief 입력 상태를 나타냅니다.
 * 
 * @note 프레임에 따른 입력 상태입니다.
 * --------------------------------------
 * | 이전 프레임 | 현재 프레임 | 입력 상태 |
 * --------------------------------------
 * |     0      |     0      | NONE     |
 * |     0      |     1      | PRESSED  |
 * |     1      |     0      | RELEASED |
 * |     1      |     1      | HELD     |
 * --------------------------------------
 */
enum class EPressState : int32_t
{
	NONE = 0x00,
	PRESSED = 0x01,
	RELEASED = 0x02,
	HELD = 0x03
};


/**
 * @brief 마우스의 버튼의 종류입니다.
 * 
 * @note
 * LEFT는 마우스의 왼쪽 버튼입니다.
 * RIGHT는 마우스의 오른쪽 버튼입니다.
 */
enum class EMouseButton : int32_t
{
    LEFT  = 0x0,
    RIGHT = 0x1,
};


/**
 * @brief 가상 키 코드입니다.
 * 
 * @see https://wiki.libsdl.org/SDL_Scancode
 */
enum class EVirtualKey : int32_t
{
	CODE_UNKNOWN = 0,
	CODE_A = 4,
	CODE_B = 5,
	CODE_C = 6,
	CODE_D = 7,
	CODE_E = 8,
	CODE_F = 9,
	CODE_G = 10,
	CODE_H = 11,
	CODE_I = 12,
	CODE_J = 13,
	CODE_K = 14,
	CODE_L = 15,
	CODE_M = 16,
	CODE_N = 17,
	CODE_O = 18,
	CODE_P = 19,
	CODE_Q = 20,
	CODE_R = 21,
	CODE_S = 22,
	CODE_T = 23,
	CODE_U = 24,
	CODE_V = 25,
	CODE_W = 26,
	CODE_X = 27,
	CODE_Y = 28,
	CODE_Z = 29,
	CODE_1 = 30,
	CODE_2 = 31,
	CODE_3 = 32,
	CODE_4 = 33,
	CODE_5 = 34,
	CODE_6 = 35,
	CODE_7 = 36,
	CODE_8 = 37,
	CODE_9 = 38,
	CODE_0 = 39,
	CODE_RETURN = 40,
	CODE_ESCAPE = 41,
	CODE_BACKSPACE = 42,
	CODE_TAB = 43,
	CODE_SPACE = 44,
	CODE_MINUS = 45,
	CODE_EQUALS = 46,
	CODE_LEFTBRACKET = 47,
	CODE_RIGHTBRACKET = 48,
	CODE_BACKSLASH = 49,
	CODE_NONUSHASH = 50,
	CODE_SEMICOLON = 51,
	CODE_APOSTROPHE = 52,
	CODE_GRAVE = 53,
	CODE_COMMA = 54,
	CODE_PERIOD = 55,
	CODE_SLASH = 56,
	CODE_CAPSLOCK = 57,
	CODE_F1 = 58,
	CODE_F2 = 59,
	CODE_F3 = 60,
	CODE_F4 = 61,
	CODE_F5 = 62,
	CODE_F6 = 63,
	CODE_F7 = 64,
	CODE_F8 = 65,
	CODE_F9 = 66,
	CODE_F10 = 67,
	CODE_F11 = 68,
	CODE_F12 = 69,
	CODE_PRINTSCREEN = 70,
	CODE_SCROLLLOCK = 71,
	CODE_PAUSE = 72,
	CODE_INSERT = 73,
	CODE_HOME = 74,
	CODE_PAGEUP = 75,
	CODE_DELETE = 76,
	CODE_END = 77,
	CODE_PAGEDOWN = 78,
	CODE_RIGHT = 79,
	CODE_LEFT = 80,
	CODE_DOWN = 81,
	CODE_UP = 82,
	CODE_NUMLOCKCLEAR = 83,
	CODE_KP_DIVIDE = 84,
	CODE_KP_MULTIPLY = 85,
	CODE_KP_MINUS = 86,
	CODE_KP_PLUS = 87,
	CODE_KP_ENTER = 88,
	CODE_KP_1 = 89,
	CODE_KP_2 = 90,
	CODE_KP_3 = 91,
	CODE_KP_4 = 92,
	CODE_KP_5 = 93,
	CODE_KP_6 = 94,
	CODE_KP_7 = 95,
	CODE_KP_8 = 96,
	CODE_KP_9 = 97,
	CODE_KP_0 = 98,
	CODE_KP_PERIOD = 99,
	CODE_NONUSBACKSLASH = 100,
	CODE_APPLICATION = 101,
	CODE_POWER = 102,
	CODE_KP_EQUALS = 103,
	CODE_F13 = 104,
	CODE_F14 = 105,
	CODE_F15 = 106,
	CODE_F16 = 107,
	CODE_F17 = 108,
	CODE_F18 = 109,
	CODE_F19 = 110,
	CODE_F20 = 111,
	CODE_F21 = 112,
	CODE_F22 = 113,
	CODE_F23 = 114,
	CODE_F24 = 115,
	CODE_EXECUTE = 116,
	CODE_HELP = 117,
	CODE_MENU = 118,
	CODE_SELECT = 119,
	CODE_STOP = 120,
	CODE_AGAIN = 121,
	CODE_UNDO = 122,
	CODE_CUT = 123,
	CODE_COPY = 124,
	CODE_PASTE = 125,
	CODE_FIND = 126,
	CODE_MUTE = 127,
	CODE_VOLUMEUP = 128,
	CODE_VOLUMEDOWN = 129,
	CODE_KP_COMMA = 133,
	CODE_KP_EQUALSAS400 = 134,
	CODE_INTERNATIONAL1 = 135,
	CODE_INTERNATIONAL2 = 136,
	CODE_INTERNATIONAL3 = 137,
	CODE_INTERNATIONAL4 = 138,
	CODE_INTERNATIONAL5 = 139,
	CODE_INTERNATIONAL6 = 140,
	CODE_INTERNATIONAL7 = 141,
	CODE_INTERNATIONAL8 = 142,
	CODE_INTERNATIONAL9 = 143,
	CODE_LANG1 = 144,
	CODE_LANG2 = 145,
	CODE_LANG3 = 146,
	CODE_LANG4 = 147,
	CODE_LANG5 = 148,
	CODE_LANG6 = 149,
	CODE_LANG7 = 150,
	CODE_LANG8 = 151,
	CODE_LANG9 = 152,
	CODE_ALTERASE = 153,
	CODE_SYSREQ = 154,
	CODE_CANCEL = 155,
	CODE_CLEAR = 156,
	CODE_PRIOR = 157,
	CODE_RETURN2 = 158,
	CODE_SEPARATOR = 159,
	CODE_OUT = 160,
	CODE_OPER = 161,
	CODE_CLEARAGAIN = 162,
	CODE_CRSEL = 163,
	CODE_EXSEL = 164,
	CODE_KP_00 = 176,
	CODE_KP_000 = 177,
	CODE_THOUSANDSSEPARATOR = 178,
	CODE_DECIMALSEPARATOR = 179,
	CODE_CURRENCYUNIT = 180,
	CODE_CURRENCYSUBUNIT = 181,
	CODE_KP_LEFTPAREN = 182,
	CODE_KP_RIGHTPAREN = 183,
	CODE_KP_LEFTBRACE = 184,
	CODE_KP_RIGHTBRACE = 185,
	CODE_KP_TAB = 186,
	CODE_KP_BACKSPACE = 187,
	CODE_KP_A = 188,
	CODE_KP_B = 189,
	CODE_KP_C = 190,
	CODE_KP_D = 191,
	CODE_KP_E = 192,
	CODE_KP_F = 193,
	CODE_KP_XOR = 194,
	CODE_KP_POWER = 195,
	CODE_KP_PERCENT = 196,
	CODE_KP_LESS = 197,
	CODE_KP_GREATER = 198,
	CODE_KP_AMPERSAND = 199,
	CODE_KP_DBLAMPERSAND = 200,
	CODE_KP_VERTICALBAR = 201,
	CODE_KP_DBLVERTICALBAR = 202,
	CODE_KP_COLON = 203,
	CODE_KP_HASH = 204,
	CODE_KP_SPACE = 205,
	CODE_KP_AT = 206,
	CODE_KP_EXCLAM = 207,
	CODE_KP_MEMSTORE = 208,
	CODE_KP_MEMRECALL = 209,
	CODE_KP_MEMCLEAR = 210,
	CODE_KP_MEMADD = 211,
	CODE_KP_MEMSUBTRACT = 212,
	CODE_KP_MEMMULTIPLY = 213,
	CODE_KP_MEMDIVIDE = 214,
	CODE_KP_PLUSMINUS = 215,
	CODE_KP_CLEAR = 216,
	CODE_KP_CLEARENTRY = 217,
	CODE_KP_BINARY = 218,
	CODE_KP_OCTAL = 219,
	CODE_KP_DECIMAL = 220,
	CODE_KP_HEXADECIMAL = 221,
	CODE_LCTRL = 224,
	CODE_LSHIFT = 225,
	CODE_LALT = 226,
	CODE_LGUI = 227,
	CODE_RCTRL = 228,
	CODE_RSHIFT = 229,
	CODE_RALT = 230,
	CODE_RGUI = 231,
	CODE_MODE = 257,
	CODE_AUDIONEXT = 258,
	CODE_AUDIOPREV = 259,
	CODE_AUDIOSTOP = 260,
	CODE_AUDIOPLAY = 261,
	CODE_AUDIOMUTE = 262,
	CODE_MEDIASELECT = 263,
	CODE_WWW = 264,
	CODE_MAIL = 265,
	CODE_CALCULATOR = 266,
	CODE_COMPUTER = 267,
	CODE_AC_SEARCH = 268,
	CODE_AC_HOME = 269,
	CODE_AC_BACK = 270,
	CODE_AC_FORWARD = 271,
	CODE_AC_STOP = 272,
	CODE_AC_REFRESH = 273,
	CODE_AC_BOOKMARKS = 274,
	CODE_BRIGHTNESSDOWN = 275,
	CODE_BRIGHTNESSUP = 276,
	CODE_DISPLAYSWITCH = 277,
	CODE_KBDILLUMTOGGLE = 278,
	CODE_KBDILLUMDOWN = 279,
	CODE_KBDILLUMUP = 280,
	CODE_EJECT = 281,
	CODE_SLEEP = 282,
	CODE_APP1 = 283,
	CODE_APP2 = 284,
	CODE_AUDIOREWIND = 285,
	CODE_AUDIOFASTFORWARD = 286,
	CODE_SOFTLEFT = 287,
	CODE_SOFTRIGHT = 288,
	CODE_CALL = 289,
	CODE_ENDCALL = 290,
	CODE_NUM_SCANCODES = 512
};


/**
 * @brief 윈도우 이벤트 키 값입니다.
 * 
 * @note https://wiki.libsdl.org/SDL_WindowEventID
 */
enum class EWindowEvent : int32_t
{
	NONE = 0,
	SHOWN = 1,
	HIDDEN = 2,
	EXPOSED = 3,
	MOVED = 4,
	RESIZED = 5,
	SIZE_CHANGED = 6,
	MINIMIZED = 7,
	MAXIMIZED = 8,
	RESTORED = 9,
	ENTER = 10,
	LEAVE = 11,
	FOCUS_GAINED = 12,
	FOCUS_LOST = 13,
	CLOSE = 14,
	TAKE_FOCUS = 15,
	HIT_TEST = 16,
	ICCPROF_CHANGED = 17,
	DISPLAY_CHANGED = 18
};


/**
 * @brief 입력 상태를 업데이트하고 처리합니다.
 * 
 * @note 이 클래스는 싱글턴으로 헤더만 추가하면 바로 사용할 수 있습니다.
 */
class InputManager
{
public:
	/**
	 * @brief 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(InputManager);


	/**
	 * @brief InputManager의 인스턴스를 얻습니다.
	 * 
	 * @return InputManager의 인스턴스 참조자를 반환합니다.
	 */
	static InputManager& Get()
	{
		static InputManager instance;
		return instance;
	}


	/**
	 * @brief InputManager를 명시적으로 초기화합니다.
	 * 
	 * @note 초기화된 적이 없거나, 초기화 이후 Cleanup이 호출되지 않았다면 아무 동작도 수행하지 않습니다.
	 */
	void Setup();


	/**
	 * @brief InputManager의 내부 요소를 명시적으로 정리합니다.
	 * 
	 * @note 초기화한 적이 없거나 Cleanup 호출 이후에 Setup을 호출하지 않았다면 아무 동작도 수행하지 않습니다.
	 */
	void Cleanup();


	/**
	 * @brief 입력 상태를 업데이트하고, 윈도우 이벤트 액션을 실행합니다.
	 */
	void Tick();


	/**
	 * @brief 키의 입력 상태를 확인합니다.
	 * 
	 * @param keyCode 입력 상태를 확인할 키의 코드값입니다.
	 * 
	 * @return 키의 입력 상태를 반환합니다.
	 */
	EPressState GetKeyPressState(const EVirtualKey& keyCode) const;


	/**
	 * @brief 마우스의 입력 상태를 확인합니다.
	 * 
	 * @param mouseButton 입력 상태를 확인할 마우스 버튼입니다.
	 * 
	 * @return 마우스의 입력 상태를 반환합니다.
	 */
	EPressState GetMousePressState(const EMouseButton& mouseButton) const;

	
	/**
	 * @brief 윈도우 이벤트에 동작할 액션을 바인딩합니다.
	 * 
	 * @param windowEvent 동작할 액션에 대응하는 윈도우 이벤트입니다.
	 * @param eventAction 윈도우 이벤트 감지될 경우 실행할 액션입니다.
	 */
	void BindWindowEventAction(const EWindowEvent& windowEvent, const std::function<void()>& eventAction);


	/**
	 * @brief 윈도우 이벤트에 동작할 액션의 바인딩을 해제합니다.
	 * 
	 * @param windowEvent 바인딩 해제할 윈도우 이벤트에 대응하는 액션입니다.
	 */
	void UnbindWindowEventAction(const EWindowEvent& windowEvent);


private:
	/**
	 * @brief 입력 상태를 업데이트하고 처리하는 클래스의 생성자입니다.
	 * 
	 * @note 이 생성자는 아무 동작도 수행하지 않습니다.
	 */
	InputManager() = default;


	/**
	 * @brief 입력 상태를 업데이트하고 처리하는 클래스의 가상 소멸자입니다.
	 */
	virtual ~InputManager();


	/**
	 * @brief 특정 키가 눌렸는지 확인합니다.
	 * 
	 * @param keyboardState 검사를 수행할 키보드 상태입니다.
	 * @param keyCode 눌렸는지 확인할 키의 코드값입니다.
	 * 
	 * @return 키가 눌렸다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool IsPressKey(const std::vector<uint8_t>& keyboardState, const EVirtualKey& keyCode) const;


	/**
	 * @brief 마우스 버튼이 눌렸는지 확인합니다.
	 * 
	 * @param buttonState 눌렸는지 확인할 마우스 버튼의 상태입니다.
	 * @param mouseButton 눌렸는지 확인할 마우스 버튼입니다.
	 * 
	 * @return 마우스 버튼이 눌렸다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool IsPressMouseButton(uint32_t buttonState, const EMouseButton& mouseButton) const;


private:
	/**
	 * @brief 초기화된 적이 있는지 확인합니다.
	 */
	bool bIsSetup_ = false;


	/**
	 * @brief Tick 호출 이전의 키보드 상태입니다.
	 */
	std::vector<uint8_t> prevKeyboardState_;


	/**
	 * @brief Tick 호출 이후의 키보드 상태입니다.
	 */
	std::vector<uint8_t> currKeyboardState_;


	/**
	 * @brief Tick 호출 이전의 마우스 위치입니다.
	 * 
	 * @note 마우스 위치는 윈도우 좌표계 기준입니다.
	 */
	DirectX::XMINT2 prevCursorPosition_;


	/**
	 * @brief Tick 호출 이전의 마우스 상태입니다.
	 */
	uint32_t prevMouseState_ = 0;


	/**
	 * @brief Tick 호출 이후의 마우스 위치입니다.
	 * 
	 * @note 마우스 위치는 윈도우 좌표계 기준입니다.
	 */
	DirectX::XMINT2 currCursorPosition_;


	/**
	 * @brief Tick 호출 이후의 마우스 상태입니다.
	 */
	uint32_t currMouseState_ = 0;


	/**
	 * @brief 윈도우 이벤트에 대응하는 액션입니다.
	 */
	std::unordered_map<EWindowEvent, std::function<void()>> windowEventActions_;
};