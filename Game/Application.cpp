#define _CRTDBG_MAP_ALLOC

#include <SDL2/SDL.h>

#include <crtdbg.h>
#include <memory>

#include "CommandLine.h"
#include "CrashHandler.h"
#include "Window.h"


/**
 * @brief Tetris3D 게임을 초기화 및 실행합니다.
 */
class Tetris3D
{
public:
	/**
	 * @brief Tetris3D 게임을 초기화 및 실행하는 클래스의 디폴트 생성자입니다.
	 * 
	 * @note 이 클래스를 초기화하기 위해서는 Setup 메서드를 호출해야 합니다.
	 */
	Tetris3D() = default;


	/**
	 * @brief Tetris3D 게임을 초기화 및 실행하는 클래스의 가상 생성자입니다.
	 * 
	 * @note 이 클래스에서 사용한 리소스를 정리하기 위해서는 Cleanup 메서드를 호출해야 합니다.
	 */
	virtual ~Tetris3D() {}


	/**
	 * @brief Tetris3D 클래스의 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Tetris3D);


	/**
	 * @brief Tetris3D 게임을 초기화합니다.
	 * 
	 * @throws Tetris3D 게임 초기화에 실패하면 표준 예외를 던집니다.
	 */
	void Setup()
	{
		CHECK(SDL_Init(SDL_INIT_EVERYTHING) == 0, "failed to initialize SDL2...");

		window_ = std::make_unique<Window>("Tetris3D", 200, 200, 1000, 800, EWindowFlags::SHOWN);
	}


	/**
	 * @brief Tetris3D 게임을 실행합니다.
	 * 
	 * @throws Tetris3D 게임 실행에 실패하면 표준 예외를 던집니다. 
	 */
	void Run()
	{
		bool quit = false;
		SDL_Event event;

		while (!quit)
		{
			while (SDL_PollEvent(&event)) 
			{
				if (event.type == SDL_QUIT) 
				{
					quit = true;
				}
			}
		}
	}


	/**
	 * @brief Tetris3D 게임 내의 리소스를 정리합니다.
	 */
	void Cleanup()
	{
		window_.reset();

		SDL_Quit();
	}


private:
	/**
	 * @brief 게임 윈도우 입니다.
	 */
	std::unique_ptr<Window> window_ = nullptr;
};


/**
 * @brief 애플리케이션을 초기화하고 실행합니다.
 * 
 * @param argc 명령행 인자의 수입니다.
 * @param argv 명령행 인자입니다.
 *
 * @throws
 * - 초기화에 실패하면 표준 예외를 던집니다.
 * - 실행에 실패하면 표준 예외를 던집니다.
 */
void RunApplication(int32_t argc, char** argv)
{
	CommandLine::Parse(argc, argv);
	CrashHandler::SetCrashDumpFilePath(CommandLine::GetValue("Crash"));

	std::unique_ptr<Tetris3D> gameApplication = std::make_unique<Tetris3D>();

	gameApplication->Setup();
	gameApplication->Run();
	gameApplication->Cleanup();
}


/**
 * @brief 애플리케이션의 진입점입니다.
 *
 * @param argc 명령행 인자의 수입니다.
 * @param argv 명령행 인자입니다.
 *
 * @return 다른 프로그램에 전달할 수 있는 상태 코드를 반환합니다.
 */
int32_t main(int32_t argc, char** argv)
{
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	__try
	{
		RunApplication(argc, argv);
	}
	__except (CrashHandler::DetectApplicationCrash(GetExceptionInformation()))
	{
		CrashHandler::CrashErrorMessageBox();
	}

	return 0;
}