#define _CRTDBG_MAP_ALLOC

#include <crtdbg.h>
#include <cstdint>


/**
 * @brief 애플리케이션의 진입점입니다.
 *
 * @param Argc 명령행 인자의 수입니다.
 * @param Argv 명령행 인자입니다.
 *
 * @return 다른 프로그램에 전달할 수 있는 상태 코드를 반환합니다.
 */
int32_t main(int32_t Argc, char** Argv)
{
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	return 0;
}